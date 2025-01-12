/*
 * Copyright 2014-present Alibaba Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "suez/drc/SwiftSource.h"

#include <sstream>

#include "autil/Log.h"
#include "build_service/util/SwiftClientCreator.h"
#include "build_service/util/SwiftTopicStreamReaderCreator.h"
#include "suez/drc/DrcConfig.h"
#include "swift/client/SwiftReader.h"
#include "swift/protocol/SwiftMessage.pb.h"

namespace suez {
AUTIL_DECLARE_AND_SETUP_LOGGER(suez.drc, SwiftSource);

const std::string SwiftSource::TYPE = "swift";

SwiftSource::SwiftSource(std::unique_ptr<swift::client::SwiftReader> reader) : _swiftReader(std::move(reader)) {}

SwiftSource::~SwiftSource() {}

const std::string &SwiftSource::getType() const { return TYPE; }

bool SwiftSource::seek(int64_t offset) {
    auto ec = _swiftReader->seekByTimestamp(offset, /*force*/ true);
    if (ec != swift::protocol::ERROR_NONE && ec != swift::protocol::ERROR_CLIENT_NO_MORE_MESSAGE) {
        AUTIL_LOG(ERROR, "seekByTimestamp with %ld failed", offset);
        return false;
    }
    return true;
}

bool SwiftSource::read(std::string &data, int64_t &logId) {
    swift::protocol::Message msg;
    int64_t offset;
    auto ec = _swiftReader->read(offset, msg);
    if (ec != swift::protocol::ERROR_NONE) {
        if (ec != swift::protocol::ERROR_CLIENT_NO_MORE_MESSAGE) {
            AUTIL_INTERVAL_LOG(
                1000, ERROR, "read from swift failed, error: %s", swift::protocol::ErrorCode_Name(ec).c_str());
        }
        AUTIL_LOG(DEBUG, "no more message");
        return false;
    }
    logId = msg.timestamp();
    auto messageData = msg.release_data();
    data = std::move(*messageData);
    delete messageData;
    return true;
}

int64_t SwiftSource::getLatestLogId() const {
    auto status = _swiftReader->getSwiftPartitionStatus();
    return status.maxMessageTimestamp;
}

bool SwiftSourceConfig::init(const std::map<std::string, std::string> &params) {
    swiftRoot = getParameter(params, "swift_root");
    if (swiftRoot.empty()) {
        return false;
    }
    topicStreamMode = getParameter(params, "topic_stream_mode") == "true";
    topicName = getParameter(params, "topic_name");
    if (!topicStreamMode && topicName.empty()) {
        return false;
    }
    topicListStr = getParameter(params, "topic_list");
    clientConfig = getParameter(params, "swift_client_config");
    readerConfig = getParameter(params, "reader_config");
    if (!getParameterT(params, "need_field_filter", needFieldFilter)) {
        return false;
    }
    if (!getParameterT(params, "from", from)) {
        return false;
    }
    if (!getParameterT(params, "to", to)) {
        return false;
    }
    if (from > to || to > 65535) {
        return false;
    }
    getParameterT(params, "swift_start_timestamp", startTimestamp);
    return true;
}

std::string SwiftSourceConfig::constructReadConfigStr() const {
    std::stringstream ss;
    if (!topicStreamMode) {
        ss << "topicName=" << topicName << ";";
    }
    ss << "from=" << from << ";";
    ss << "to=" << to;
    if (!readerConfig.empty()) {
        ss << ";" << readerConfig;
    }
    return ss.str();
}

std::unique_ptr<swift::client::SwiftReader>
SwiftSource::createSwiftReader(const std::shared_ptr<build_service::util::SwiftClientCreator> &client,
                               const std::map<std::string, std::string> &param) {
    SwiftSourceConfig swiftSourceConfig;
    if (!swiftSourceConfig.init(param)) {
        AUTIL_LOG(ERROR, "invalid swift source config [%s]", autil::StringUtil::toString(param).c_str());
        return nullptr;
    }
    AUTIL_LOG(INFO,
              "create swift client, swift_root: %s, client config: %s",
              swiftSourceConfig.swiftRoot.c_str(),
              swiftSourceConfig.clientConfig.c_str());
    auto swiftClient = client->createSwiftClient(swiftSourceConfig.swiftRoot, swiftSourceConfig.clientConfig);
    if (!swiftClient) {
        AUTIL_LOG(ERROR,
                  "create swift client failed, swift_root: %s, client config: %s",
                  swiftSourceConfig.swiftRoot.c_str(),
                  swiftSourceConfig.clientConfig.c_str());
        return nullptr;
    }

    std::unique_ptr<swift::client::SwiftReader> reader;
    auto readerConfigStr = swiftSourceConfig.constructReadConfigStr();
    if (!swiftSourceConfig.topicStreamMode) {
        swift::protocol::ErrorInfo error;
        reader.reset(swiftClient->createReader(readerConfigStr, &error));
        if (!reader) {
            AUTIL_LOG(ERROR,
                      "create swift reader with config %s failed, error: %s",
                      readerConfigStr.c_str(),
                      error.ShortDebugString().c_str());
            return nullptr;
        }
        if (swiftSourceConfig.startTimestamp > 0) {
            auto ec = reader->seekByTimestamp(swiftSourceConfig.startTimestamp, true);
            if (ec != swift::protocol::ERROR_NONE) {
                AUTIL_LOG(ERROR,
                          "seek to %ld failed, error: %s",
                          swiftSourceConfig.startTimestamp,
                          swift::protocol::ErrorCode_Name(ec).c_str());
                return nullptr;
            }
        }
    } else {
        reader = build_service::util::SwiftTopicStreamReaderCreator::create(
            swiftClient, swiftSourceConfig.topicListStr, readerConfigStr, swiftSourceConfig.swiftRoot);
        if (!reader) {
            AUTIL_LOG(ERROR,
                      "create swift reader for topic %s with config %s failed",
                      swiftSourceConfig.topicListStr.c_str(),
                      readerConfigStr.c_str());
            return nullptr;
        }
    }
    return reader;
}

std::unique_ptr<Source> SwiftSource::create(const std::shared_ptr<build_service::util::SwiftClientCreator> &client,
                                            const SourceConfig &config) {
    auto reader = createSwiftReader(client, config.parameters);
    if (!reader) {
        AUTIL_LOG(ERROR, "create swift reader failed");
        return {};
    }
    return std::make_unique<SwiftSource>(std::move(reader));
}

} // namespace suez
