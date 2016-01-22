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

#define FIELD_TYPE "type"
#define FIELD_VALUE "value"
#define FIELD_NAME "name"

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

void XMPMetadataSource::saveSchema(const MetaString& schemaName, const MetadataStream& stream)
{
    shared_ptr<MetadataSchema> thisSchemaDescription = stream.getSchema(schemaName);

    MetaString thisSchemaPath = findSchema(schemaName);

    if (thisSchemaPath.empty())
    {
        xmp->AppendArrayItem(VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_ArrayLastItem, &thisSchemaPath);
        xmp->SetStructField(VMF_NS, thisSchemaPath.c_str(), VMF_NS, SCHEMA_NAME, schemaName);
        xmp->SetStructField(VMF_NS, thisSchemaPath.c_str(), VMF_NS, SCHEMA_SET, nullptr, kXMP_PropValueIsArray);
    }

    MetadataSet thisSchemaSet = stream.queryBySchema(schemaName);
    vector< shared_ptr<MetadataDesc> > thisSchemaProperties = thisSchemaDescription->getAll();
    for(auto descIter = thisSchemaProperties.begin(); descIter != thisSchemaProperties.end(); ++descIter)
    {
        MetaString metadataName = (*descIter)->getMetadataName();
        MetadataSet currentPropertySet(thisSchemaSet.queryByName(metadataName));
        saveProperty(currentPropertySet, thisSchemaPath, metadataName);
    }
}

void XMPMetadataSource::saveProperty(const MetadataSet& property, const MetaString& pathToSchema, const MetaString& propertyName)
{
    if (property.empty())
    {
        // not loaded or empty. In any case nothing to do.
        return;
    }

    MetaString pathToPropertiesArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToPropertiesArray);

    MetaString thisPropertyPath = findProperty(pathToSchema, propertyName);
    if (thisPropertyPath.empty())
    {
        xmp->AppendArrayItem(VMF_NS, pathToPropertiesArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToPropertiesArray.c_str(), kXMP_ArrayLastItem, &thisPropertyPath);
    }

    savePropertyName(thisPropertyPath, propertyName);
    xmp->SetStructField(VMF_NS, thisPropertyPath.c_str(), VMF_NS, PROPERTY_SET, nullptr, kXMP_PropValueIsArray);
    MetaString thisPropertySetPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, thisPropertyPath.c_str(), VMF_NS, PROPERTY_SET, &thisPropertySetPath);

    for(auto metadata = property.begin(); metadata != property.end(); ++metadata)
    {
        saveMetadata(*metadata, thisPropertySetPath);
    }
}


void XMPMetadataSource::saveMetadata(const shared_ptr<Metadata>& md, const MetaString& thisPropertySetPath)
{
    if (md == nullptr)
        VMF_EXCEPTION(DataStorageException, "Trying to save nullptr metadata");

    MetaString pathToMetadata;

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
}

void XMPMetadataSource::saveField(const MetaString& fieldName, const Variant& _value, const MetaString& fieldsPath)
{
    std::string value = _value.toString();
    if(value.empty())
        value = " ";
    xmp->AppendArrayItem(VMF_NS, fieldsPath.c_str(), kXMP_PropValueIsArray, value, kXMP_NoOptions);
    if (!fieldName.empty())
    {
        MetaString thisFieldPath;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, fieldsPath.c_str(), kXMP_ArrayLastItem, &thisFieldPath);
        xmp->SetQualifier(VMF_NS, thisFieldPath.c_str(), VMF_NS, FIELD_NAME, fieldName.c_str(), kXMP_NoOptions);
    }
}

void XMPMetadataSource::loadSchema(const MetaString &schemaName, MetadataStream &stream)
{
    MetaString schemaPath = findSchema(schemaName);
    if (schemaPath.empty())
    {
        VMF_EXCEPTION(DataStorageException, "Schema " + schemaName + " not found");
    }

    MetaString pathToProperties;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_SET, &pathToProperties);
    SXMPIterator propIterator(*xmp, VMF_NS, pathToProperties.c_str(), kXMP_IterJustChildren);
    MetaString currentPropertyPath;
    while(propIterator.Next(NULL, &currentPropertyPath))
    {
        loadPropertyByPath(currentPropertyPath, schemaName, stream);
    }
}

