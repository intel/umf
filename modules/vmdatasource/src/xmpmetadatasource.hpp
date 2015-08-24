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
    void saveSchema(const vmf::MetaString& schemaName, const vmf::MetadataStream& stream);
    void loadSchema(const vmf::MetaString& schemaName, vmf::MetadataStream& stream);
    void loadProperty(const vmf::MetaString& schemaName, const vmf::MetaString& metadataName, vmf::MetadataStream& stream);
    void remove(const std::vector<vmf::IdType>& removedIds);
    void clear();
private:
    struct InternalPath {
        vmf::MetaString schema;
        vmf::MetaString metadata;
        vmf::MetaString path;
    };

    typedef std::map<vmf::IdType, InternalPath> IdMap;

    void loadPropertyByPath(const vmf::MetaString& pathToProperty, const vmf::MetaString& schemaName, vmf::MetadataStream& stream);
    void saveProperty(const vmf::MetadataSet& property, const vmf::MetaString& pathToSchema, const vmf::MetaString& propertyName);

    void loadMetadata(const vmf::MetaString& pathToCurrentMetadata, const std::shared_ptr<MetadataDesc>& description, vmf::MetadataStream& stream);
    void saveMetadata(const std::shared_ptr<vmf::Metadata>& md, const vmf::MetaString& thisPropertySetPath);

    void loadSchemaName(const vmf::MetaString& pathToSchema, vmf::MetaString& schemaName);
    void loadReference(const vmf::MetaString& thisRefPath, const std::shared_ptr<vmf::Metadata>& md, vmf::MetadataStream& stream);

    void loadField(const vmf::MetaString& fieldPath, const std::shared_ptr<vmf::Metadata>& md, const std::shared_ptr<MetadataDesc>& thisPropertyDesc);
    void saveField(const vmf::MetaString& fieldName, const vmf::Variant& value, const vmf::MetaString& fieldsPath);

    void loadIds();
    void loadIds(const vmf::MetaString& pathToSchema);

    void loadMetadataId(const vmf::MetaString& pathToMetadata, vmf::IdType& id);
    void saveMetadataId(const vmf::MetaString& pathToMetadata, const vmf::IdType& id);

    void loadMetadataFrameIndex(const vmf::MetaString& pathToMetadata, long long& frameIndex);
    void saveMetadataFrameIndex(const vmf::MetaString& pathToProperty, const long long& frameIndex);

    void loadMetadataNumOfFrames(const vmf::MetaString& pathToProperty, long long& num);
    void saveMetadataNumOfFrames(const vmf::MetaString& pathToProperty, const long long& numOfFrames);

    void loadMetadataTime(const vmf::MetaString& pathToProperty, long long& timestamp);
    void saveMetadataTime(const vmf::MetaString& pathToProperty, const long long& timestamp);

    void loadMetadataDuration(const vmf::MetaString& pathToProperty, long long& duration);
    void saveMetadataDuration(const vmf::MetaString& pathToProperty, const long long& duration);

    void loadPropertyName(const vmf::MetaString& pathToMetadata, vmf::MetaString& metadataName);
    void savePropertyName(const vmf::MetaString& pathToProperty, const MetaString &name);

    void saveMetadataFields(const vmf::MetaString& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    void saveMetadataReferences(const vmf::MetaString& pathToMetadata, const std::shared_ptr<vmf::Metadata>& md);

    MetaString appendProperty(const vmf::MetaString& pathToSchema);
    MetaString findSchema(const vmf::MetaString& name);
    MetaString findProperty(const vmf::MetaString& pathToSchema, const vmf::MetaString& name);
    MetaString findId(const InternalPath& internalPath, const vmf::IdType& id);

    XMPMetadataSource();
    XMPMetadataSource(const vmf::XMPMetadataSource& origin);
    XMPMetadataSource& operator=(const vmf::XMPMetadataSource& origin);
    std::shared_ptr<SXMPMeta> xmp;
    IdMap idMap;
};

} // namespace vmf


#endif // __XMPMETADATASOURCE_HPP__
