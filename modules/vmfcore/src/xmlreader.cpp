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
#include "vmf/xmlreader.hpp"
#include "vmf/rwconst.hpp"

#include "libxml/tree.h"

namespace vmf
{

static std::shared_ptr<MetadataSchema> parseSchemaFromNode(xmlNodePtr schemaNode)
{
    std::shared_ptr<vmf::MetadataSchema> spSchema;
    std::shared_ptr<vmf::MetadataDesc> spDesc;
    std::string schema_name, schema_author;
    bool schemaUseEncryption = false;
    for(xmlAttrPtr cur_prop = schemaNode->properties; cur_prop; cur_prop = cur_prop->next)
    {
        if(std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
            schema_name = (char*)xmlGetProp(schemaNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SCHEMA_AUTHOR))
            schema_author = (char*)xmlGetProp(schemaNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_BOOL))
        {
            std::string encBool = (char*)xmlGetProp(schemaNode, cur_prop->name);
            schemaUseEncryption = encBool == "true";
        }
    }
    spSchema = std::make_shared<vmf::MetadataSchema>(schema_name, schema_author, schemaUseEncryption);

    for(xmlNodePtr descNode = schemaNode->children; descNode; descNode = descNode->next)
    {
        if (descNode->type == XML_ELEMENT_NODE && (char*)descNode->name == std::string(TAG_DESCRIPTION))
        {
            std::string desc_name;
            bool descUseEncryption = false;
            for(xmlAttrPtr cur_prop = descNode->properties; cur_prop; cur_prop = cur_prop->next)
            {
                if(std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                    desc_name = (char*)xmlGetProp(descNode, cur_prop->name);
                else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_BOOL))
                {
                    std::string encBool = (char*)xmlGetProp(descNode, cur_prop->name);
                    descUseEncryption = encBool == "true";
                }
            }

            std::vector<FieldDesc> vFields;
            std::vector<std::shared_ptr<ReferenceDesc>> vReferences;
            for(xmlNodePtr fieldNode = descNode->children; fieldNode; fieldNode = fieldNode->next)
            {
                if (fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_FIELD))
                {
                    std::string field_name;
                    vmf::Variant::Type field_type = vmf::Variant::type_unknown;
                    bool field_optional = false;
                    bool fieldUseEncryption = false;
                    for(xmlAttrPtr cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next) //fill field's attributes
                    {
                        if(std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                            field_name = (char*)xmlGetProp(fieldNode, cur_prop->name);
                        if(std::string((char*)cur_prop->name) == std::string(ATTR_FIELD_TYPE))
                        {
                            std::string sFieldType = (char*)xmlGetProp(fieldNode, cur_prop->name);
                            field_type = vmf::Variant::typeFromString(sFieldType);
                        }
                        if(std::string((char*)cur_prop->name) == std::string(ATTR_FIELD_OPTIONAL))
                        {
                            if( std::string((char*)xmlGetProp(fieldNode, cur_prop->name)) == "true" )
                                field_optional = true;
                            else if( std::string((char*)xmlGetProp(fieldNode, cur_prop->name)) == "false" )
                                field_optional = false;
                            else
                                VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'optional'");
                        }
                        if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_BOOL))
                        {
                            std::string encBool = (char*)xmlGetProp(fieldNode, cur_prop->name);
                            fieldUseEncryption = encBool == "true";
                        }
                    }
                    vFields.push_back(FieldDesc(field_name, field_type, field_optional, fieldUseEncryption));
                }
                else if (fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_METADATA_REFERENCE))
                {
                    std::string reference_name;
                    bool isUnique = false;
                    bool isCustom = false;
                    for (xmlAttrPtr cur_ref = fieldNode->properties; cur_ref; cur_ref = cur_ref->next)
                    {
                        if (std::string((char*)cur_ref->name) == std::string(ATTR_NAME))
                            reference_name = (char*)xmlGetProp(fieldNode, cur_ref->name);
                        if (std::string((char*)cur_ref->name) == std::string(ATTR_REFERENCE_UNIQUE))
                        {
                            if (std::string((char*)xmlGetProp(fieldNode, cur_ref->name)) == "true")
                                isUnique = true;
                            else if (std::string((char*)xmlGetProp(fieldNode, cur_ref->name)) == "false")
                                isUnique = false;
                            else
                                VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'unique'");
                        }
                        if (std::string((char*)cur_ref->name) == std::string(ATTR_REFERENCE_CUSTOM))
                        {
                            if (std::string((char*)xmlGetProp(fieldNode, cur_ref->name)) == "true")
                                isCustom = true;
                            else if (std::string((char*)xmlGetProp(fieldNode, cur_ref->name)) == "false")
                                isCustom = false;
                            else
                                VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'custom'");
                        }
                    }
                    vReferences.emplace_back(std::make_shared<ReferenceDesc>(reference_name, isUnique, isCustom));
                }

            }

            spDesc = std::make_shared<vmf::MetadataDesc>(desc_name, vFields, vReferences, descUseEncryption);
            spSchema->add(spDesc);
        }
    }
    return spSchema;
}

