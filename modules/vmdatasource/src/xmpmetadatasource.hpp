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

#ifndef __XMPMETADATASOURCE_HPP__
#define __XMPMETADATASOURCE_HPP__

#include "xmpdatasource.hpp"
#include "vmf/vmf.hpp"
#include <map>

namespace vmf
{

class XMPMetadataSource
{
public:
    explicit XMPMetadataSource(const std::shared_ptr<SXMPMeta>& meta);
    void saveSchema(const std::shared_ptr<MetadataSchema>& schemaDesc, const vmf::MetadataSet& mdSet);
    void loadSchema(const vmf::vmf_string& schemaName, vmf::MetadataStream& stream);
    void loadProperty(const vmf::vmf_string& schemaName, const vmf::vmf_string& metadataName, vmf::MetadataStream& stream);
    void remove(const std::vector<vmf::IdType>& removedIds);
    void clear();
private:
    struct InternalPath {
        vmf::vmf_string schema;
        vmf::vmf_string metadata;
        vmf::vmf_string path;
    };

    typedef std::map<vmf::IdType, InternalPath> IdMap;

    void loadPropertyByPath(const vmf::vmf_string& pathToProperty, const vmf::vmf_string& schemaName, vmf::MetadataStream& stream);
    void saveProperty(const vmf::MetadataSet& property, const vmf::vmf_string& pathToSchema, const vmf::vmf_string& propertyName);

    void loadMetadata(const vmf::vmf_string& pathToCurrentMetadata, const std::shared_ptr<MetadataDesc>& description, vmf::MetadataStream& stream);
    void saveMetadata(const std::shared_ptr<vmf::Metadata>& md, const vmf::vmf_string& thisPropertySetPath);

    void loadSchemaName(const vmf::vmf_string& pathToSchema, vmf::vmf_string& schemaName);
    void loadReference(const vmf::vmf_string& thisRefPath, const std::shared_ptr<vmf::Metadata>& md, vmf::MetadataStream& stream);

    void loadField(const vmf::vmf_string& fieldPath, const std::shared_ptr<vmf::Metadata>& md,
                   const std::shared_ptr<MetadataDesc>& thisPropertyDesc);
    void saveField(const vmf::vmf_string& fieldName, const vmf::Variant& value, const bool isEncrypted,
                   const vmf::vmf_string& encryptedData, const vmf::vmf_string& fieldsPath);

    void loadIds();
    void loadIds(const vmf::vmf_string& pathToSchema);

    void loadMetadataId(const vmf::vmf_string& pathToMetadata, vmf::IdType& id);
    void saveMetadataId(const vmf::vmf_string& pathToMetadata, const vmf::IdType& id);

    void loadMetadataFrameIndex(const vmf::vmf_string& pathToMetadata, long long& frameIndex);
    void saveMetadataFrameIndex(const vmf::vmf_string& pathToProperty, const long long& frameIndex);

    void loadMetadataNumOfFrames(const vmf::vmf_string& pathToProperty, long long& num);
    void saveMetadataNumOfFrames(const vmf::vmf_string& pathToProperty, const long long& numOfFrames);

    void loadMetadataTime(const vmf::vmf_string& pathToProperty, long long& timestamp);
    void saveMetadataTime(const vmf::vmf_string& pathToProperty, const long long& timestamp);

    void loadMetadataEncrypted(const vmf::vmf_string& pathToProperty, bool &isEncrypted,
                               vmf::vmf_string& encryptedData);
    void saveMetadataEncrypted(const vmf::vmf_string& pathToProperty, bool isEncrypted,
                               const vmf::vmf_string& encryptedData);

    void loadMetadataDuration(const vmf::vmf_string& pathToProperty, long long& duration);
    void saveMetadataDuration(const vmf::vmf_string& pathToProperty, const long long& duration);

    void loadPropertyName(const vmf::vmf_string& pathToMetadata, vmf::vmf_string& metadataName);
    void savePropertyName(const vmf::vmf_string& pathToProperty, const vmf_string &name);

    void saveMetadataFields(const vmf::vmf_string& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    void saveMetadataReferences(const vmf::vmf_string& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    vmf_string appendProperty(const vmf::vmf_string& pathToSchema);
    vmf_string findSchema(const vmf::vmf_string& name);
    vmf_string findProperty(const vmf::vmf_string& pathToSchema, const vmf::vmf_string& name);
    vmf_string findId(const InternalPath& internalPath, const vmf::IdType& id);

    XMPMetadataSource();
    XMPMetadataSource(const vmf::XMPMetadataSource& origin);
    XMPMetadataSource& operator=(const vmf::XMPMetadataSource& origin);
    std::shared_ptr<SXMPMeta> xmp;
    IdMap idMap;
};

} // namespace vmf


#endif // __XMPMETADATASOURCE_HPP__
