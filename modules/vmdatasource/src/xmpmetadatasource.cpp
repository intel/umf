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
#include "xmpmetadatasource.hpp"

#include "vmf/metadatastream.hpp"

#define VMF_GLOBAL_SCHEMAS_ARRAY "metadata"

#define SCHEMA_NAME "schema"
#define SCHEMA_SET "set"

#define PROPERTY_NAME "name"
#define PROPERTY_SET "set"

#define METADATA_ID "id"
#define METADATA_FIELDS "fields"
#define METADATA_FRAME_INDEX "index"
#define METADATA_NUM_OF_FRAMES "nframes"
#define METADATA_TIMESTAMP "timestamp"
#define METADATA_DURATION "duration"
#define METADATA_REFERENCES "references"
#define METADATA_ENCRYPTED_BOOL "is-encrypted"
#define METADATA_ENCRYPTED_DATA "encrypted-data"

#define FIELD_TYPE "type"
#define FIELD_VALUE "value"
#define FIELD_NAME "name"
#define FIELD_ENCRYPTED_BOOL "is-encrypted"
#define FIELD_ENCRYPTED_DATA "encrypted-data"

#define REF_NAME "name"
#define REF_SCHEMA "schema"
#define REF_PROPERTY "descriptor"
#define REF_ID "id"

using namespace std;
using namespace vmf;

class MetadataAccessor: public Metadata
{
public:
    MetadataAccessor( const std::shared_ptr< MetadataDesc >& spDescription )
      : Metadata(spDescription) { }
    MetadataAccessor( const Metadata& oMetadata )
      : Metadata(oMetadata) { }
    using Metadata::setId;
    virtual ~MetadataAccessor() {}
};

class MetadataStreamAccessor: public MetadataStream
{
public:
    MetadataStreamAccessor()
      : MetadataStream() { }
    virtual ~MetadataStreamAccessor() { }
    using MetadataStream::internalAdd;
};

XMPMetadataSource::XMPMetadataSource(const std::shared_ptr<SXMPMeta>& meta)
  : xmp(meta)
{
    loadIds();
}

void XMPMetadataSource::saveSchema(const std::shared_ptr<MetadataSchema>& schemaDesc, const MetadataSet& mdSet)
{
    shared_ptr<MetadataSchema> thisSchemaDescription = schemaDesc;
    vmf_string schemaName = schemaDesc->getName();
    vmf_string thisSchemaPath = findSchema(schemaName);

    if (thisSchemaPath.empty())
    {
        xmp->AppendArrayItem(VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_ArrayLastItem, &thisSchemaPath);
        xmp->SetStructField(VMF_NS, thisSchemaPath.c_str(), VMF_NS, SCHEMA_NAME, schemaName);
        xmp->SetStructField(VMF_NS, thisSchemaPath.c_str(), VMF_NS, SCHEMA_SET, nullptr, kXMP_PropValueIsArray);
    }

    MetadataSet thisSchemaSet = mdSet.queryBySchema(schemaName);
    vector< shared_ptr<MetadataDesc> > thisSchemaProperties = thisSchemaDescription->getAll();
    for(auto descIter = thisSchemaProperties.begin(); descIter != thisSchemaProperties.end(); ++descIter)
    {
        vmf_string metadataName = (*descIter)->getMetadataName();
        MetadataSet currentPropertySet(thisSchemaSet.queryByName(metadataName));
        saveProperty(currentPropertySet, thisSchemaPath, metadataName);
    }
}

void XMPMetadataSource::saveProperty(const MetadataSet& property, const vmf_string& pathToSchema, const vmf_string& propertyName)
{
    if (property.empty())
    {
        // not loaded or empty. In any case nothing to do.
        return;
    }

    vmf_string pathToPropertiesArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToPropertiesArray);

    vmf_string thisPropertyPath = findProperty(pathToSchema, propertyName);
    if (thisPropertyPath.empty())
    {
        xmp->AppendArrayItem(VMF_NS, pathToPropertiesArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToPropertiesArray.c_str(), kXMP_ArrayLastItem, &thisPropertyPath);
    }

    savePropertyName(thisPropertyPath, propertyName);
    xmp->SetStructField(VMF_NS, thisPropertyPath.c_str(), VMF_NS, PROPERTY_SET, nullptr, kXMP_PropValueIsArray);
    vmf_string thisPropertySetPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, thisPropertyPath.c_str(), VMF_NS, PROPERTY_SET, &thisPropertySetPath);

    for(auto metadata = property.begin(); metadata != property.end(); ++metadata)
    {
        saveMetadata(*metadata, thisPropertySetPath);
    }
}


