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
#include "indexlib/index/operation_log/OperationLogBuildWorkItem.h"

#include "indexlib/document/IDocument.h"
#include "indexlib/document/IDocumentBatch.h"

namespace indexlib::index {
AUTIL_LOG_SETUP(indexlib.index, OperationLogBuildWorkItem);

OperationLogBuildWorkItem::OperationLogBuildWorkItem(SingleOperationLogBuilder* builder,
                                                     indexlibv2::document::IDocumentBatch* documentBatch)
    : BuildWorkItem(("_OPERATION_LOG_"), BuildWorkItemType::OPERATION_LOG, documentBatch)
    , _builder(builder)

{
}

OperationLogBuildWorkItem::~OperationLogBuildWorkItem() {}

Status OperationLogBuildWorkItem::doProcess()
{
    for (size_t i = 0; i < _documentBatch->GetBatchSize(); ++i) {
        std::shared_ptr<indexlibv2::document::IDocument> iDoc = (*_documentBatch)[i];
        assert(iDoc->GetDocOperateType() != UNKNOWN_OP);

        RETURN_STATUS_DIRECTLY_IF_ERROR(_builder->ProcessDocument(iDoc.get()));
    }
    return Status::OK();
}

} // namespace indexlib::index
