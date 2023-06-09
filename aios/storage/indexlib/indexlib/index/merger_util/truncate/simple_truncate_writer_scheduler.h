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
#ifndef __INDEXLIB_SIMPLE_TRUNCATE_WRITER_SCHEDULER_H
#define __INDEXLIB_SIMPLE_TRUNCATE_WRITER_SCHEDULER_H

#include <memory>

#include "indexlib/common_define.h"
#include "indexlib/index/merger_util/truncate/truncate_writer_scheduler.h"
#include "indexlib/indexlib.h"

namespace indexlib::index::legacy {

class SimpleTruncateWriterScheduler : public TruncateWriterScheduler
{
public:
    SimpleTruncateWriterScheduler();
    ~SimpleTruncateWriterScheduler();

public:
    void PushWorkItem(autil::WorkItem* workItem) override;
    void WaitFinished() override;

private:
    IE_LOG_DECLARE();
};

DEFINE_SHARED_PTR(SimpleTruncateWriterScheduler);
} // namespace indexlib::index::legacy

#endif //__INDEXLIB_SIMPLE_TRUNCATE_WRITER_SCHEDULER_H