void XMPMetadataSource::saveMetadata(const shared_ptr<Metadata>& md, const vmf_string& thisPropertySetPath)
{
    if (md == nullptr)
        VMF_EXCEPTION(DataStorageException, "Trying to save nullptr metadata");

    vmf_string pathToMetadata;

    auto it = idMap.find(md->getId());
    if (it == idMap.end())
    {
        xmp->AppendArrayItem(VMF_NS, thisPropertySetPath.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, thisPropertySetPath.c_str(), kXMP_ArrayLastItem, &pathToMetadata);
    }
    else
    {
        pathToMetadata = it->second.path;
    }

    saveMetadataId(pathToMetadata, md->getId());
    saveMetadataFrameIndex(pathToMetadata, md->getFrameIndex());
    saveMetadataNumOfFrames(pathToMetadata, md->getNumOfFrames());
    saveMetadataTime(pathToMetadata, md->getTime());
    saveMetadataDuration(pathToMetadata, md->getDuration());
    saveMetadataFields(pathToMetadata, md);
    saveMetadataReferences(pathToMetadata, md);
    saveMetadataEncrypted(pathToMetadata, md->getUseEncryption(), md->getEncryptedData());
}


void XMPMetadataSource::saveField(const vmf_string& fieldName, const Variant& _value, const bool isEncrypted,
                                  const vmf_string &encryptedData, const vmf_string& fieldsPath)
{
    std::string value = _value.toString();
    if(value.empty())
        value = " ";
    xmp->AppendArrayItem(VMF_NS, fieldsPath.c_str(), kXMP_PropValueIsArray, value, kXMP_NoOptions);

    if (!fieldName.empty())
    {
        vmf_string thisFieldPath;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, fieldsPath.c_str(), kXMP_ArrayLastItem, &thisFieldPath);
        xmp->SetQualifier(VMF_NS, thisFieldPath.c_str(), VMF_NS, FIELD_NAME, fieldName.c_str(), kXMP_NoOptions);
    }
    vmf_string encBoolPath;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, fieldsPath.c_str(), kXMP_ArrayLastItem, &encBoolPath);
    if(isEncrypted)
    {
        xmp->SetQualifier(VMF_NS, encBoolPath.c_str(), VMF_NS, FIELD_ENCRYPTED_BOOL, "true", kXMP_NoOptions);
    }
    else
    {
        vmf_string tmpString;
        if(xmp->GetQualifier(VMF_NS, fieldsPath.c_str(), VMF_NS, FIELD_ENCRYPTED_BOOL, &tmpString, NULL))
        {
            xmp->DeleteQualifier(VMF_NS, fieldsPath.c_str(), VMF_NS, FIELD_ENCRYPTED_BOOL);
        }
    }
    vmf_string encDataPath;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, fieldsPath.c_str(), kXMP_ArrayLastItem, &encDataPath);
    if(!encryptedData.empty())
    {
        xmp->SetQualifier(VMF_NS, encDataPath.c_str(), VMF_NS, FIELD_ENCRYPTED_DATA,
                          encryptedData.c_str(), kXMP_NoOptions);
    }
    else
    {
        vmf_string tmpString;
        if(xmp->GetQualifier(VMF_NS, fieldsPath.c_str(), VMF_NS, FIELD_ENCRYPTED_DATA, &tmpString, NULL))
        {
            xmp->DeleteQualifier(VMF_NS, fieldsPath.c_str(), VMF_NS, FIELD_ENCRYPTED_DATA);
        }
    }
}