void XMPMetadataSource::loadProperty(const MetaString& schemaName, const MetaString& metadataName, MetadataStream& stream)
{
    MetaString pathToSchema = findSchema(schemaName);
    MetaString pathToProperty = findProperty(pathToSchema, metadataName);
    loadPropertyByPath(pathToProperty, schemaName, stream);
}

void XMPMetadataSource::loadSchemaName(const MetaString &pathToSchema, MetaString& schemaName)
{
    if (!xmp->GetStructField(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_NAME, &schemaName, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Broken schema by path " + pathToSchema);
    }
}

void XMPMetadataSource::loadPropertyByPath(const MetaString& pathToProperty, const MetaString& schemaName, MetadataStream& stream)
{
    shared_ptr<MetadataSchema> schema(stream.getSchema(schemaName));
    MetaString metadataName;
    loadPropertyName(pathToProperty, metadataName);
    MetaString pathToMetadataSet;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, PROPERTY_SET, &pathToMetadataSet);

    shared_ptr<MetadataDesc> description(schema->findMetadataDesc(metadataName));

    SXMPIterator mIter(*xmp, VMF_NS, pathToMetadataSet.c_str(), kXMP_IterJustChildren);
    MetaString pathToCurrentMetadata;
    while(mIter.Next(nullptr, &pathToCurrentMetadata))
    {
        loadMetadata(pathToCurrentMetadata, description, stream);
    }
    //unsorted stream fails on save
    stream.sortById();
}

void XMPMetadataSource::loadMetadata(const MetaString& pathToCurrentMetadata, const shared_ptr<MetadataDesc>& description, MetadataStream& stream)
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

    shared_ptr<MetadataAccessor> metadataAccessor(new MetadataAccessor(description));

    metadataAccessor->setFrameIndex(frameIndex, numOfFrames);
    metadataAccessor->setTimestamp(timestamp, duration);
    metadataAccessor->setId(id);
    MetaString fieldsPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentMetadata.c_str(), VMF_NS, METADATA_FIELDS, &fieldsPath);

    shared_ptr<MetadataSchema> thisSchemaDesc = stream.getSchema(description->getSchemaName());
    shared_ptr<MetadataDesc> thisPropertyDesc = thisSchemaDesc->findMetadataDesc(description->getMetadataName());
    SXMPIterator fieldsIterator(*xmp, VMF_NS, fieldsPath.c_str(), kXMP_IterJustChildren);
    MetaString currentFieldPath;
    while(fieldsIterator.Next(NULL, &currentFieldPath))
    {
        loadField(currentFieldPath, metadataAccessor, thisPropertyDesc);
    }

    MetadataStreamAccessor* streamAccessor = (MetadataStreamAccessor*) &stream;
    streamAccessor->internalAdd(metadataAccessor);

    // Load refs only after adding to steam to stop recursive loading when there are circular references
    MetaString pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentMetadata.c_str(), VMF_NS, METADATA_REFERENCES, &pathToRefs);
    SXMPIterator refsIterator(*xmp, VMF_NS, pathToRefs.c_str(), kXMP_IterJustChildren);
    MetaString currentRefPath;
    while(refsIterator.Next(NULL, &currentRefPath))
    {
        loadReference(currentRefPath, metadataAccessor, stream);
    }

}

void XMPMetadataSource::loadPropertyName(const MetaString& pathToMetadata, MetaString& metadataName)
{
    if(!xmp->GetStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, PROPERTY_NAME, &metadataName, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted property by path " + pathToMetadata);
    }
}

void XMPMetadataSource::loadField(const MetaString& fieldPath, const shared_ptr<Metadata>& md, const shared_ptr<MetadataDesc>& thisPropertyDesc)
{
    MetaString rawValue;
    if (!xmp->GetProperty(VMF_NS, fieldPath.c_str(), &rawValue, NULL))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted field by path " + fieldPath);
    }


    MetaString fieldName;
    if (!xmp->GetQualifier(VMF_NS, fieldPath.c_str(), VMF_NS, FIELD_NAME, &fieldName, NULL))
    {
        fieldName = "";
    }

    FieldDesc thisFieldDesc;
    if (!thisPropertyDesc->getFieldDesc(thisFieldDesc, fieldName))
    {
        VMF_EXCEPTION(DataStorageException, "Extra field by path " + fieldPath);
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
}

