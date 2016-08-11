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
    void loadSchema(const vmf::umf_string& schemaName, vmf::MetadataStream& stream);
    void loadProperty(const vmf::umf_string& schemaName, const vmf::umf_string& metadataName, vmf::MetadataStream& stream);
    void remove(const std::vector<vmf::IdType>& removedIds);
    void clear();
private:
    struct InternalPath {
        vmf::umf_string schema;
        vmf::umf_string metadata;
        vmf::umf_string path;
    };

    typedef std::map<vmf::IdType, InternalPath> IdMap;

    void loadPropertyByPath(const vmf::umf_string& pathToProperty, const vmf::umf_string& schemaName, vmf::MetadataStream& stream);
    void saveProperty(const vmf::MetadataSet& property, const vmf::umf_string& pathToSchema, const vmf::umf_string& propertyName);

    void loadMetadata(const vmf::umf_string& pathToCurrentMetadata, const std::shared_ptr<MetadataDesc>& description, vmf::MetadataStream& stream);
    void saveMetadata(const std::shared_ptr<vmf::Metadata>& md, const vmf::umf_string& thisPropertySetPath);

    void loadSchemaName(const vmf::umf_string& pathToSchema, vmf::umf_string& schemaName);
    void loadReference(const vmf::umf_string& thisRefPath, const std::shared_ptr<vmf::Metadata>& md, vmf::MetadataStream& stream);

    void loadField(const vmf::umf_string& fieldPath, const std::shared_ptr<vmf::Metadata>& md,
                   const std::shared_ptr<MetadataDesc>& thisPropertyDesc);
    void saveField(const vmf::umf_string& fieldName, const vmf::Variant& value, const bool isEncrypted,
                   const vmf::umf_string& encryptedData, const vmf::umf_string& fieldsPath);

    void loadIds();
    void loadIds(const vmf::umf_string& pathToSchema);

    void loadMetadataId(const vmf::umf_string& pathToMetadata, vmf::IdType& id);
    void saveMetadataId(const vmf::umf_string& pathToMetadata, const vmf::IdType& id);

    void loadMetadataFrameIndex(const vmf::umf_string& pathToMetadata, long long& frameIndex);
    void saveMetadataFrameIndex(const vmf::umf_string& pathToProperty, const long long& frameIndex);

    void loadMetadataNumOfFrames(const vmf::umf_string& pathToProperty, long long& num);
    void saveMetadataNumOfFrames(const vmf::umf_string& pathToProperty, const long long& numOfFrames);

    void loadMetadataTime(const vmf::umf_string& pathToProperty, long long& timestamp);
    void saveMetadataTime(const vmf::umf_string& pathToProperty, const long long& timestamp);

    void loadMetadataEncrypted(const vmf::umf_string& pathToProperty, bool &isEncrypted,
                               vmf::umf_string& encryptedData);
    void saveMetadataEncrypted(const vmf::umf_string& pathToProperty, bool isEncrypted,
                               const vmf::umf_string& encryptedData);

    void loadMetadataDuration(const vmf::umf_string& pathToProperty, long long& duration);
    void saveMetadataDuration(const vmf::umf_string& pathToProperty, const long long& duration);

    void loadPropertyName(const vmf::umf_string& pathToMetadata, vmf::umf_string& metadataName);
    void savePropertyName(const vmf::umf_string& pathToProperty, const umf_string &name);

    void saveMetadataFields(const vmf::umf_string& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    void saveMetadataReferences(const vmf::umf_string& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    umf_string appendProperty(const vmf::umf_string& pathToSchema);
    umf_string findSchema(const vmf::umf_string& name);
    umf_string findProperty(const vmf::umf_string& pathToSchema, const vmf::umf_string& name);
    umf_string findId(const InternalPath& internalPath, const vmf::IdType& id);

    XMPMetadataSource();
    XMPMetadataSource(const vmf::XMPMetadataSource& origin);
    XMPMetadataSource& operator=(const vmf::XMPMetadataSource& origin);
    std::shared_ptr<SXMPMeta> xmp;
    IdMap idMap;
};

} // namespace vmf


#endif // __XMPMETADATASOURCE_HPP__