void XMPMetadataSource::loadSchema(const vmf_string &schemaName, MetadataStream &stream)
{
    vmf_string schemaPath = findSchema(schemaName);
    if (schemaPath.empty())
    {
        VMF_EXCEPTION(DataStorageException, "Schema " + schemaName + " not found");
    }

    vmf_string pathToProperties;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_SET, &pathToProperties);
    SXMPIterator propIterator(*xmp, VMF_NS, pathToProperties.c_str(), kXMP_IterJustChildren);
    vmf_string currentPropertyPath;
    while(propIterator.Next(NULL, &currentPropertyPath))
    {
        loadPropertyByPath(currentPropertyPath, schemaName, stream);
    }
}

void XMPMetadataSource::loadProperty(const vmf_string& schemaName, const vmf_string& metadataName, MetadataStream& stream)
{
    vmf_string pathToSchema = findSchema(schemaName);
    vmf_string pathToProperty = findProperty(pathToSchema, metadataName);
    loadPropertyByPath(pathToProperty, schemaName, stream);
}

void XMPMetadataSource::loadSchemaName(const vmf_string &pathToSchema, vmf_string& schemaName)
{
    if (!xmp->GetStructField(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_NAME, &schemaName, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Broken schema by path " + pathToSchema);
    }
}

void XMPMetadataSource::loadPropertyByPath(const vmf_string& pathToProperty, const vmf_string& schemaName, MetadataStream& stream)
{
    shared_ptr<MetadataSchema> schema(stream.getSchema(schemaName));
    vmf_string metadataName;
    loadPropertyName(pathToProperty, metadataName);
    vmf_string pathToMetadataSet;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, PROPERTY_SET, &pathToMetadataSet);

    shared_ptr<MetadataDesc> description(schema->findMetadataDesc(metadataName));

    SXMPIterator mIter(*xmp, VMF_NS, pathToMetadataSet.c_str(), kXMP_IterJustChildren);
    vmf_string pathToCurrentMetadata;
    while(mIter.Next(nullptr, &pathToCurrentMetadata))
    {
        loadMetadata(pathToCurrentMetadata, description, stream);
    }
    //unsorted stream fails on save
    stream.sortById();
}

void XMPMetadataSource::loadMetadata(const vmf_string& pathToCurrentMetadata, const shared_ptr<MetadataDesc>& description, MetadataStream& stream)
{
    IdType id;
    loadMetadataId(pathToCurrentMetadata, id);

    shared_ptr<Metadata> thisMetadata = stream.getById(id);
    if (thisMetadata)
    {
        // already loaded
        return;
    }

    long long frameIndex;
    loadMetadataFrameIndex(pathToCurrentMetadata, frameIndex);

    long long numOfFrames;
    loadMetadataNumOfFrames(pathToCurrentMetadata, numOfFrames);

    long long timestamp;
    loadMetadataTime(pathToCurrentMetadata, timestamp);

    long long duration;
    loadMetadataDuration(pathToCurrentMetadata, duration);

    bool isEncrypted;
    vmf_string encryptedData;
    loadMetadataEncrypted(pathToCurrentMetadata, isEncrypted, encryptedData);

    shared_ptr<MetadataAccessor> metadataAccessor(new MetadataAccessor(description));

    metadataAccessor->setFrameIndex(frameIndex, numOfFrames);
    metadataAccessor->setTimestamp(timestamp, duration);
    metadataAccessor->setId(id);
    //since we've already checked the flag isEncrypted and encrypted data presence
    //that's why we set them the following way
    if(encryptedData.length() > 0)
    {
        metadataAccessor->setUseEncryption(isEncrypted);
        metadataAccessor->setEncryptedData(encryptedData);
    }
    else
    {
        metadataAccessor->setUseEncryption(false);
    }
    vmf_string fieldsPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentMetadata.c_str(), VMF_NS, METADATA_FIELDS, &fieldsPath);

    shared_ptr<MetadataSchema> thisSchemaDesc = stream.getSchema(description->getSchemaName());
    shared_ptr<MetadataDesc> thisPropertyDesc = thisSchemaDesc->findMetadataDesc(description->getMetadataName());
    SXMPIterator fieldsIterator(*xmp, VMF_NS, fieldsPath.c_str(), kXMP_IterJustChildren);
    vmf_string currentFieldPath;
    while(fieldsIterator.Next(NULL, &currentFieldPath))
    {
        loadField(currentFieldPath, metadataAccessor, thisPropertyDesc);
    }

    MetadataStreamAccessor* streamAccessor = (MetadataStreamAccessor*) &stream;
    streamAccessor->internalAdd(metadataAccessor);

    // Load refs only after adding to steam to stop recursive loading when there are circular references
    vmf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentMetadata.c_str(), VMF_NS, METADATA_REFERENCES, &pathToRefs);
    SXMPIterator refsIterator(*xmp, VMF_NS, pathToRefs.c_str(), kXMP_IterJustChildren);
    vmf_string currentRefPath;
    while(refsIterator.Next(NULL, &currentRefPath))
    {
        loadReference(currentRefPath, metadataAccessor, stream);
    }

}

