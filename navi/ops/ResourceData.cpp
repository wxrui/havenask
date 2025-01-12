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
#include "navi/ops/ResourceData.h"

namespace navi {

const std::string ResourceDataType::RESOURCE_DATA_TYPE_ID = "navi@resource_data";

ResourceDataType::ResourceDataType()
    : Type(RESOURCE_DATA_TYPE_ID)
{
}

REGISTER_TYPE(ResourceDataType);

ResourceData::ResourceData()
    : Data(ResourceDataType::RESOURCE_DATA_TYPE_ID, nullptr)
    , _require(true)
{
}

bool ResourceData::isValid() const {
    return !_require || (_resource != nullptr);
}

}