#if defined _MSC_VER && _MSC_VER < 1800
    #define ATOLL(x) _atoi64(x)
#else
    #define ATOLL(x) atoll(x)
#endif

static std::shared_ptr<MetadataInternal> parseMetadataFromNode(xmlNodePtr metadataNode, const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    std::string schema_name, desc_name;
    long long frameIndex = vmf::Metadata::UNDEFINED_FRAME_INDEX, nFrames = vmf::Metadata::UNDEFINED_FRAMES_NUMBER,
        timestamp = vmf::Metadata::UNDEFINED_TIMESTAMP, duration = vmf::Metadata::UNDEFINED_DURATION, id = INVALID_ID;
    std::string encryptedMetadata;
    bool metadataUseEncryption = false;
    for(xmlAttr* cur_prop = metadataNode->properties; cur_prop; cur_prop = cur_prop->next)
    {
        if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_SCHEMA))
            schema_name = (char*)xmlGetProp(metadataNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_DESCRIPTION))
            desc_name = (char*)xmlGetProp(metadataNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_FRAME_IDX))
            frameIndex = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_NFRAMES))
            nFrames = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_TIMESTAMP))
            timestamp = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_DURATION))
            duration = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_ID))
            id = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_DATA))
            encryptedMetadata = (char*)xmlGetProp(metadataNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_BOOL))
        {
            std::string encBool = (char*)xmlGetProp(metadataNode, cur_prop->name);
            metadataUseEncryption = encBool == "true";
        }
    }

    if(metadataUseEncryption && encryptedMetadata.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "No encrypted data presented while the flag is set on");

    if(id == INVALID_ID)
        VMF_EXCEPTION(vmf::InternalErrorException, "XML element has no id");

    auto schema = schemas.begin();
    for(; schema != schemas.end(); schema++)
        if((*schema)->getName() == schema_name)
            break;

    if(schema == schemas.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Unknown metadata item schema");

    std::shared_ptr<MetadataDesc> spDesc;
    if( (spDesc = (*schema)->findMetadataDesc(desc_name)) == nullptr)
        VMF_EXCEPTION(vmf::IncorrectParamException, "Unknown metadata item description");

    std::shared_ptr<MetadataInternal> spMetadataInternal(new MetadataInternal(spDesc));
    spMetadataInternal->setId(id);
    spMetadataInternal->setUseEncryption(metadataUseEncryption);
    spMetadataInternal->setEncryptedData(encryptedMetadata);

    if(frameIndex != vmf::Metadata::UNDEFINED_FRAME_INDEX)
    {
        if(nFrames != vmf::Metadata::UNDEFINED_FRAMES_NUMBER)
            spMetadataInternal->setFrameIndex(frameIndex, nFrames);
        else
            spMetadataInternal->setFrameIndex(frameIndex);
    }
    if(timestamp != vmf::Metadata::UNDEFINED_TIMESTAMP)
    {
        if(duration != vmf::Metadata::UNDEFINED_DURATION)
            spMetadataInternal->setTimestamp(timestamp, duration);
        else
            spMetadataInternal->setTimestamp(timestamp);
    }

    for(xmlNode *fieldNode = metadataNode->children; fieldNode; fieldNode = fieldNode->next)
    {
        if(fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_FIELD))
        {
            std::string field_name;
            vmf::Variant field_value;
            std::string field_encrypted_data;
            bool field_use_encryption = false;
            for(xmlAttr* cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next)
            {
                if(std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                    field_name = (char*)xmlGetProp(fieldNode, cur_prop->name);
                else if(std::string((char*)cur_prop->name) == std::string(ATTR_VALUE))
                {
                    FieldDesc fieldDesc;
                    spDesc->getFieldDesc(fieldDesc, field_name);
                    field_value.fromString(fieldDesc.type, (char*)xmlGetProp(fieldNode, cur_prop->name));
                }
                else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_BOOL))
                {
                    std::string encBool = (char*)xmlGetProp(fieldNode, cur_prop->name);
                    field_use_encryption = encBool == "true";
                }
                else if(std::string((char*)cur_prop->name) == std::string(ATTR_ENCRYPTED_DATA))
                {
                    field_encrypted_data = (char*)xmlGetProp(fieldNode, cur_prop->name);
                }
                if(field_use_encryption && field_encrypted_data.empty())
                    VMF_EXCEPTION(vmf::IncorrectParamException,
                                  "No encrypted data presented while the flag is set on");
            }
            FieldValue& fv = *(spMetadataInternal->findField(field_name));
            fv = FieldValue(field_name, field_value, field_use_encryption);
            fv.setEncryptedData(field_encrypted_data);
        }
        else if(fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_METADATA_REFERENCE))
        {
            std::string refName;
            long refId = -1;
            for(xmlAttr* cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next)
            {
                if(std::string((char*)cur_prop->name) == std::string(ATTR_ID))
                    refId = atol((char*)xmlGetProp(fieldNode, cur_prop->name));
                else if(std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                    refName = (char*)xmlGetProp(fieldNode, cur_prop->name);
            }
            spMetadataInternal->vRefs.push_back(std::make_pair(IdType(refId), refName));
        }
    }
    return spMetadataInternal;
}

