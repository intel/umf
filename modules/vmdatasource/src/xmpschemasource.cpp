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


#define VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY "schemas"
#define VMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY "metadata"


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
using namespace vmf;



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
    XMP_Index size = metadata->CountArrayItems(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        vmf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        vmf_string currentSchemaName;
        if (!metadata->GetStructField(VMF_NS, currentSchemaPath.c_str(), VMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            VMF_EXCEPTION(DataStorageException, "Corrupted schema by path" + currentSchemaPath);
        }
        if (currentSchemaName == schema->getName())
        {
            metadata->DeleteArrayItem(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }

    vmf_string schemaPath;

    metadata->AppendArrayItem(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_ArrayLastItem, &schemaPath);
    metadata->SetStructField(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_NAME, schema->getName());
    auto author = schema->getAuthor();
    if(!author.empty())
    {
        metadata->SetStructField(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_AUTHOR, schema->getAuthor());
    }
    bool useEncryption = schema->getUseEncryption();
    if(useEncryption)
    {
        metadata->SetStructField(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_ENCRYPTED, "true");
    }
    else
    {
        vmf_string tmpString;
        if(metadata->GetStructField(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_ENCRYPTED);
        }
    }

    vmf_string pathToDescriptions;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, schemaPath.c_str(), VMF_NS, SCHEMA_PROPERTIES, &pathToDescriptions);

    shared_ptr<MetadataSchemaAccessor> accessor = static_pointer_cast<MetadataSchemaAccessor>(schema);

    for(auto it = accessor->begin(); it != accessor->end(); ++it)
        saveDescription(**it, pathToDescriptions);
}


void XMPSchemaSource::saveDescription(const MetadataDesc& desc, const vmf_string& pathToDescriptions)
{
    SXMPIterator descIterator(*metadata, VMF_NS, pathToDescriptions.c_str(), kXMP_IterJustChildren);
    vmf_string currentPropertyPath;
    vmf_string propertyPath;
    while(descIterator.Next(NULL, &currentPropertyPath))
    {
        vmf_string currentPropertyName;
        if(!metadata->GetStructField(VMF_NS, currentPropertyPath.c_str(), VMF_NS, PROPERTY_NAME, &currentPropertyName, NULL))
        {
            VMF_EXCEPTION(DataStorageException, "Corrupted property by path " + currentPropertyPath);
        }
        if (desc.getMetadataName() == currentPropertyName)
        {
            propertyPath = currentPropertyPath;
            break;
        }
    }
    if (propertyPath.empty())
    {
        metadata->AppendArrayItem(VMF_NS, pathToDescriptions.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToDescriptions.c_str(), kXMP_ArrayLastItem, &propertyPath);
        metadata->SetStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_NAME, desc.getMetadataName());
    }

    bool useEncryption = desc.getUseEncryption();
    if(useEncryption)
    {
        metadata->SetStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_ENCRYPTED, "true");
    }
    else
    {
        vmf_string tmpString;
        if(metadata->GetStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_ENCRYPTED);
        }
    }

    metadata->DeleteStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_FIELDS);
    vmf_string pathToFields;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_FIELDS, &pathToFields);

    vector<FieldDesc> fields = desc.getFields();
    for(auto field = fields.begin(); field != fields.end(); ++field)
    {
        saveField(*field, pathToFields);
    }

    metadata->DeleteStructField(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_REFERENCES);
    vmf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, propertyPath.c_str(), VMF_NS, PROPERTY_REFERENCES, &pathToRefs);

    auto refs = desc.getAllReferenceDescs();
    for (auto ref = refs.begin(); ref != refs.end(); ref++)
    {
        saveReference((*ref), pathToRefs);
    }
}



void XMPSchemaSource::saveField(const FieldDesc& desc, const vmf_string& pathToFields)
{
    metadata->AppendArrayItem(VMF_NS, pathToFields.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    vmf_string thisField;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToFields.c_str(), kXMP_ArrayLastItem, &thisField);
    metadata->SetStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_NAME, desc.name.c_str());
    metadata->SetStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_TYPE, Variant::typeToString(desc.type).c_str());
    if(desc.optional)
        metadata->SetStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_OPTIONALITY, "true");
    if(desc.useEncryption)
    {
        metadata->SetStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_ENCRYPTED, "true");
    }
    else
    {
        vmf_string tmpString;
        if(metadata->GetStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_ENCRYPTED, &tmpString, nullptr))
        {
            metadata->DeleteStructField(VMF_NS, thisField.c_str(), VMF_NS, FIELD_ENCRYPTED);
        }
    }
}

