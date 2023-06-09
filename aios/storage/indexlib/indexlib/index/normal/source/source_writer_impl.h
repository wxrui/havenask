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
#ifndef __INDEXLIB_SOURCE_WRITER_IMPL_H
#define __INDEXLIB_SOURCE_WRITER_IMPL_H

#include <memory>

#include "indexlib/common_define.h"
#include "indexlib/index/normal/source/in_mem_source_segment_reader.h"
#include "indexlib/index/normal/source/source_group_writer.h"
#include "indexlib/index/normal/source/source_writer.h"
#include "indexlib/indexlib.h"

namespace indexlib { namespace index {

class SourceWriterImpl : public SourceWriter
{
public:
    SourceWriterImpl();
    ~SourceWriterImpl();

public:
    void Init(const config::SourceSchemaPtr& sourceSchema, util::BuildResourceMetrics* buildResourceMetrics) override;

    // If any validation needs to be done, add a preprocess check instead of returning false here.
    void AddDocument(const document::SerializedSourceDocumentPtr& document) override;

    void Dump(const file_system::DirectoryPtr& dir, autil::mem_pool::PoolBase* dumpPool) override;
    const InMemSourceSegmentReaderPtr CreateInMemSegmentReader() override;

private:
    config::SourceSchemaPtr mSourceSchema;
    GroupFieldDataWriterPtr mMetaWriter;
    std::vector<SourceGroupWriterPtr> mDataWriters;

private:
    IE_LOG_DECLARE();
};

DEFINE_SHARED_PTR(SourceWriterImpl);
}} // namespace indexlib::index

#endif //__INDEXLIB_SOURCE_WRITER_IMPL_H