void XMPMetadataSource::loadPropertyName(const vmf_string& pathToMetadata, vmf_string& metadataName)
{
    if(!xmp->GetStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, PROPERTY_NAME, &metadataName, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted property by path " + pathToMetadata);
    }
}

void XMPMetadataSource::loadField(const vmf_string& fieldPath, const shared_ptr<Metadata>& md, const shared_ptr<MetadataDesc>& thisPropertyDesc)
{
    vmf_string rawValue;
    if (!xmp->GetProperty(VMF_NS, fieldPath.c_str(), &rawValue, NULL))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted field by path " + fieldPath);
    }

    vmf_string fieldName;
    if (!xmp->GetQualifier(VMF_NS, fieldPath.c_str(), VMF_NS, FIELD_NAME, &fieldName, NULL))
    {
        fieldName = "";
    }

    FieldDesc thisFieldDesc;
    if (!thisPropertyDesc->getFieldDesc(thisFieldDesc, fieldName))
    {
        VMF_EXCEPTION(DataStorageException, "Extra field by path " + fieldPath);
    }

    bool isEncrypted;
    vmf_string encBool;
    xmp->GetQualifier(VMF_NS, fieldPath.c_str(), VMF_NS, FIELD_ENCRYPTED_BOOL, &encBool, NULL);
    isEncrypted = (encBool=="true") ;

    vmf_string encData;
    xmp->GetQualifier(VMF_NS, fieldPath.c_str(), VMF_NS, FIELD_ENCRYPTED_DATA, &encData, NULL);
    if(encData.empty() && isEncrypted)
    {
        VMF_EXCEPTION(DataStorageException, "No encrypted data provided in field");
    }

    Variant fieldValue;
    fieldValue.fromString(thisFieldDesc.type, rawValue);
    if (fieldName.empty())
    {
        md->addValue(fieldValue);
    }
    else
    {
        md->setFieldValue(fieldName, fieldValue);
    }

    FieldValue& fv = *md->findField(fieldName);
    fv.setUseEncryption(isEncrypted);
    fv.setEncryptedData(encData);
}

void XMPMetadataSource::loadReference(const vmf_string& thisRefPath, const shared_ptr<Metadata>& md, MetadataStream& stream)
{
    XMP_Int64 id;
    vmf_string tmpPath;
    vmf_string refName;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, thisRefPath.c_str(), VMF_NS, REF_NAME, &tmpPath);
    if (!xmp->GetProperty(VMF_NS, tmpPath.c_str(), &refName, nullptr))
        refName = "";
    
    SXMPUtils::ComposeStructFieldPath(VMF_NS, thisRefPath.c_str(), VMF_NS, REF_ID, &tmpPath);
    if (!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &id, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Broken reference by path" + thisRefPath);
    }

    IdType realId = id;
    shared_ptr<Metadata> refTo = stream.getById(realId);
    if (!refTo)
    {
        auto it = idMap.find(realId);
        if (it == idMap.end())
        {
            VMF_EXCEPTION(DataStorageException, "Undefined reference by path " + thisRefPath);
        }
        InternalPath path = it->second;
        std::shared_ptr<MetadataSchema> refSchemaDesc = stream.getSchema(path.schema);
        std::shared_ptr<MetadataDesc> refMetadataDesc = refSchemaDesc->findMetadataDesc(path.metadata);
        loadMetadata(path.path, refMetadataDesc, stream);
        refTo = stream.getById(realId);
    }
    md->addReference(refTo, refName);
}