static std::shared_ptr<MetadataStream::VideoSegment> parseSegmentFromNode(xmlNodePtr segmentNode)
{
    std::string title;
    double fps = 0;
    long long timestamp = -1, duration = 0;
    long width = 0, height = 0;
    for(xmlAttr* cur_prop = segmentNode->properties; cur_prop; cur_prop = cur_prop->next)
    {
        if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_TITLE))
            title = (char*)xmlGetProp(segmentNode, cur_prop->name);
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_FPS))
            fps = atof((char*)xmlGetProp(segmentNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_TIME))
            timestamp = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_DURATION))
            duration = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_WIDTH))
            width = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
        else if(std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_HEIGHT))
            height = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
    }

    if(title.empty())
	VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid title");
    if(fps <= 0)
	VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid fps value");
    if(timestamp < 0)
	VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid time value");

    std::shared_ptr<MetadataStream::VideoSegment> spSegment(new MetadataStream::VideoSegment(title, fps, timestamp));
    if(duration > 0)
	spSegment->setDuration(duration);
    if(width > 0 && height > 0)
	spSegment->setResolution(width, height);

    return spSegment;
}


XMLReader::XMLReader() : IReader() { }
XMLReader::~XMLReader(){}


//this version of parseSchemas always gets uncompressed text as input
bool XMLReader::parseSchemas(const std::string& text, std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input XML string");
        return false;
    }

    schemas.clear();

    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
    {
        VMF_LOG_ERROR("Failed to allocate XML parser context");
        return false;
    }
    xmlDocPtr doc = xmlCtxtReadMemory(ctxt, text.c_str(), (int)text.size(), NULL, NULL, 0);

    //xmlDocPtr doc = xmlParseMemory(text.c_str(), (int)text.size());
    if(doc == NULL)
    {
        VMF_LOG_ERROR("Can't create XML document");
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        VMF_LOG_ERROR("XML tree has no root element");
        return false;
    }

    if( (char*)root->name == std::string(TAG_SCHEMA) )
    {
        try
        {
            std::shared_ptr<MetadataSchema> spSchema = parseSchemaFromNode(root);
            schemas.push_back(spSchema);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( (char*)root->name == std::string(TAG_SCHEMAS_ARRAY) )
    {
        for(xmlNodePtr node = root->children; node; node = node->next)
            if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_SCHEMA))
            {
                try
                {
                    std::shared_ptr<MetadataSchema> spSchema = parseSchemaFromNode(node);
                    schemas.push_back(spSchema);
                }
                catch(Exception& e)
                {
                    VMF_LOG_ERROR("Exception: %s", e.what());
                    return false;
                }
            }
    }
    else if( (char*)root->name == std::string(TAG_VMF) )
    {
        for(xmlNodePtr rootChildNode = root->children; rootChildNode; rootChildNode = rootChildNode->next)
            if(rootChildNode->type == XML_ELEMENT_NODE && (char*)rootChildNode->name == std::string(TAG_SCHEMAS_ARRAY))
                for(xmlNodePtr node = rootChildNode->children; node; node = node->next)
                    if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_SCHEMA))
                    {
                        try
                        {
                            std::shared_ptr<MetadataSchema> spSchema = parseSchemaFromNode(node);
                            schemas.push_back(spSchema);
                        }
                        catch(Exception& e)
                        {
                            VMF_LOG_ERROR("Exception: %s", e.what());
                            return false;
                        }
                    }
    }

    xmlFreeDoc(doc);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();

    return true;
}


