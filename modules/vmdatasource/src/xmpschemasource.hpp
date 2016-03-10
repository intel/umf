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
#ifndef __XMPSCHEMASOURCE_HPP__
#define __XMPSCHEMASOURCE_HPP__

#include "xmpdatasource.hpp"

namespace vmf {

class XMPSchemaSource
{
public:

    explicit XMPSchemaSource(const std::shared_ptr<SXMPMeta>& schemaStorage);

    void save(const std::shared_ptr<vmf::MetadataSchema>& schema);

    void load(std::map< vmf::vmf_string, std::shared_ptr<vmf::MetadataSchema> >& schemas);

    virtual void clear();

    virtual void remove(const vmf_string &schemaName);

    virtual ~XMPSchemaSource();

protected:

    std::shared_ptr<vmf::MetadataSchema> loadMetadataSchemaByPath(const vmf::vmf_string& pathToSchema);

    void saveDescription(const MetadataDesc& desc, const vmf_string& pathToDescriptions);

    void saveField(const FieldDesc& desc, const vmf_string& pathToFields);

    void saveReference(const std::shared_ptr<ReferenceDesc> ref, const vmf_string& pathToRefs);

    std::shared_ptr<MetadataDesc> loadDescription(const vmf_string& pathToDesc);

private:
    XMPSchemaSource();
    XMPSchemaSource(const XMPSchemaSource& origin);
    XMPSchemaSource& operator=(const XMPSchemaSource& origin);

    std::shared_ptr<SXMPMeta> metadata;

};

} // namespace vmf

#endif // __XMPSCHEMASOURCE_HPP__