vmf_string XMPMetadataSource::findSchema(const vmf_string& name)
{
    SXMPIterator it(*xmp, VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_IterJustChildren);
    vmf_string currentSchemaPath;
    while(it.Next(nullptr, &currentSchemaPath))
    {
        vmf_string currentSchemaName;
        loadSchemaName(currentSchemaPath, currentSchemaName);
        if (currentSchemaName == name)
        {
            return currentSchemaPath;
        }
    }
    return vmf_string("");
}

vmf_string XMPMetadataSource::findProperty(const vmf_string& pathToSchema, const vmf_string& name)
{
    vmf_string pathToSchemaSet;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToSchemaSet);
    SXMPIterator pIter(*xmp, VMF_NS, pathToSchemaSet.c_str(), kXMP_IterJustChildren);
    vmf_string currentPropertyPath;
    while(pIter.Next(nullptr, &currentPropertyPath))
    {
        vmf_string currentPropertyName;
        if (!xmp->GetStructField(VMF_NS, currentPropertyPath.c_str(), VMF_NS, PROPERTY_NAME, &currentPropertyName, 0))
        {
            VMF_EXCEPTION(DataStorageException, "Broken property by path " + currentPropertyPath);
        }
        if (currentPropertyName == name)
        {
            return currentPropertyPath;
        }
    }
    return vmf_string("");
}

void XMPMetadataSource::remove(const vector<IdType>& removedIds)
{
    for (auto id = removedIds.rbegin(); id != removedIds.rend(); ++id)
    {
        auto property = idMap.find(*id);
        if (property != idMap.end())
        {
            xmp->DeleteProperty(VMF_NS, property->second.path.c_str());
            idMap.erase(property);
        }
    }
}

void XMPMetadataSource::clear()
{
    xmp->DeleteProperty(VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY);
}

void XMPMetadataSource::loadIds()
{
    idMap.clear();
    SXMPIterator sIter(*xmp, VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_IterJustChildren);
    vmf_string currentSchemaPath;
    while (sIter.Next(nullptr, &currentSchemaPath))
    {
        loadIds(currentSchemaPath);
    }
}

void XMPMetadataSource::loadIds(const vmf::vmf_string& pathToSchema)
{
    vmf_string pathToPropertiesArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToPropertiesArray);
    vmf_string schemaName;
    loadSchemaName(pathToSchema, schemaName);

    SXMPIterator pIter(*xmp, VMF_NS, pathToPropertiesArray.c_str(), kXMP_IterJustChildren);
    vmf_string pathToCurrentProperty;
    while (pIter.Next(nullptr, &pathToCurrentProperty))
    {
        vmf_string metadataName;
        loadPropertyName(pathToCurrentProperty, metadataName);

        vmf_string pathToCurrentMetadataSet;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentProperty.c_str(), VMF_NS, PROPERTY_SET, &pathToCurrentMetadataSet);
        SXMPIterator mIter(*xmp, VMF_NS, pathToCurrentMetadataSet.c_str(), kXMP_IterJustChildren);
        vmf_string pathToCurrentMetadata;
        while (mIter.Next(nullptr, &pathToCurrentMetadata))
        {
            IdType id;
            loadMetadataId(pathToCurrentMetadata, id);
            InternalPath path;
            path.schema = schemaName;
            path.metadata = metadataName;
            path.path = pathToCurrentMetadata;
            idMap[id] = path;
        }
    }
}

void XMPMetadataSource::loadMetadataId(const vmf_string& pathToMetadata, IdType& id)
{
    vmf_string pathToId;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_ID, &pathToId);
    XMP_Int64 idValue;
    if(!xmp->GetProperty_Int64(VMF_NS, pathToId.c_str(), &idValue, 0))
    {
        VMF_EXCEPTION(DataStorageException, "Broken property by path " + pathToMetadata);
    }
    id = (IdType) idValue;
}