//this version of parseMetadata always gets uncompressed input
bool XMLReader::parseMetadata(const std::string& text,
                              const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                              std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input XML string");
        return false;
    }

    metadata.clear();

    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
    {
        VMF_LOG_ERROR("Failed to allocate XML parser context");
        return false;
    }

    xmlDocPtr doc = xmlCtxtReadMemory(ctxt, text.c_str(), (int)text.size(), NULL, NULL, 0);
    if(doc == NULL)
    {
        VMF_LOG_ERROR("Can't create XML document");
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        VMF_LOG_ERROR("XML tree has no root element");
        return false;
    }

    if( (char*)root->name == std::string(TAG_METADATA) )
    {
        try
        {
            std::shared_ptr<MetadataInternal> spMetadata = parseMetadataFromNode(root, schemas);
            metadata.push_back(spMetadata);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( (char*)root->name == std::string(TAG_METADATA_ARRAY) )
    {
        for(xmlNodePtr node = root->children; node; node = node->next)
            if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_METADATA))
            {
                try
                {
                    std::shared_ptr<MetadataInternal> spMetadata = parseMetadataFromNode(node, schemas);
                    metadata.push_back(spMetadata);
                }
                catch(Exception& e)
                {
                    VMF_LOG_ERROR("Exception: %s", e.what());
                    return false;
                }
            }
    }
    else if( (char*)root->name == std::string(TAG_VMF) )
    {
        for(xmlNodePtr rootChildNode = root->children; rootChildNode; rootChildNode = rootChildNode->next)
            if(rootChildNode->type == XML_ELEMENT_NODE && (char*)rootChildNode->name == std::string(TAG_METADATA_ARRAY))
                for(xmlNodePtr node = rootChildNode->children; node; node = node->next)
                    if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_METADATA))
                    {
                        try
                        {
                            std::shared_ptr<MetadataInternal> spMetadata = parseMetadataFromNode(node, schemas);
                            metadata.push_back(spMetadata);
                        }
                        catch(Exception& e)
                        {
                            VMF_LOG_ERROR("Exception: %s", e.what());
                            return false;
                        }
                    }
    }

    xmlFreeDoc(doc);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();

    return true;
}