void XMPMetadataSource::loadReference(const MetaString& thisRefPath, const shared_ptr<Metadata>& md, MetadataStream& stream)
{
    XMP_Int64 id;
    MetaString tmpPath;
    MetaString refName;
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

MetaString XMPMetadataSource::findSchema(const MetaString& name)
{
    SXMPIterator it(*xmp, VMF_NS, VMF_GLOBAL_SCHEMAS_ARRAY, kXMP_IterJustChildren);
    MetaString currentSchemaPath;
    while(it.Next(nullptr, &currentSchemaPath))
    {
        MetaString currentSchemaName;
        loadSchemaName(currentSchemaPath, currentSchemaName);
        if (currentSchemaName == name)
        {
            return currentSchemaPath;
        }
    }
    return MetaString("");
}

MetaString XMPMetadataSource::findProperty(const MetaString& pathToSchema, const MetaString& name)
{
    MetaString pathToSchemaSet;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToSchemaSet);
    SXMPIterator pIter(*xmp, VMF_NS, pathToSchemaSet.c_str(), kXMP_IterJustChildren);
    MetaString currentPropertyPath;
    while(pIter.Next(nullptr, &currentPropertyPath))
    {
        MetaString currentPropertyName;
        if (!xmp->GetStructField(VMF_NS, currentPropertyPath.c_str(), VMF_NS, PROPERTY_NAME, &currentPropertyName, 0))
        {
            VMF_EXCEPTION(DataStorageException, "Broken property by path " + currentPropertyPath);
        }
        if (currentPropertyName == name)
        {
            return currentPropertyPath;
        }
    }
    return MetaString("");
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
    MetaString currentSchemaPath;
    while (sIter.Next(nullptr, &currentSchemaPath))
    {
        loadIds(currentSchemaPath);
    }
}

void XMPMetadataSource::loadIds(const vmf::MetaString& pathToSchema)
{
    MetaString pathToPropertiesArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_SET, &pathToPropertiesArray);
    MetaString schemaName;
    loadSchemaName(pathToSchema, schemaName);

    SXMPIterator pIter(*xmp, VMF_NS, pathToPropertiesArray.c_str(), kXMP_IterJustChildren);
    MetaString pathToCurrentProperty;
    while (pIter.Next(nullptr, &pathToCurrentProperty))
    {
        MetaString metadataName;
        loadPropertyName(pathToCurrentProperty, metadataName);

        MetaString pathToCurrentMetadataSet;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToCurrentProperty.c_str(), VMF_NS, PROPERTY_SET, &pathToCurrentMetadataSet);
        SXMPIterator mIter(*xmp, VMF_NS, pathToCurrentMetadataSet.c_str(), kXMP_IterJustChildren);
        MetaString pathToCurrentMetadata;
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

void XMPMetadataSource::loadMetadataId(const MetaString& pathToMetadata, IdType& id)
{
    MetaString pathToId;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_ID, &pathToId);
    XMP_Int64 idValue;
    if(!xmp->GetProperty_Int64(VMF_NS, pathToId.c_str(), &idValue, 0))
    {
        VMF_EXCEPTION(DataStorageException, "Broken property by path " + pathToMetadata);
    }
    id = (IdType) idValue;
}

void XMPMetadataSource::saveMetadataId(const MetaString& pathToMetadata, const IdType& id)
{
    MetaString tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_ID, &tmpPath);
    xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), id);
}

void XMPMetadataSource::loadMetadataFrameIndex(const MetaString& pathToMetadata, long long& frameIndex)
{
    XMP_Int64 frameIndexValue;
    MetaString tmpPath;
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

void XMPMetadataSource::saveMetadataFrameIndex(const MetaString& pathToProperty, const long long& frameIndex)
{
    MetaString tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_FRAME_INDEX, &tmpPath);
    if (frameIndex >= 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), frameIndex);
    else if (frameIndex == Metadata::UNDEFINED_FRAME_INDEX)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata frame index. Invalid frame index value");
}

