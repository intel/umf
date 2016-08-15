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
#include <algorithm>

#include "xmpschemasource.hpp"


#define UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY "schemas"
#define UMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY "metadata"


#define SCHEMA_NAME "schema"
#define SCHEMA_PROPERTIES "descriptors"
#define SCHEMA_AUTHOR "schema-author"
#define SCHEMA_ENCRYPTED "schema-encrypted"


#define PROPERTY_NAME "name"
#define PROPERTY_FIELDS "fields"
#define PROPERTY_REFERENCES "references"
#define PROPERTY_ENCRYPTED "encrypted"

#define REFERENCE_NAME "name"
#define REFERENCE_UNIQUE "unique"
#define REFERENCE_CUSTOM "custom"

#define FIELD_NAME "name"
#define FIELD_TYPE "type"
#define FIELD_OPTIONALITY "optional"
#define FIELD_ENCRYPTED "encrypted"


using namespace std;
using namespace umf;



class MetadataSchemaAccessor: public MetadataSchema
{
public:
    MetadataSchemaAccessor( const std::string& sName )
        : MetadataSchema(sName) { }
    using vector< shared_ptr<MetadataDesc> >::begin;
    using vector< shared_ptr<MetadataDesc> >::end;
};



XMPSchemaSource::XMPSchemaSource(const std::shared_ptr<SXMPMeta>& schemaStorage)
    : metadata(schemaStorage)
{
}



XMPSchemaSource::~XMPSchemaSource()
{
}



void XMPSchemaSource::save(const shared_ptr<MetadataSchema>& schema)
{
    XMP_Index size = metadata->CountArrayItems(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        umf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        umf_string currentSchemaName;
        if (!metadata->GetStructField(UMF_NS, currentSchemaPath.c_str(), UMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            UMF_EXCEPTION(DataStorageException, "Corrupted schema by path" + currentSchemaPath);
        }
        if (currentSchemaName == schema->getName())
        {
            metadata->DeleteArrayItem(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }

    umf_string schemaPath;

    metadata->AppendArrayItem(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_ArrayLastItem, &schemaPath);
    metadata->SetStructField(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_NAME, schema->getName());
    auto author = schema->getAuthor();
    if(!author.empty())
    {
        metadata->SetStructField(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_AUTHOR, schema->getAuthor());
    }
    bool useEncryption = schema->getUseEncryption();
    if(useEncryption)
    {
        metadata->SetStructField(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_ENCRYPTED, "true");
    }
    else
    {
        umf_string tmpString;
        if(metadata->GetStructField(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_ENCRYPTED);
        }
    }

    umf_string pathToDescriptions;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, schemaPath.c_str(), UMF_NS, SCHEMA_PROPERTIES, &pathToDescriptions);

    shared_ptr<MetadataSchemaAccessor> accessor = static_pointer_cast<MetadataSchemaAccessor>(schema);

    for(auto it = accessor->begin(); it != accessor->end(); ++it)
        saveDescription(**it, pathToDescriptions);
}


void XMPSchemaSource::saveDescription(const MetadataDesc& desc, const umf_string& pathToDescriptions)
{
    SXMPIterator descIterator(*metadata, UMF_NS, pathToDescriptions.c_str(), kXMP_IterJustChildren);
    umf_string currentPropertyPath;
    umf_string propertyPath;
    while(descIterator.Next(NULL, &currentPropertyPath))
    {
        umf_string currentPropertyName;
        if(!metadata->GetStructField(UMF_NS, currentPropertyPath.c_str(), UMF_NS, PROPERTY_NAME, &currentPropertyName, NULL))
        {
            UMF_EXCEPTION(DataStorageException, "Corrupted property by path " + currentPropertyPath);
        }
        if (desc.getMetadataName() == currentPropertyName)
        {
            propertyPath = currentPropertyPath;
            break;
        }
    }
    if (propertyPath.empty())
    {
        metadata->AppendArrayItem(UMF_NS, pathToDescriptions.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(UMF_NS, pathToDescriptions.c_str(), kXMP_ArrayLastItem, &propertyPath);
        metadata->SetStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_NAME, desc.getMetadataName());
    }

    bool useEncryption = desc.getUseEncryption();
    if(useEncryption)
    {
        metadata->SetStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_ENCRYPTED, "true");
    }
    else
    {
        umf_string tmpString;
        if(metadata->GetStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_ENCRYPTED);
        }
    }

    metadata->DeleteStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_FIELDS);
    umf_string pathToFields;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_FIELDS, &pathToFields);

    vector<FieldDesc> fields = desc.getFields();
    for(auto field = fields.begin(); field != fields.end(); ++field)
    {
        saveField(*field, pathToFields);
    }

    metadata->DeleteStructField(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_REFERENCES);
    umf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, propertyPath.c_str(), UMF_NS, PROPERTY_REFERENCES, &pathToRefs);

    auto refs = desc.getAllReferenceDescs();
    for (auto ref = refs.begin(); ref != refs.end(); ref++)
    {
        saveReference((*ref), pathToRefs);
    }
}



