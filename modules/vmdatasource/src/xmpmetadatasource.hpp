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
#include "umf/umf.hpp"
#include <map>

namespace umf
{

class XMPMetadataSource
{
public:
    explicit XMPMetadataSource(const std::shared_ptr<SXMPMeta>& meta);
    void saveSchema(const std::shared_ptr<MetadataSchema>& schemaDesc, const umf::MetadataSet& mdSet);
    void loadSchema(const umf::umf_string& schemaName, umf::MetadataStream& stream);
    void loadProperty(const umf::umf_string& schemaName, const umf::umf_string& metadataName, umf::MetadataStream& stream);
    void remove(const std::vector<umf::IdType>& removedIds);
    void clear();
private:
    struct InternalPath {
        umf::umf_string schema;
        umf::umf_string metadata;
        umf::umf_string path;
    };

    typedef std::map<umf::IdType, InternalPath> IdMap;

    void loadPropertyByPath(const umf::umf_string& pathToProperty, const umf::umf_string& schemaName, umf::MetadataStream& stream);
    void saveProperty(const umf::MetadataSet& property, const umf::umf_string& pathToSchema, const umf::umf_string& propertyName);

    void loadMetadata(const umf::umf_string& pathToCurrentMetadata, const std::shared_ptr<MetadataDesc>& description, umf::MetadataStream& stream);
    void saveMetadata(const std::shared_ptr<umf::Metadata>& md, const umf::umf_string& thisPropertySetPath);

    void loadSchemaName(const umf::umf_string& pathToSchema, umf::umf_string& schemaName);
    void loadReference(const umf::umf_string& thisRefPath, const std::shared_ptr<umf::Metadata>& md, umf::MetadataStream& stream);

    void loadField(const umf::umf_string& fieldPath, const std::shared_ptr<umf::Metadata>& md,
                   const std::shared_ptr<MetadataDesc>& thisPropertyDesc);
    void saveField(const umf::umf_string& fieldName, const umf::Variant& value, const bool isEncrypted,
                   const umf::umf_string& encryptedData, const umf::umf_string& fieldsPath);

    void loadIds();
    void loadIds(const umf::umf_string& pathToSchema);

    void loadMetadataId(const umf::umf_string& pathToMetadata, umf::IdType& id);
    void saveMetadataId(const umf::umf_string& pathToMetadata, const umf::IdType& id);

    void loadMetadataFrameIndex(const umf::umf_string& pathToMetadata, long long& frameIndex);
    void saveMetadataFrameIndex(const umf::umf_string& pathToProperty, const long long& frameIndex);

    void loadMetadataNumOfFrames(const umf::umf_string& pathToProperty, long long& num);
    void saveMetadataNumOfFrames(const umf::umf_string& pathToProperty, const long long& numOfFrames);

    void loadMetadataTime(const umf::umf_string& pathToProperty, long long& timestamp);
    void saveMetadataTime(const umf::umf_string& pathToProperty, const long long& timestamp);

    void loadMetadataEncrypted(const umf::umf_string& pathToProperty, bool &isEncrypted,
                               umf::umf_string& encryptedData);
    void saveMetadataEncrypted(const umf::umf_string& pathToProperty, bool isEncrypted,
                               const umf::umf_string& encryptedData);

    void loadMetadataDuration(const umf::umf_string& pathToProperty, long long& duration);
    void saveMetadataDuration(const umf::umf_string& pathToProperty, const long long& duration);

    void loadPropertyName(const umf::umf_string& pathToMetadata, umf::umf_string& metadataName);
    void savePropertyName(const umf::umf_string& pathToProperty, const umf_string &name);

    void saveMetadataFields(const umf::umf_string& pathToMetadata, const std::shared_ptr<umf::Metadata>& md);

    void saveMetadataReferences(const umf::umf_string& pathToMetadata, const std::shared_ptr<umf::Metadata>& md);

    umf_string appendProperty(const umf::umf_string& pathToSchema);
    umf_string findSchema(const umf::umf_string& name);
    umf_string findProperty(const umf::umf_string& pathToSchema, const umf::umf_string& name);
    umf_string findId(const InternalPath& internalPath, const umf::IdType& id);

    XMPMetadataSource();
    XMPMetadataSource(const umf::XMPMetadataSource& origin);
    XMPMetadataSource& operator=(const umf::XMPMetadataSource& origin);
    std::shared_ptr<SXMPMeta> xmp;
    IdMap idMap;
};

} // namespace umf


#endif // __XMPMETADATASOURCE_HPP__