void XMPMetadataSource::loadMetadataNumOfFrames(const MetaString& pathToProperty, long long& num)
{
    XMP_Int64 numOfFrames;
    MetaString tmpPath;
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


void XMPMetadataSource::saveMetadataNumOfFrames(const MetaString& pathToProperty, const long long& numOfFrames)
{
    MetaString tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_NUM_OF_FRAMES, &tmpPath);
    if (numOfFrames > 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), numOfFrames);
    else if (numOfFrames == Metadata::UNDEFINED_FRAMES_NUMBER)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata number of frames. Invalid number of frames value");
}

void XMPMetadataSource::loadMetadataTime(const MetaString& pathToProperty, long long& time)
{
    XMP_Int64 timestamp;
    MetaString tmpPath;
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

void XMPMetadataSource::saveMetadataTime(const MetaString& pathToProperty, const long long& time)
{
    MetaString tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_TIMESTAMP, &tmpPath);
    if (time >= 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), time);
    else if (time == Metadata::UNDEFINED_TIMESTAMP)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata timestamp. Invalid timestamp value");
}

void XMPMetadataSource::loadMetadataDuration(const MetaString& pathToProperty, long long& dur)
{
    XMP_Int64 duration;
    MetaString tmpPath;
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

void XMPMetadataSource::saveMetadataDuration(const MetaString& pathToProperty, const long long& duration)
{
    MetaString tmpPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToProperty.c_str(), VMF_NS, METADATA_DURATION, &tmpPath);
    if (duration > 0)
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), duration);
    else if (duration == Metadata::UNDEFINED_DURATION)
        xmp->DeleteProperty(VMF_NS, tmpPath.c_str());
    else
        VMF_EXCEPTION(DataStorageException, "Can't save metadata duration. Invalid duration value");

}

void XMPMetadataSource::savePropertyName(const MetaString& pathToProperty, const MetaString& name)
{
    xmp->SetStructField(VMF_NS, pathToProperty.c_str(), VMF_NS, PROPERTY_NAME, name.c_str());
}

void XMPMetadataSource::saveMetadataFields(const MetaString& pathToMetadata, const shared_ptr<Metadata>& md)
{
    xmp->DeleteStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_FIELDS);
    MetaString fieldsPath;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_FIELDS, &fieldsPath);
    vector<MetaString> fieldNames = md->getFieldNames();
    if (fieldNames.empty() && !md->empty())
    {
        for(auto it = md->begin(); it != md->end(); ++it)
        {
            saveField("", *it, fieldsPath);
        }
    }
    else
    {
        for(auto it = fieldNames.begin(); it != fieldNames.end(); ++it)
        {
            saveField(*it, md->getFieldValue(*it), fieldsPath);
        }
    }

}

void XMPMetadataSource::saveMetadataReferences(const MetaString& pathToMetadata, const shared_ptr<Metadata>& md)
{
    auto refs = md->getAllReferences();
    if (refs.empty())
    {
        return;
    }
    xmp->DeleteStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES);
    MetaString pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES, &pathToRefs);
    xmp->SetStructField(VMF_NS, pathToMetadata.c_str(), VMF_NS, METADATA_REFERENCES, nullptr, kXMP_PropValueIsArray);
    for(auto ref = refs.begin(); ref != refs.end(); ++ref)
    {
        auto spMetadata = ref->getReferenceMetadata().lock();
        if (spMetadata == NULL)
            VMF_EXCEPTION(NullPointerException, "Trying to save nullptr reference in property by path " + pathToMetadata);

        xmp->AppendArrayItem(VMF_NS, pathToRefs.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        MetaString pathToThisRef;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToRefs.c_str(), kXMP_ArrayLastItem, &pathToThisRef);

        MetaString tmpPath;

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToThisRef.c_str(), VMF_NS, REF_NAME, &tmpPath);
        xmp->SetProperty(VMF_NS, tmpPath.c_str(), ref->getReferenceDescription()->name.c_str());

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToThisRef.c_str(), VMF_NS, REF_ID, &tmpPath);
        xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), spMetadata->getId());
    }
}