//this version of parseAll always gets uncompressed text
bool XMLReader::parseAll(const std::string& text, IdType& nextId,
                         std::string& filepath, std::string& checksum, std::string& hint,
                         std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                         std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                         std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input XML string");
        return false;
    }

    schemas.clear();
    metadata.clear();

    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
    {
        VMF_LOG_ERROR("Failed to allocate XML parser context");
        return false;
    }

    xmlDocPtr doc = xmlCtxtReadMemory(ctxt, text.c_str(), (int)text.size(),
                                      NULL, NULL, 0);
    if(doc == NULL)
    {
        VMF_LOG_ERROR("Can't create XML document");
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        VMF_LOG_ERROR("XML tree has no root element");
        return false;
    }

    if( (char*)root->name == std::string(TAG_VMF) )
    {
        for(xmlAttr* cur_prop = root->properties; cur_prop; cur_prop = cur_prop->next)
        {
            if(std::string((char*)cur_prop->name) == std::string(ATTR_VMF_NEXTID))
                nextId = atol((char*)xmlGetProp(root, cur_prop->name));
            else if(std::string((char*)cur_prop->name) == std::string(ATTR_VMF_FILEPATH))
                filepath = (char*)xmlGetProp(root, cur_prop->name);
            else if(std::string((char*)cur_prop->name) == std::string(ATTR_VMF_CHECKSUM))
                checksum = (char*)xmlGetProp(root, cur_prop->name);
            else if(std::string((char*)cur_prop->name) == std::string(ATTR_VMF_HINT))
                hint = (char*)xmlGetProp(root, cur_prop->name);
        }
        if(!parseVideoSegments(text, segments))
            return false;
        if(!parseSchemas(text, schemas))
            return false;
        if(!parseMetadata(text, schemas, metadata))
            return false;
    }
    else
    {
        VMF_LOG_ERROR("Invalid XML document format. Root element of the XMLTree is not the <vmf> tag element");
        return false;
    }

    xmlFreeDoc(doc);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();

    return true;
}


//this version of parseVideoSegments always gets uncompressed text
bool XMLReader::parseVideoSegments(const std::string& text,
                                   std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input XML string");
        return false;
    }

    segments.clear();

    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
    {
        VMF_LOG_ERROR("Failed to allocate XML parser context");
        return false;
    }
    xmlDocPtr doc = xmlCtxtReadMemory(ctxt, text.c_str(), (int)text.size(), NULL, NULL, 0);

    //xmlDocPtr doc = xmlParseMemory(text.c_str(), (int)text.size());
    if(doc == NULL)
    {
        VMF_LOG_ERROR("Can't create XML document");
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        VMF_LOG_ERROR("XML tree has no root element");
        return false;
    }

    if( (char*)root->name == std::string(TAG_VIDEO_SEGMENT) )
    {
        try
        {
            std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseSegmentFromNode(root);
            segments.push_back(spSegment);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( (char*)root->name == std::string(TAG_VIDEO_SEGMENTS_ARRAY) )
    {
        for(xmlNodePtr node = root->children; node; node = node->next)
            if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_VIDEO_SEGMENT))
            {
                try
                {
                    std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseSegmentFromNode(node);
                    segments.push_back(spSegment);
                }
                catch(Exception& e)
                {
                    VMF_LOG_ERROR("Exception: %s", e.what());
                    return false;
                }
            }
    }
    else if( (char*)root->name == std::string(TAG_VMF) )
    {
        for(xmlNodePtr rootChildNode = root->children; rootChildNode; rootChildNode = rootChildNode->next)
            if(rootChildNode->type == XML_ELEMENT_NODE && (char*)rootChildNode->name == std::string(TAG_VIDEO_SEGMENTS_ARRAY))
                for(xmlNodePtr node = rootChildNode->children; node; node = node->next)
                    if(node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_VIDEO_SEGMENT))
                    {
                        try
                        {
                            std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseSegmentFromNode(node);
                            segments.push_back(spSegment);
                        }
                        catch(Exception& e)
                        {
                            VMF_LOG_ERROR("Exception: %s", e.what());
                            return false;
                        }
                    }
    }

    xmlFreeDoc(doc);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();

    return true;
}

}//vmf