void XMPMetadataSource::saveMetadataId(const vmf_string& pathToMetadata, const IdType& id)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_ID, &tmpPath);
    xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), id);
}

void XMPMetadataSource::loadMetadataFrameIndex(const vmf_string& pathToMetadata, long long& frameIndex)
{
    XMP_Int64 frameIndexValue;
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_FRAME_INDEX, &tmpPath);
    if(!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &frameIndexValue, nullptr))
    {
        frameIndexValue = Metadata::UNDEFINED_FRAME_INDEX;
    }
    if(frameIndexValue < 0 && frameIndexValue != Metadata::UNDEFINED_FRAME_INDEX)
    {
        VMF_EXCEPTION(DataStorageException, "Can't load metadata frame index. Invalid frame index value");
    }
    frameIndex = frameIndexValue;
}

void XMPMetadataSource::saveMetadataFrameIndex(const vmf_string& pathToProperty, const long long& frameIndex)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_FRAME_INDEX, &tmpPath);
    if (frameIndex >= 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), frameIndex);
    else if (frameIndex == Metadata::UNDEFINED_FRAME_INDEX)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata frame index. Invalid frame index value");
}

void XMPMetadataSource::loadMetadataNumOfFrames(const vmf_string& pathToProperty, long long& num)
{
    XMP_Int64 numOfFrames;
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_NUM_OF_FRAMES, &tmpPath);
    if (!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &numOfFrames, nullptr))
    {
        numOfFrames = Metadata::UNDEFINED_FRAMES_NUMBER;
    }
    if(numOfFrames < 0)
    {
        VMF_EXCEPTION(DataStorageException, "Can't load metadata number of frames. Invalid number of frames value");
    }
    num = numOfFrames;
}


void XMPMetadataSource::saveMetadataNumOfFrames(const vmf_string& pathToProperty, const long long& numOfFrames)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_NUM_OF_FRAMES, &tmpPath);
    if (numOfFrames > 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), numOfFrames);
    else if (numOfFrames == Metadata::UNDEFINED_FRAMES_NUMBER)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata number of frames. Invalid number of frames value");
}

void XMPMetadataSource::loadMetadataTime(const vmf_string& pathToProperty, long long& time)
{
    XMP_Int64 timestamp;
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_TIMESTAMP, &tmpPath);
    if (!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &timestamp, nullptr))
    {
        timestamp = Metadata::UNDEFINED_TIMESTAMP;
    }
    if(timestamp < 0 && timestamp != Metadata::UNDEFINED_TIMESTAMP)
    {
        VMF_EXCEPTION(DataStorageException, "Can't load metadata timestamp. Invalid timestamp value");
    }
    time = timestamp;
}

void XMPMetadataSource::saveMetadataTime(const vmf_string& pathToProperty, const long long& time)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_TIMESTAMP, &tmpPath);
    if (time >= 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), time);
    else if (time == Metadata::UNDEFINED_TIMESTAMP)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata timestamp. Invalid timestamp value");
}


void XMPMetadataSource::saveMetadataEncrypted(const vmf_string &pathToProperty, bool isEncrypted,
                                              const vmf_string &encryptedData)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS,
                                      METADATA_ENCRYPTED_DATA, &tmpPath);
    if(encryptedData.length() > 0)
    {
        xmp->SetProperty(VMF_NS, tmpPath.c_str(), encryptedData.c_str());
    }
    else
    {
        vmf_string tmpString;
        if(xmp->GetProperty(VMF_NS, tmpPath.c_str(), &tmpString, nullptr))
        {
            xmp->DeleteStructField(VMF_NS, tmpPath.c_str(), VMF_NS, METADATA_ENCRYPTED_DATA);
        }
    }
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS,
                                      METADATA_ENCRYPTED_BOOL, &tmpPath);
    if(isEncrypted)
    {
        xmp->SetProperty(VMF_NS, tmpPath.c_str(), "true");
    }
    else
    {
        vmf_string tmpString;
        if(xmp->GetProperty(VMF_NS, tmpPath.c_str(), &tmpString, nullptr))
        {
            xmp->DeleteStructField(VMF_NS, tmpPath.c_str(), VMF_NS, METADATA_ENCRYPTED_BOOL);
        }
    }
}


