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
#include "ha3/qrs/LayerClauseValidator.h"

#include <stdint.h>
#include <cstddef>
#include <limits>
#include <vector>

#include "alog/Logger.h"
#include "autil/Log.h"
#include "autil/StringUtil.h"
#include "autil/TimeUtility.h"
#include "ha3/common/ErrorDefine.h"
#include "ha3/common/LayerDescription.h"
#include "ha3/common/QueryLayerClause.h"
#include "ha3/isearch.h"
#include "suez/turing/expression/util/AttributeInfos.h"

using namespace std;
using namespace autil;
using namespace suez::turing;

using namespace isearch::common;

namespace isearch {
namespace qrs {
AUTIL_LOG_SETUP(ha3, LayerClauseValidator);

LayerClauseValidator::LayerClauseValidator(const AttributeInfos *attrInfos)
    : _attrInfos(attrInfos)
{
}

LayerClauseValidator::~LayerClauseValidator() { 
}

bool LayerClauseValidator::validate(QueryLayerClause* layerClause) {
    if (layerClause == NULL) {
        return true;
    }
    if (!checkRange(layerClause) 
        || !checkField(layerClause) 
        || !checkSequence(layerClause)) 
    {
        return false;
    }
    autoAddSortedRange(layerClause);
    return true;
}

bool LayerClauseValidator::checkRange(const QueryLayerClause *layerClause) {
    for (size_t i = 0; i < layerClause->getLayerCount(); ++i) {
        LayerDescription *layerDesc = layerClause->getLayerDescription(i);
        for (size_t j = 0; j < layerDesc->getLayerRangeCount(); ++j) {
            LayerKeyRange *layerRange = layerDesc->getLayerRange(j);
            if (layerRange->isRangeKeyWord()) {
                for (size_t k = 0; k < layerRange->ranges.size(); k++) {
                    LayerAttrRange attrRange = layerRange->ranges[k];
                    int64_t from = attrRange.from == "INFINITE" ? 0 :
                                   StringUtil::fromString<int32_t>(attrRange.from);
                    int64_t to = attrRange.to == "INFINITE" ? 0 :
                                 StringUtil::fromString<int32_t>(attrRange.to);
                    if (from < 0 || to < 0) {
                        _errorCode = ERROR_RANGE_LESS_THAN_ZERO;
                        _errorMsg = "layer range[" + attrRange.from + ", " + attrRange.to + "] can't less than 0";
                        AUTIL_LOG(ERROR, "layer range[%s, %s] can't less than 0",
                                attrRange.from.c_str(), attrRange.to.c_str());
                        return false;
                    }
                }
                for (size_t k = 0; k < layerRange->values.size(); k++) {
                    string valueStr = layerRange->values[k];
                    int64_t value = StringUtil::fromString<int32_t>(valueStr);
                    if (value < 0) {
                        _errorCode = ERROR_RANGE_LESS_THAN_ZERO;
                        _errorMsg = "layer value[" +  valueStr + "] can't less than 0";
                        AUTIL_LOG(ERROR, "layer value[%s] can't less than 0", 
                                valueStr.c_str());
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool LayerClauseValidator::checkField(const QueryLayerClause* layerClause) {
    for (size_t i = 0; i < layerClause->getLayerCount(); ++i) {
        LayerDescription *layerDesc = layerClause->getLayerDescription(i);
        for (size_t j = 0; j < layerDesc->getLayerRangeCount(); ++j) {
            LayerKeyRange *layerRange = layerDesc->getLayerRange(j);
            if (layerRange->isRangeKeyWord()) {
                continue;
            }
            const std::string &attrName = layerRange->attrName;
            const AttributeInfo *attrInfo = NULL;
            if (!_attrInfos || NULL == (attrInfo = _attrInfos->getAttributeInfo(attrName))) {
                _errorCode = ERROR_ATTRIBUTE_NOT_EXIST;
                _errorMsg = "attribute [" + attrName + "] not exist";
                return false;
            } else if (attrInfo != NULL && attrInfo->getSubDocAttributeFlag()) {
                _errorCode = ERROR_ATTRIBUTE_EXIST_IN_SUB_DOC;
                _errorMsg = "attribute [" + attrName + "] is exist in sub doc";
                return false;
            }
        }
    }
    return true;
}

bool LayerClauseValidator::checkSequence(const QueryLayerClause* layerClause) {
    for (size_t i = 0; i < layerClause->getLayerCount(); ++i) {
        LayerDescription *layerDesc = layerClause->getLayerDescription(i);
        size_t lastAttrPos = size_t(-1);
        for (size_t j = 0; j < layerDesc->getLayerRangeCount(); ++j) {
            LayerKeyRange *layerRange = layerDesc->getLayerRange(j);
            if (layerRange->isRangeKeyWord()) {
                continue;
            }
            if (lastAttrPos != size_t(-1) && lastAttrPos != j - 1) {
                _errorCode = ERROR_LAYER_CLAUSE_ATTRIBUTE_NOT_SEQUENCE;
                _errorMsg = "attribute is not sequence in layer clause";
                return false;
            }
            lastAttrPos = j;
        }
    }
    return true;
}

void LayerClauseValidator::autoAddSortedRange(QueryLayerClause* layerClause) {
    bool needUnsortedRange = true;
    for (size_t i = 0; i < layerClause->getLayerCount(); ++i) {
        LayerDescription *layerDesc = layerClause->getLayerDescription(i);
        bool hasSortedInLayer = false;
        for (size_t j = 0; j < layerDesc->getLayerRangeCount(); ++j) {
            LayerKeyRange *layerRange = layerDesc->getLayerRange(j);
            const std::string &attrName = layerRange->attrName;
            if (attrName == LAYERKEY_SORTED) {
                hasSortedInLayer = true;
                needUnsortedRange = false;
            } else if (attrName == LAYERKEY_UNSORTED) {
                needUnsortedRange = false;
            } else if (!layerRange->isRangeKeyWord()) {
                if (!hasSortedInLayer) {
                    LayerKeyRange *sortRange = new LayerKeyRange;
                    sortRange->attrName = LAYERKEY_SORTED;
                    layerDesc->insertKeyRange(j, sortRange);
                }
                break;
            }
        }
    }
    if (needUnsortedRange) {
        LayerDescription *layerDesc = new LayerDescription;
        LayerKeyRange *unsortRange = new LayerKeyRange;
        unsortRange->attrName = LAYERKEY_UNSORTED;
        layerDesc->addLayerRange(unsortRange);
        layerDesc->setQuota(numeric_limits<uint32_t>::max());
        layerClause->addLayerDescription(layerDesc);
    }
}

ErrorCode LayerClauseValidator::getErrorCode() const {
    return  _errorCode;
}

const string &LayerClauseValidator::getErrorMsg() const {
    return _errorMsg; 
}
} // namespace qrs
} // namespace isearch

