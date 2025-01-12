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
#ifndef NAVI_RESOURCEMAP_H
#define NAVI_RESOURCEMAP_H

#include "navi/common.h"
#include "autil/Lock.h"
#include <unordered_map>

namespace navi {

class Resource;

class ResourceMap
{
public:
    ResourceMap();
    ~ResourceMap();
public:
    // thread safe
    bool addResource(const std::shared_ptr<Resource> &resource);
    bool addResource(const ResourceMap &resourceMap);
    std::shared_ptr<Resource> getResource(const std::string &name) const;
    template <typename T>
    T *getResource(const std::string &name) const;
    bool hasResource(const std::string &name) const;
    void clear();
public:
    // not thread safe
    const std::unordered_map<std::string, std::shared_ptr<Resource>> &getMap() const;
public:
    friend std::ostream& operator<<(std::ostream& os, const ResourceMap &resourceMap);
private:
    mutable autil::ReadWriteLock _lock;
    std::unordered_map<std::string, std::shared_ptr<Resource>> _map;
};

template <typename T>
T *ResourceMap::getResource(const std::string &name) const {
    return dynamic_cast<T *>(getResource(name).get());
}

NAVI_TYPEDEF_PTR(ResourceMap);

class MultiLayerResourceMap {
public:
    MultiLayerResourceMap() {
    }
public:
    void addResourceMap(const ResourceMap *resourceMap);
    std::shared_ptr<Resource> getResource(const std::string &name) const;
    bool hasResource(const std::string &name) const;
public:
    friend std::ostream& operator<<(std::ostream& os,
                                    const MultiLayerResourceMap &multiLayerResourceMap);
private:
    std::vector<const ResourceMap *> _resourceMapVec;
};
}

#endif //NAVI_RESOURCEMAP_H
