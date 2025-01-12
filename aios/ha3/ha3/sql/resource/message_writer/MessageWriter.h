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
#pragma once

#include "autil/result/Result.h"
#include "ha3/sql/common/Log.h"
#include "suez/sdk/RemoteTableWriterParam.h"

namespace isearch {
namespace sql {

class MessageWriter
{
public:
    MessageWriter();
    virtual ~MessageWriter();
    MessageWriter(const MessageWriter &) = delete;
    MessageWriter& operator=(const MessageWriter &) = delete;
public:
    virtual void write(suez::MessageWriteParam param) = 0;
    void setWriterName(const std::string &writerName) {
        _writerName = writerName;
    }
    const std::string& getWriterName() const {
        return _writerName;
    }
private:
    std::string _writerName;
private:
    AUTIL_LOG_DECLARE();
};

typedef std::shared_ptr<MessageWriter> MessageWriterPtr;

} //end namespace sql
} //end namespace isearch