void XMPMetadataSource::loadMetadataEncrypted(const vmf_string &pathToProperty, bool& isEncrypted,
                                              vmf_string &encryptedData)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS,
                                      METADATA_ENCRYPTED_DATA, &tmpPath);
    if(!xmp->GetProperty(VMF_NS, tmpPath.c_str(), &encryptedData, nullptr))
    {
        encryptedData = "";
    }
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS,
                                      METADATA_ENCRYPTED_BOOL, &tmpPath);
    vmf_string textBool;
    isEncrypted = xmp->GetProperty(VMF_NS, tmpPath.c_str(), &textBool, nullptr);
    isEncrypted = isEncrypted && textBool == "true";
    if(isEncrypted && encryptedData.empty())
    {
        VMF_EXCEPTION(DataStorageException, "No encrypted data provided in metadata");
    }
}


void XMPMetadataSource::loadMetadataDuration(const vmf_string& pathToProperty, long long& dur)
{
    XMP_Int64 duration;
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_DURATION, &tmpPath);
    if (!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &duration, nullptr))
    {
        duration = Metadata::UNDEFINED_DURATION;
    }
    if(duration < 0)
    {
        VMF_EXCEPTION(DataStorageException, "Can't load metadata duration. Invalid duration value");
    }
    dur = duration;
}

void XMPMetadataSource::saveMetadataDuration(const vmf_string& pathToProperty, const long long& duration)
{
    vmf_string tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_DURATION, &tmpPath);
    if (duration > 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), duration);
    else if (duration == Metadata::UNDEFINED_DURATION)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata duration. Invalid duration value");

}

void XMPMetadataSource::savePropertyName(const vmf_string& pathToProperty, const vmf_string& name)
{
    xmp->SetStructField(VMF_NS, pathToProperty.c_str(), VMF_NS, PROPERTY_NAME, name.c_str());
}

void XMPMetadataSource::saveMetadataFields(const vmf_string& pathToMetadata, const shared_ptr<Metadata>& md)
{
    xmp->DeleteStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_FIELDS);
    vmf_string fieldsPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_FIELDS, &fieldsPath);
    vector<vmf_string> fieldNames = md->getFieldNames();
    if (fieldNames.empty() && !md->empty())
    {
        for(auto it = md->begin(); it != md->end(); ++it)
        {
            saveField("", *it, it->getUseEncryption(), it->getEncryptedData(), fieldsPath);
        }
    }
    else
    {
        for(auto it = fieldNames.begin(); it != fieldNames.end(); ++it)
        {
            FieldValue& fv = *md->findField(*it);
            saveField(*it, md->getFieldValue(*it), fv.getUseEncryption(), fv.getEncryptedData(), fieldsPath);
        }
    }
}

void XMPMetadataSource::saveMetadataReferences(const vmf_string& pathToMetadata, const shared_ptr<Metadata>& md)
{
    auto refs = md->getAllReferences();
    if (refs.empty())
    {
        return;
    }
    xmp->DeleteStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES);
    vmf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES, &pathToRefs);
    xmp->SetStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES, nullptr, kXMP_PropValueIsArray);
    for(auto ref = refs.begin(); ref != refs.end(); ++ref)
    {
        auto spMetadata = ref->getReferenceMetadata().lock();
        if (spMetadata == NULL)
            VMF_EXCEPTION(NullPointerException, "Trying to save nullptr reference in property by path " + pathToMetadata);

        xmp->AppendArrayItem(VMF_NS, pathToRefs.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        vmf_string pathToThisRef;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToRefs.c_str(), kXMP_ArrayLastItem, &pathToThisRef);

        vmf_string tmpPath;

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToThisRef.c_str(), VMF_NS, REF_NAME, &tmpPath);
        xmp->SetProperty(VMF_NS, tmpPath.c_str(), ref->getReferenceDescription()->name.c_str());

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToThisRef.c_str(), VMF_NS, REF_ID, &tmpPath);
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), spMetadata->getId());
    }
}
