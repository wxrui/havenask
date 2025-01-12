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

#include <memory>
#include <string>

#include "autil/Log.h" // IWYU pragma: keep
#include "aios/network/http_arpc/ProtoJsonizer.h"

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace isearch {
namespace worker {

class HaProtoJsonizer: public http_arpc::ProtoJsonizer
{
public:
    HaProtoJsonizer();
    ~HaProtoJsonizer();
private:
    HaProtoJsonizer(const HaProtoJsonizer &);
    HaProtoJsonizer& operator=(const HaProtoJsonizer &);
public:
    bool fromJson(const std::string &jsonStr, 
                  ::google::protobuf::Message *message) override;
    std::string toJson(const ::google::protobuf::Message &message) override;
private:
    AUTIL_LOG_DECLARE();
};

typedef std::shared_ptr<HaProtoJsonizer> HaProtoJsonizerPtr;

} // namespace worker
} // namespace isearch

