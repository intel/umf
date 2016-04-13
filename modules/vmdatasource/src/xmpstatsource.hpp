/* 
 * Copyright 2015 Intel(r) Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __XMPSTATSOURCE_HPP__
#define __XMPSTATSOURCE_HPP__

#include "xmpdatasource.hpp"

namespace vmf {

class XMPStatSource
{
public:

    explicit XMPStatSource(const std::shared_ptr<SXMPMeta> statStorage);

    void save(const std::vector< std::shared_ptr<Stat> >& stats);

    void load(std::vector< std::shared_ptr<Stat> >& stats);

    virtual void clear();

    virtual ~XMPStatSource();

private:
    XMPStatSource();
    XMPStatSource(const XMPStatSource& origin);
    XMPStatSource& operator=(const XMPStatSource& origin);

    std::shared_ptr<SXMPMeta> metadata;

};

} // namespace vmf

#endif // __XMPSTATSOURCE_HPP__