void XMPSchemaSource::saveReference(const std::shared_ptr<ReferenceDesc> ref, const vmf_string& pathToRefs)
{
    metadata->AppendArrayItem(VMF_NS, pathToRefs.c_str(), kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    vmf_string thisRef;
    
    SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToRefs.c_str(), kXMP_ArrayLastItem, &thisRef);
    metadata->SetStructField(VMF_NS, thisRef.c_str(), VMF_NS, REFERENCE_NAME, ref->name.c_str());
    if (ref->isUnique)
        metadata->SetStructField(VMF_NS, thisRef.c_str(), VMF_NS, REFERENCE_UNIQUE, "true");

    if (ref->isCustom)
        metadata->SetStructField(VMF_NS, thisRef.c_str(), VMF_NS, REFERENCE_CUSTOM, "true");

    return;
}

void XMPSchemaSource::load(std::map<vmf::vmf_string, std::shared_ptr<vmf::MetadataSchema> >& schemas)
{
    SXMPIterator schemasIterator(*metadata, VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, kXMP_IterJustChildren);
    vmf_string currentSchemaPath;
    while (schemasIterator.Next(nullptr, &currentSchemaPath))
    {
        shared_ptr<MetadataSchema> schema = loadMetadataSchemaByPath(currentSchemaPath);
        if (schema != nullptr)
        {
            schemas[schema->getName()] = schema;
        }
    }
}

shared_ptr<MetadataSchema> XMPSchemaSource::loadMetadataSchemaByPath(const vmf_string& pathToSchema)
{
    vmf_string thisSchemaName, thisSchemaAuthor;
    if(!metadata->GetStructField(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_NAME, &thisSchemaName, nullptr))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted schema description by path " + pathToSchema);
    }
    metadata->GetStructField(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_AUTHOR, &thisSchemaAuthor, nullptr);
    vmf_string thisSchemaEncrypted;
    bool useEncryption = metadata->GetStructField(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_ENCRYPTED, &thisSchemaEncrypted, nullptr);
    useEncryption = useEncryption && (thisSchemaEncrypted == "true");
    auto schema = make_shared<MetadataSchema>(thisSchemaName, thisSchemaAuthor, useEncryption);

    vmf_string thisSchemaDescriptors;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSchema.c_str(), VMF_NS, SCHEMA_PROPERTIES, &thisSchemaDescriptors);

    SXMPIterator propertiesIterator(*metadata, VMF_NS, thisSchemaDescriptors.c_str(), kXMP_IterJustChildren);
    vmf_string currentPropertyPath;
    while(propertiesIterator.Next(NULL, &currentPropertyPath))
    {
        shared_ptr<MetadataDesc> desc = loadDescription(currentPropertyPath);
        schema->add(desc);
    }
    return schema;
}

