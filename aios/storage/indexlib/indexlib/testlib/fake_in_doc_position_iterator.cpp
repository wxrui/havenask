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
#include "indexlib/testlib/fake_in_doc_position_iterator.h"

using namespace std;

namespace indexlib { namespace testlib {
IE_LOG_SETUP(testlib, FakeInDocPositionIterator);

FakeInDocPositionIterator::FakeInDocPositionIterator(const std::vector<pos_t>& occArray,
                                                     const std::vector<int32_t>& fieldBitArray,
                                                     const std::vector<sectionid_t>& sectionIdArray)
    : _occArray(occArray)
    , _fieldBitArray(fieldBitArray)
    , _sectionIdArray(sectionIdArray)
{
    _offset = -1;
}

FakeInDocPositionIterator::~FakeInDocPositionIterator() {}

index::ErrorCode FakeInDocPositionIterator::SeekPositionWithErrorCode(pos_t pos, pos_t& nextpos)
{
    assert(_offset <= (int32_t)_occArray.size());
    while (++_offset < (int32_t)_occArray.size()) {
        if (_occArray[_offset] >= pos) {
            nextpos = _occArray[_offset];
            return index::ErrorCode::OK;
        }
    }
    nextpos = INVALID_POSITION;
    return index::ErrorCode::OK;
}

pospayload_t FakeInDocPositionIterator::GetPosPayload()
{
    assert(false);
    return (pospayload_t)0;
}

sectionid_t FakeInDocPositionIterator::GetSectionId() { return _sectionIdArray[_offset]; }

section_len_t FakeInDocPositionIterator::GetSectionLength()
{
    assert(false);
    return (section_len_t)0;
}

fieldid_t FakeInDocPositionIterator::GetFieldId()
{
    assert(false);
    return (int32_t)0;
    // return _fieldBitArray[_offset];
}

section_weight_t FakeInDocPositionIterator::GetSectionWeight()
{
    assert(false);
    return (section_weight_t)0;
}

int32_t FakeInDocPositionIterator::GetFieldPosition() { return _fieldBitArray[_offset]; }

bool FakeInDocPositionIterator::HasPosPayload() const
{
    assert(false);
    return true;
}
}} // namespace indexlib::testlib
