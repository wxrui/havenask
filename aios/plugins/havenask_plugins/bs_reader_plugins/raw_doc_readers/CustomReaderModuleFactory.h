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

#include <string>
#include "build_service/reader/ReaderModuleFactory.h"
#include "build_service/util/Log.h"

namespace build_service {
namespace reader {
class RawDocumentReader;
}  // namespace reader
}  // namespace build_service

namespace pluginplatform {
namespace reader_plugins {

class CustomReaderModuleFactory : public build_service::reader::ReaderModuleFactory
{
public:
    CustomReaderModuleFactory() {}
    virtual ~CustomReaderModuleFactory() {}
private:
    CustomReaderModuleFactory(const CustomReaderModuleFactory &);
    CustomReaderModuleFactory& operator=(const CustomReaderModuleFactory &);
public:
    /*override*/
    virtual bool init(const build_service::KeyValueMap &parameters) {
        return true;
    }

    /*override*/
    virtual void destroy() {
        delete this;
    }

    /*override reader*/
    virtual build_service::reader::RawDocumentReader* createRawDocumentReader(
            const std::string &readerName);

private:
    BS_LOG_DECLARE();
};

}}
