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
#include <set>
#include <stdint.h>
#include <string>

#include "autil/mem_pool/Pool.h"
#include "ha3/sql/common/Log.h" // IWYU pragma: keep
#include "ha3/sql/ops/util/AsyncCallbackCtxBase.h"
#include "indexlib/document/index_document/normal_document/search_summary_document.h"
#include "indexlib/index/normal/summary/summary_reader.h"
#include "navi/engine/AsyncPipe.h"

namespace future_lite {
class Executor;
template <typename T>
class Try;
}

namespace isearch {
namespace sql {

class CountedAsyncPipe {
public:
    CountedAsyncPipe(navi::AsyncPipePtr pipe, size_t count = 1);
    CountedAsyncPipe(const CountedAsyncPipe &) = delete;
    CountedAsyncPipe &operator=(const CountedAsyncPipe &) = delete;
    ~CountedAsyncPipe() = default;
public:
    void reset();
    navi::ErrorCode done();
    navi::AsyncPipePtr getAsyncPipe();
private:
    size_t _rawCount;
    navi::AsyncPipePtr _asyncPipe;
    std::atomic<size_t> _count;
};

typedef std::shared_ptr<CountedAsyncPipe> CountedAsyncPipePtr;

struct AsyncSummaryLookupMetricsCollector {
    int64_t lookupTime = 0;
    size_t docsCount = 0;
    size_t failedDocsCount = 0;
};

class AsyncSummaryLookupCallbackCtx
    : public AsyncCallbackCtxBase,
      public std::enable_shared_from_this<AsyncSummaryLookupCallbackCtx> {
public:
    AsyncSummaryLookupCallbackCtx(CountedAsyncPipePtr pipe,
                                  indexlib::index::SummaryReaderPtr summaryReader,
                                  autil::mem_pool::PoolPtr pool,
                                  future_lite::Executor *executor);
    virtual ~AsyncSummaryLookupCallbackCtx();
    AsyncSummaryLookupCallbackCtx(const AsyncSummaryLookupCallbackCtx &) = delete;
    AsyncSummaryLookupCallbackCtx &operator=(const AsyncSummaryLookupCallbackCtx &) = delete;

public: // for callback
    void onSessionCallback(const future_lite::Try<indexlib::index::ErrorCodeVec> &errorCodeTry);

public: // for kernel && scan
    virtual void start(std::vector<docid_t> docIds, size_t fieldCount, int64_t timeout);
    virtual const indexlib::index::SearchSummaryDocVec &getResult() const;
    virtual bool hasError() const;
    std::string getErrorDesc() const;
    int64_t getSeekTime() const;
    bool tryReportMetrics(kmonitor::MetricsReporter &opMetricsReporter) const;

private:
    void prepareDocs(std::vector<docid_t> docIds, size_t fieldCount);
    void processErrorCodes(const indexlib::index::ErrorCodeVec &errorCodes);

private:
    CountedAsyncPipePtr _asyncPipe;
    indexlib::index::SummaryReaderPtr _summaryReader;
    std::vector<docid_t> _docIds;
    std::vector<indexlib::document::SearchSummaryDocument> _summaryDocDataVec;
    indexlib::index::SearchSummaryDocVec _summaryDocVec;
    std::vector<docid_t> _errorDocIds;
    autil::TimeoutTerminatorPtr _terminator;
    autil::mem_pool::PoolPtr _poolPtr;
    future_lite::Executor *_executor = nullptr;
    AsyncSummaryLookupMetricsCollector _metricsCollector;
    bool _hasError = false;
};

typedef std::shared_ptr<AsyncSummaryLookupCallbackCtx> AsyncSummaryLookupCallbackCtxPtr;

} // end namespace sql
} // end namespace isearch
