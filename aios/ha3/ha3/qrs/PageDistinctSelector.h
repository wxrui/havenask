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

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <algorithm>
#include <deque>
#include <limits>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <tr1/unordered_map>
#include <vector>

#include "autil/Log.h" // IWYU pragma: keep
#include "ha3/common/Hit.h"
#include "ha3/qrs/PageDistinctHitConvertor.h"

namespace isearch {
namespace qrs {

typedef std::vector<common::HitPtr> HitVector;
typedef std::set<size_t> PosSet;
typedef std::list<size_t> PosList;

class PageDistinctSelector
{
public:
    PageDistinctSelector(uint32_t pgHitNum, const std::string &distKey,
                         uint32_t distCount,
                         const std::vector<bool> &sortFlags,
                         const std::vector<double> &gradeThresholds = std::vector<double>());

    virtual ~PageDistinctSelector();
public:
    virtual void selectPages(const HitVector &hits , uint32_t startOffSet,
                     uint32_t hitCount, HitVector &pageHits) = 0;

protected:
    uint32_t _pgHitNum;
    std::string _distKey;
    uint32_t _distCount;
    std::vector<double> _grades;
    std::vector<bool> _sortFlags;
private:
    friend class PageDistinctSelectorTest;
    friend class PageDistinctProcessorTest;
private:
    AUTIL_LOG_DECLARE();
};

typedef std::shared_ptr<PageDistinctSelector> PageDistinctSelectorPtr;

template <typename KeyType>
class PageDistinctSelectorTyped : public PageDistinctSelector
{
public:
    PageDistinctSelectorTyped(uint32_t pgHitNum,
                              const std::string &distKey,
                              uint32_t distCount,
                              const std::vector<bool> &sortFlags,
                              const std::vector<double> &gradeThresholds =
                              std::vector<double>()) :
        PageDistinctSelector(pgHitNum, distKey, distCount,
                             sortFlags, gradeThresholds)
    {
    }
    ~PageDistinctSelectorTyped() {
    }
private:
    PageDistinctSelectorTyped(const PageDistinctSelectorTyped &);
    PageDistinctSelectorTyped& operator=(const PageDistinctSelectorTyped &);
public:
    virtual void selectPages(const HitVector &hits , uint32_t startOffSet,
                             uint32_t hitCount, HitVector &pageHits);

protected:
    void selectPages(const std::vector<PageDistinctHit<KeyType> *> &hits,
                     uint32_t pageCount,
                     std::vector<size_t> &pageHitPoses) const;
    inline void collectPage(std::vector<size_t> &pageHitPoses,
                            std::vector<size_t> &page) const;
    inline void collectPageFromBakList(std::vector<size_t> &page,
            std::deque<size_t> &bakList) const;
    friend class PageDistinctSelectorTest;
private:
    AUTIL_LOG_DECLARE();
};

AUTIL_LOG_SETUP_TEMPLATE(ha3, PageDistinctSelectorTyped, KeyType);

template <typename KeyType>
void PageDistinctSelectorTyped<KeyType>::selectPages(
        const HitVector &hits, uint32_t startOffSet,
        uint32_t hitCount, HitVector &pageHits)
{
    pageHits.clear();
    if (_pgHitNum == 0 || startOffSet >= hits.size() ||
        (startOffSet + hitCount == 0))
    {
        return;
    }

    PageDistinctHitConvertor<KeyType> convertor(_distKey, _sortFlags, _grades);

    std::vector<PageDistinctHit<KeyType>*> pgDistHits;
    convertor.convertAndSort(hits, pgDistHits);

    uint32_t pageCount = (startOffSet + hitCount + _pgHitNum - 1) / _pgHitNum;

    std::vector<size_t> tmpPageHits;
    selectPages(pgDistHits, pageCount, tmpPageHits);
    uint32_t lastHitPos = std::min(startOffSet + hitCount,
                                   (uint32_t)tmpPageHits.size());
    assert(lastHitPos >= startOffSet);
    pageHits.reserve(lastHitPos - startOffSet);
    for (uint32_t i = startOffSet; i < lastHitPos; ++i)
    {
        pageHits.push_back(hits[pgDistHits[tmpPageHits[i]]->pos]);
    }
}

template <typename KeyType>
inline void PageDistinctSelectorTyped<KeyType>::collectPage(
        std::vector<size_t> &pageHitPoses,
        std::vector<size_t> &page) const
{
    std::sort(page.begin(), page.end());
    pageHitPoses.insert(pageHitPoses.end(), page.begin(), page.end());
    page.clear();
}

template <typename KeyType>
inline void PageDistinctSelectorTyped<KeyType>::collectPageFromBakList(
        std::vector<size_t> &page,
        std::deque<size_t> &bakList) const
{
    while (page.size() < _pgHitNum && !bakList.empty()) {
        page.push_back(bakList.front());
        bakList.pop_front();
    }
}

template <typename KeyType>
void PageDistinctSelectorTyped<KeyType>::selectPages(
        const  std::vector<PageDistinctHit<KeyType> *> &hits,
        uint32_t pageCount, std::vector<size_t> &pageHitPoses) const
{
    size_t hitCount = hits.size();
    if (hitCount == 0 || _pgHitNum == 0 || pageCount == 0) {
        return;
    }

    pageHitPoses.reserve(pageCount * _pgHitNum);
    std::vector<size_t> page;
    page.reserve(_pgHitNum);

    size_t hitPos = 0;
    int32_t preGrade = std::numeric_limits<int32_t>::min();
    uint32_t curPageCount = 0;
    uint32_t leftHitNum = 0;
    std::deque<size_t> backHitPosList;

    std::unordered_map<KeyType, uint32_t> distMap;
    while (true) {
        if (page.size() == _pgHitNum) {
            distMap.clear();
            collectPage(pageHitPoses, page);
            leftHitNum = backHitPosList.size();
            curPageCount ++;
            if (curPageCount >= pageCount) {
                break;
            }
        }

        size_t pos = hitPos;
        if (leftHitNum > 0) {
            pos = backHitPosList.front();
            backHitPosList.pop_front();
            leftHitNum --;
        } else if (pos < hitCount) {
            pos = hitPos;
        } else if (!backHitPosList.empty()) {
            collectPageFromBakList(page, backHitPosList);
            if (page.size() >= _pgHitNum) {
                continue;
            } else {
                break;
            }
        } else {
            break;
        }

        PageDistinctHit<KeyType> *hit = hits[pos];
        int32_t curGrade = hit->gradeValue;
        if (curGrade < preGrade) {
            collectPageFromBakList(page, backHitPosList);
            if (page.size() >= _pgHitNum) {
                continue;
            }
        }
        preGrade = curGrade;

        if (hit->hasKeyValue) {
            uint32_t &distCount = distMap[hit->keyValue];
            if (distCount < _distCount) {
                distCount ++;
                page.push_back(pos);
            } else {
                backHitPosList.push_back(pos);
            }
        } else {
            page.push_back(pos);
        }
        if (pos == hitPos) {
            hitPos ++;
        }
    }

    collectPage(pageHitPoses, page);
}

} // namespace qrs
} // namespace isearch