shared_ptr<MetadataDesc> XMPSchemaSource::loadDescription(const vmf_string& pathToDesc)
{
    vmf_string propertyName;
    if(!metadata->GetStructField(VMF_NS, pathToDesc.c_str(), VMF_NS, PROPERTY_NAME, &propertyName, NULL))
    {
        VMF_EXCEPTION(DataStorageException, "Corrupted property by path " + pathToDesc);
    }
    vmf_string encryptedDescStr;
    bool useEncryptionDesc = metadata->GetStructField(VMF_NS, pathToDesc.c_str(), VMF_NS,
                                                      PROPERTY_ENCRYPTED, &encryptedDescStr, NULL);
    useEncryptionDesc = useEncryptionDesc && encryptedDescStr == "true";

    vector<FieldDesc> fields;
    vmf_string pathToFields;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToDesc.c_str(), VMF_NS, PROPERTY_FIELDS, &pathToFields);
    SXMPIterator fieldsIterator(*metadata, VMF_NS, pathToFields.c_str(), kXMP_IterJustChildren);
    vmf_string currentFieldPath;
    while(fieldsIterator.Next(NULL, &currentFieldPath))
    {
        vmf_string name;
        if(!metadata->GetStructField(VMF_NS, currentFieldPath.c_str(), VMF_NS, FIELD_NAME, &name, NULL))
        {
            name = "";
        }

        vmf_string rawType;
        if(!metadata->GetStructField(VMF_NS, currentFieldPath.c_str(), VMF_NS, FIELD_TYPE, &rawType, NULL))
        {
            VMF_EXCEPTION(DataStorageException, "Corrupted field by path " + currentFieldPath);
        }

        Variant::Type type = Variant::typeFromString(rawType);

        bool optional = true;
        if(!metadata->GetStructField(VMF_NS, currentFieldPath.c_str(), VMF_NS, FIELD_OPTIONALITY, NULL, NULL))
            optional = false;

        vmf_string encryptedFieldStr;
        bool useEncryptionField = metadata->GetStructField(VMF_NS, currentFieldPath.c_str(), VMF_NS,
                                                           FIELD_ENCRYPTED, &encryptedFieldStr, NULL);
        useEncryptionField = useEncryptionField && encryptedFieldStr == "true";

        fields.push_back(FieldDesc(name, type, optional, useEncryptionField));
    }

    vector < shared_ptr<ReferenceDesc> > refs;
    vmf_string pathToRefs;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToDesc.c_str(), VMF_NS, PROPERTY_REFERENCES, &pathToRefs);

    SXMPIterator refsIterator(*metadata, VMF_NS, pathToRefs.c_str(), kXMP_IterJustChildren);
    vmf_string currentReferencePath;

    while (refsIterator.Next(NULL, &currentReferencePath))
    {
        vmf_string name;
        if (!metadata->GetStructField(VMF_NS, currentReferencePath.c_str(), VMF_NS, REFERENCE_NAME, &name, NULL))
            VMF_EXCEPTION(IncorrectParamException, "XMP element has invalid reference name.");

        bool isUnique = false;
        if (metadata->GetStructField(VMF_NS, currentReferencePath.c_str(), VMF_NS, REFERENCE_UNIQUE, NULL, NULL))
            isUnique = true;

        bool isCustom = false;
        if (metadata->GetStructField(VMF_NS, currentReferencePath.c_str(), VMF_NS, REFERENCE_CUSTOM, NULL, NULL))
            isCustom = true;

        refs.push_back(make_shared<ReferenceDesc>(name, isUnique, isCustom));
    }

    return shared_ptr<MetadataDesc>(new MetadataDesc(propertyName, fields, refs, useEncryptionDesc));
}

void XMPSchemaSource::clear()
{
    metadata->DeleteProperty(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
}

void XMPSchemaSource::remove(const vmf_string& schemaName)
{
    if(schemaName == "")
    {
        metadata->DeleteProperty(VMF_NS, VMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY);
        metadata->DeleteProperty(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
        metadata->DeleteProperty(VMF_NS, "next-id");
        metadata->DeleteProperty(kXMP_NS_XMP, "CreatorTool");
        return;
    }

    XMP_Index size = metadata->CountArrayItems(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        vmf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        vmf_string currentSchemaName;
        if (!metadata->GetStructField(VMF_NS, currentSchemaPath.c_str(), VMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            VMF_EXCEPTION(DataStorageException, "Corrputed schema by path" + currentSchemaPath);
        }
        if (currentSchemaName == schemaName)
        {
            metadata->DeleteArrayItem(VMF_NS, VMF_GLOBAL_SCHEMA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }

    size = metadata->CountArrayItems(VMF_NS, VMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY);
    for(XMP_Index i = 1; i <= size; ++i)
    {
        vmf_string currentSchemaPath;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY, i, &currentSchemaPath);
        vmf_string currentSchemaName;
        if (!metadata->GetStructField(VMF_NS, currentSchemaPath.c_str(), VMF_NS, SCHEMA_NAME, &currentSchemaName, NULL))
        {
            VMF_EXCEPTION(DataStorageException, "Corrputed metadata by path" + currentSchemaPath);
        }
        if (currentSchemaName == schemaName)
        {
            metadata->DeleteArrayItem(VMF_NS, VMF_GLOBAL_METADTATA_DESRIPTIONS_ARRAY, i);
            break;
        }
    }
}