void XMPSchemaSource::saveField(const FieldDesc& desc, const umf_string& pathToFields)
{
    metadata->AppendArrayItem(UMF_NS, pathToFields.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    umf_string thisField;
    SXMPUtils::ComposeArrayItemPath(UMF_NS, pathToFields.c_str(), kXMP_ArrayLastItem, &thisField);
    metadata->SetStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_NAME, desc.name.c_str());
    metadata->SetStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_TYPE, Variant::typeToString(desc.type).c_str());
    if(desc.optional)
        metadata->SetStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_OPTIONALITY, "true");
    if(desc.useEncryption)
    {
        metadata->SetStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_ENCRYPTED, "true");
    }
    else
    {
        umf_string tmpString;
        if(metadata->GetStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(UMF_NS, thisField.c_str(), UMF_NS, FIELD_ENCRYPTED);
        }
    }
}

void XMPSchemaSource::saveReference(const std::shared_ptr<ReferenceDesc> ref, const umf_string& pathToRefs)
{
    metadata->AppendArrayItem(UMF_NS, pathToRefs.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    umf_string thisRef;
    
    SXMPUtils::ComposeArrayItemPath(UMF_NS, pathToRefs.c_str(), kXMP_ArrayLastItem, &thisRef);
    metadata->SetStructField(UMF_NS, thisRef.c_str(), UMF_NS, REFERENCE_NAME, ref->name.c_str());
    if (ref->isUnique)
        metadata->SetStructField(UMF_NS, thisRef.c_str(), UMF_NS, REFERENCE_UNIQUE, "true");

    if (ref->isCustom)
        metadata->SetStructField(UMF_NS, thisRef.c_str(), UMF_NS, REFERENCE_CUSTOM, "true");

    return;
}

void XMPSchemaSource::load(std::map<umf::umf_string, std::shared_ptr<umf::MetadataSchema> >& schemas)
{
    SXMPIterator schemasIterator(*metadata, UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_IterJustChildren);
    umf_string currentSchemaPath;
    while (schemasIterator.Next(nullptr, &currentSchemaPath))
    {
        shared_ptr<MetadataSchema> schema = loadMetadataSchemaByPath(currentSchemaPath);
        if (schema != nullptr)
        {
            schemas[schema->getName()] = schema;
        }
    }
}

shared_ptr<MetadataSchema> XMPSchemaSource::loadMetadataSchemaByPath(const umf_string& pathToSchema)
{
    umf_string thisSchemaName, thisSchemaAuthor;
    if(!metadata->GetStructField(UMF_NS, pathToSchema.c_str(), UMF_NS, SCHEMA_NAME, &thisSchemaName, nullptr))
    {
        UMF_EXCEPTION(DataStorageException, "Corrupted schema description by path " + pathToSchema);
    }
    metadata->GetStructField(UMF_NS, pathToSchema.c_str(), UMF_NS, SCHEMA_AUTHOR, &thisSchemaAuthor, nullptr);
    umf_string thisSchemaEncrypted;
    bool useEncryption = metadata->GetStructField(UMF_NS, pathToSchema.c_str(), UMF_NS, SCHEMA_ENCRYPTED, &thisSchemaEncrypted, nullptr);
    useEncryption = useEncryption && (thisSchemaEncrypted == "true");
    auto schema = make_shared<MetadataSchema>(thisSchemaName, thisSchemaAuthor, useEncryption);

    umf_string thisSchemaDescriptors;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToSchema.c_str(), UMF_NS, SCHEMA_PROPERTIES, &thisSchemaDescriptors);

    SXMPIterator propertiesIterator(*metadata, UMF_NS, thisSchemaDescriptors.c_str(), kXMP_IterJustChildren);
    umf_string currentPropertyPath;
    while(propertiesIterator.Next(NULL, &currentPropertyPath))
    {
        shared_ptr<MetadataDesc> desc = loadDescription(currentPropertyPath);
        schema->add(desc);
    }
    return schema;
}

shared_ptr<MetadataDesc> XMPSchemaSource::loadDescription(const umf_string& pathToDesc)
{
    umf_string propertyName;
    if(!metadata->GetStructField(UMF_NS, pathToDesc.c_str(), UMF_NS, PROPERTY_NAME, &propertyName, NULL))
    {
        UMF_EXCEPTION(DataStorageException, "Corrupted property by path " + pathToDesc);
    }
    umf_string encryptedDescStr;
    bool useEncryptionDesc = metadata->GetStructField(UMF_NS, pathToDesc.c_str(), UMF_NS,
                                                      PROPERTY_ENCRYPTED, &encryptedDescStr, NULL);
    useEncryptionDesc = useEncryptionDesc && encryptedDescStr == "true";

    vector<FieldDesc> fields;
    umf_string pathToFields;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToDesc.c_str(), UMF_NS, PROPERTY_FIELDS, &pathToFields);
    SXMPIterator fieldsIterator(*metadata, UMF_NS, pathToFields.c_str(), kXMP_IterJustChildren);
    umf_string currentFieldPath;
    while(fieldsIterator.Next(NULL, &currentFieldPath))
    {
        umf_string name;
        if(!metadata->GetStructField(UMF_NS, currentFieldPath.c_str(), UMF_NS, FIELD_NAME, &name, NULL))
        {
            name = "";
        }

        umf_string rawType;
        if(!metadata->GetStructField(UMF_NS, currentFieldPath.c_str(), UMF_NS, FIELD_TYPE, &rawType, NULL))
        {
            UMF_EXCEPTION(DataStorageException, "Corrupted field by path " + currentFieldPath);
        }

        Variant::Type type = Variant::typeFromString(rawType);

        bool optional = true;
        if(!metadata->GetStructField(UMF_NS, currentFieldPath.c_str(), UMF_NS, FIELD_OPTIONALITY, NULL, NULL))
            optional = false;

        umf_string encryptedFieldStr;
        bool useEncryptionField = metadata->GetStructField(UMF_NS, currentFieldPath.c_str(), UMF_NS,
                                                           FIELD_ENCRYPTED, &encryptedFieldStr, NULL);
        useEncryptionField = useEncryptionField && encryptedFieldStr == "true";

        fields.push_back(FieldDesc(name, type, optional, useEncryptionField));
    }

    vector < shared_ptr<ReferenceDesc> > refs;
    umf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToDesc.c_str(), UMF_NS, PROPERTY_REFERENCES, &pathToRefs);

    SXMPIterator refsIterator(*metadata, UMF_NS, pathToRefs.c_str(), kXMP_IterJustChildren);
    umf_string currentReferencePath;

    while (refsIterator.Next(NULL, &currentReferencePath))
    {
        umf_string name;
        if (!metadata->GetStructField(UMF_NS, currentReferencePath.c_str(), UMF_NS, REFERENCE_NAME, &name, NULL))
            UMF_EXCEPTION(IncorrectParamException, "XMP element has invalid reference name.");

        bool isUnique = false;
        if (metadata->GetStructField(UMF_NS, currentReferencePath.c_str(), UMF_NS, REFERENCE_UNIQUE, NULL, NULL))
            isUnique = true;

        bool isCustom = false;
        if (metadata->GetStructField(UMF_NS, currentReferencePath.c_str(), UMF_NS, REFERENCE_CUSTOM, NULL, NULL))
            isCustom = true;

        refs.push_back(make_shared<ReferenceDesc>(name, isUnique, isCustom));
    }

    return shared_ptr<MetadataDesc>(new MetadataDesc(propertyName, fields, refs, useEncryptionDesc));
}

void XMPSchemaSource::clear()
{
    metadata->DeleteProperty(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
}

void XMPSchemaSource::remove(const umf_string& schemaName)
{
    if(schemaName == "")
    {
        metadata->DeleteProperty(UMF_NS, UMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY);
        metadata->DeleteProperty(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
        metadata->DeleteProperty(UMF_NS, "next-id");
        metadata->DeleteProperty(kXMP_NS_XMP, "CreatorTool");
        return;
    }

    XMP_Index size = metadata->CountArrayItems(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        umf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        umf_string currentSchemaName;
        if (!metadata->GetStructField(UMF_NS, currentSchemaPath.c_str(), UMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            UMF_EXCEPTION(DataStorageException, "Corrputed schema by path" + currentSchemaPath);
        }
        if (currentSchemaName == schemaName)
        {
            metadata->DeleteArrayItem(UMF_NS, UMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }

    size = metadata->CountArrayItems(UMF_NS, UMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        umf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        umf_string currentSchemaName;
        if (!metadata->GetStructField(UMF_NS, currentSchemaPath.c_str(), UMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            UMF_EXCEPTION(DataStorageException, "Corrputed metadata by path" + currentSchemaPath);
        }
        if (currentSchemaName == schemaName)
        {
            metadata->DeleteArrayItem(UMF_NS, UMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }
}
