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
#include "vmf/format_xml.hpp"
#include "vmf/format_const.hpp"

#include "libxml/tree.h"

namespace vmf
{

FormatXML::FormatXML()
{}

FormatXML::~FormatXML()
{}

/*
** store() support
*/

static void add(xmlNodePtr schemaNode, const std::shared_ptr<MetadataSchema>& spSchema)
{
    if (xmlNewProp(schemaNode, BAD_CAST ATTR_NAME, BAD_CAST spSchema->getName().c_str()) == NULL)
        VMF_EXCEPTION(Exception, "Can't create xmlNode property (schema name)");

    if (xmlNewProp(schemaNode, BAD_CAST ATTR_SCHEMA_AUTHOR, BAD_CAST spSchema->getAuthor().c_str()) == NULL)
        VMF_EXCEPTION(Exception, "Can't create xmlNode property (schema author)");

    auto vDescs = spSchema->getAll();
    for (auto spDescriptor = vDescs.begin(); spDescriptor != vDescs.end(); spDescriptor++)
    {
        xmlNodePtr descNode = xmlNewChild(schemaNode, NULL, BAD_CAST TAG_DESCRIPTION, NULL);
        if (descNode == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode for description");

        if (xmlNewProp(descNode, BAD_CAST ATTR_NAME, BAD_CAST spDescriptor->get()->getMetadataName().c_str()) == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode property (description name)");

        auto vFields = spDescriptor->get()->getFields();
        for (auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
        {
            xmlNodePtr fieldNode = xmlNewChild(descNode, NULL, BAD_CAST TAG_FIELD, NULL);
            if (fieldNode == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode for field");

            if (xmlNewProp(fieldNode, BAD_CAST ATTR_NAME, BAD_CAST fieldDesc->name.c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (field name)");

            if (xmlNewProp(fieldNode, BAD_CAST ATTR_FIELD_TYPE, BAD_CAST Variant::typeToString(fieldDesc->type).c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (field type)");

            if (fieldDesc->optional)
                if (xmlNewProp(fieldNode, BAD_CAST ATTR_FIELD_OPTIONAL, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(Exception, "Can't create xmlNode property (field is optional)");
        }

        auto vRefs = (*spDescriptor)->getAllReferenceDescs();
        for (auto refDesc = vRefs.begin(); refDesc != vRefs.end(); refDesc++)
        {
            if ((*refDesc)->name.empty())
                continue;

            xmlNodePtr refNode = xmlNewChild(descNode, NULL, BAD_CAST TAG_METADATA_REFERENCE, NULL);
            if (refNode == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode for reference.");

            if (xmlNewProp(refNode, BAD_CAST ATTR_NAME, BAD_CAST(*refDesc)->name.c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (reference name)");

            if ((*refDesc)->isUnique)
                if (xmlNewProp(refNode, BAD_CAST ATTR_REFERENCE_UNIQUE, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(Exception, "Can't create xmlNode property (reference is unique)");

            if ((*refDesc)->isCustom)
                if (xmlNewProp(refNode, BAD_CAST ATTR_REFERENCE_CUSTOM, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(Exception, "Can't create xmlNode property (reference is custom)");
        }
    }
}


static void add(xmlNodePtr metadataNode, const std::shared_ptr<Metadata>& spMetadata)
{
    if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_SCHEMA, BAD_CAST spMetadata->getSchemaName().c_str()) == NULL)
        VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata schema name)");

    if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_DESCRIPTION, BAD_CAST spMetadata->getName().c_str()) == NULL)
        VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata description name)");

    if (xmlNewProp(metadataNode, BAD_CAST ATTR_ID, BAD_CAST to_string(spMetadata->getId()).c_str()) == NULL)
        VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata id)");

    if (spMetadata->getFrameIndex() != Metadata::UNDEFINED_FRAME_INDEX)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_FRAME_IDX, BAD_CAST to_string(spMetadata->getFrameIndex()).c_str()) == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata frame index)");

    if (spMetadata->getNumOfFrames() != Metadata::UNDEFINED_FRAMES_NUMBER)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_NFRAMES, BAD_CAST to_string(spMetadata->getNumOfFrames()).c_str()) == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata number of frames)");

    if (spMetadata->getTime() != Metadata::UNDEFINED_TIMESTAMP)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_TIMESTAMP, BAD_CAST to_string(spMetadata->getTime()).c_str()) == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata timestamp)");

    if (spMetadata->getDuration() != Metadata::UNDEFINED_DURATION)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_DURATION, BAD_CAST to_string(spMetadata->getDuration()).c_str()) == NULL)
            VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata duration)");

    auto vFields = spMetadata->getDesc()->getFields();
    for (auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
    {
        Variant val = spMetadata->getFieldValue(fieldDesc->name);
        if (!val.isEmpty())
        {
            xmlNodePtr metadataFieldNode = xmlNewChild(metadataNode, NULL, BAD_CAST TAG_FIELD, NULL);
            if (metadataFieldNode == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode for metadata field");

            if (xmlNewProp(metadataFieldNode, BAD_CAST ATTR_NAME, BAD_CAST fieldDesc->name.c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata field name)");

            if (xmlNewProp(metadataFieldNode, BAD_CAST ATTR_VALUE, BAD_CAST val.toString().c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata field value)");
        }
    }

    auto refs = spMetadata->getAllReferences();
    if (!refs.empty())
    {
        for (auto reference = refs.begin(); reference != refs.end(); reference++)
        {
            xmlNodePtr referenceNode = xmlNewChild(metadataNode, NULL, BAD_CAST TAG_METADATA_REFERENCE, NULL);
            if (referenceNode == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode for metadata reference");

            if (xmlNewProp(referenceNode, BAD_CAST ATTR_NAME, BAD_CAST reference->getReferenceDescription()->name.c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata reference name)");

            if (xmlNewProp(referenceNode, BAD_CAST ATTR_ID, BAD_CAST to_string((*reference).getReferenceMetadata().lock()->getId()).c_str()) == NULL)
                VMF_EXCEPTION(Exception, "Can't create xmlNode property (metadata reference id)");
        }
    }
}

static void add(xmlNodePtr segNode, const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    if (spSegment->getTitle() == "" || spSegment->getFPS() <= 0 || spSegment->getTime() < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid segment. Segment must have not empty title, fps > 0 and start time >= 0");

    if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_TITLE, BAD_CAST spSegment->getTitle().c_str()) == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_FPS, BAD_CAST to_string(spSegment->getFPS()).c_str()) == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_TIME, BAD_CAST to_string(spSegment->getTime()).c_str()) == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (spSegment->getDuration() > 0)
        if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_DURATION, BAD_CAST to_string(spSegment->getDuration()).c_str()) == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");

    long width, height;
    spSegment->getResolution(width, height);
    if (width > 0 && height > 0)
    {
        if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_WIDTH, BAD_CAST to_string(width).c_str()) == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");

        if (xmlNewProp(segNode, BAD_CAST ATTR_SEGMENT_HEIGHT, BAD_CAST to_string(height).c_str()) == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property (video segment title)");
    }
}

/*
static void add(xmlNodePtr statNode, const Stat& stat)
{
VMF_EXCEPTION(NotImplementedException, "Stat -> XML NYI");
}
*/

std::string FormatXML::store(
    const MetadataSet& set,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    //const std::vector<Stat>& stats,
    const AttribMap& attribs
    )
{
    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr vmfRootNode = xmlNewNode(NULL, BAD_CAST TAG_VMF);
    if (vmfRootNode == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode for vmf root element");
    if (xmlDocSetRootElement(doc, vmfRootNode) != 0)
        VMF_EXCEPTION(InternalErrorException, "Can't set root element to the document");

    // attribs
    for (const auto& a : attribs)
        if (xmlNewProp(vmfRootNode, BAD_CAST a.first.c_str(), BAD_CAST a.second.c_str()) == NULL)
        {
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode property. Next Id");
        }

    // stats
    /*
    if (!stats.empty())
    {
        xmlNodePtr statsArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_STATS_ARRAY, NULL);
        if (statsArrayNode == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode for Stat objects array");
        for (const auto& st : stats)
        {
            xmlNodePtr statNode = xmlNewChild(statsArrayNode, NULL, BAD_CAST TAG_STAT_OBJ, NULL);
            add(statNode, st);
        }
    }
    */

    // segments
    if (!segments.empty())
    {
        xmlNodePtr segmentsArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_VIDEO_SEGMENTS_ARRAY, NULL);
        if (segmentsArrayNode == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode for video segments array");
        for(const auto& seg : segments)
        {
            if (seg == nullptr)
                VMF_EXCEPTION(NullPointerException, "Video Segment pointer is null");
            xmlNodePtr segmentNode = xmlNewChild(segmentsArrayNode, NULL, BAD_CAST TAG_VIDEO_SEGMENT, NULL);
            add(segmentNode, seg);
        }
    }

    // schemas
    if (!schemas.empty())
    {
        xmlNodePtr schemasArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_SCHEMAS_ARRAY, NULL);
        if (schemasArrayNode == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode for schemas array");
        for(const auto& sc : schemas)
        {
            if (sc== nullptr)
                VMF_EXCEPTION(NullPointerException, "Schema pointer is null");
            xmlNodePtr schemaNode = xmlNewChild(schemasArrayNode, NULL, BAD_CAST TAG_SCHEMA, NULL);
            add(schemaNode, sc);
        }
    }

    // set
    if (!set.empty())
    {
        xmlNodePtr metadataArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_METADATA_ARRAY, NULL);
        if (metadataArrayNode == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create xmlNode for metadata array");
        for(const auto& md : set)
        {
            if (md == nullptr)
                VMF_EXCEPTION(NullPointerException, "Metadata pointer is null");
            xmlNodePtr metadataNode = xmlNewChild(metadataArrayNode, NULL, BAD_CAST TAG_METADATA, NULL);
            add(metadataNode, md);
        }
    }

    xmlChar *buf = nullptr;
    int size = 0;
    xmlKeepBlanksDefault(0);
    xmlDocDumpFormatMemory(doc, &buf, &size, 1);
    if (buf == nullptr || size == 0)
        VMF_EXCEPTION(InternalErrorException, "Can't save xmlDoc into a temp buffer");

    std::string outputString( (char *)buf );

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

/*
** parse() support
*/

    static std::shared_ptr<MetadataSchema> parseSchemaFromNode(xmlNodePtr schemaNode)
    {
        std::shared_ptr<vmf::MetadataSchema> spSchema;
        std::shared_ptr<vmf::MetadataDesc> spDesc;
        std::string schema_name, schema_author;
        for (xmlAttrPtr cur_prop = schemaNode->properties; cur_prop; cur_prop = cur_prop->next)
        {
            if (std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                schema_name = (char*)xmlGetProp(schemaNode, cur_prop->name);
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SCHEMA_AUTHOR))
                schema_author = (char*)xmlGetProp(schemaNode, cur_prop->name);
        }
        spSchema = std::make_shared<vmf::MetadataSchema>(schema_name, schema_author);

        for (xmlNodePtr descNode = schemaNode->children; descNode; descNode = descNode->next)
        {
            if (descNode->type == XML_ELEMENT_NODE && (char*)descNode->name == std::string(TAG_DESCRIPTION))
            {
                std::string desc_name;
                for (xmlAttrPtr cur_prop = descNode->properties; cur_prop; cur_prop = cur_prop->next)
                    if (std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                        desc_name = (char*)xmlGetProp(descNode, cur_prop->name);

                std::vector<FieldDesc> vFields;
                std::vector<std::shared_ptr<ReferenceDesc>> vReferences;
                for (xmlNodePtr fieldNode = descNode->children; fieldNode; fieldNode = fieldNode->next)
                {
                    if (fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_FIELD))
                    {
                        std::string field_name;
                        vmf::Variant::Type field_type = vmf::Variant::type_unknown;
                        bool field_optional = false;
                        for (xmlAttrPtr cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next) //fill field's attributes
                        {
                            if (std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                                field_name = (char*)xmlGetProp(fieldNode, cur_prop->name);
                            if (std::string((char*)cur_prop->name) == std::string(ATTR_FIELD_TYPE))
                            {
                                std::string sFieldType = (char*)xmlGetProp(fieldNode, cur_prop->name);
                                field_type = vmf::Variant::typeFromString(sFieldType);
                            }
                            if (std::string((char*)cur_prop->name) == std::string(ATTR_FIELD_OPTIONAL))
                            {
                                if (std::string((char*)xmlGetProp(fieldNode, cur_prop->name)) == "true")
                                    field_optional = true;
                                else if (std::string((char*)xmlGetProp(fieldNode, cur_prop->name)) == "false")
                                    field_optional = false;
                                else
                                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'optional'");
                            }
                        }
                        vFields.push_back(FieldDesc(field_name, field_type, field_optional));
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

                spDesc = std::make_shared<vmf::MetadataDesc>(desc_name, vFields, vReferences);
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

    static MetadataInternal parseMetadataFromNode(xmlNodePtr metadataNode)
    {
        std::string schema_name, desc_name;
        long long frameIndex = vmf::Metadata::UNDEFINED_FRAME_INDEX, nFrames = vmf::Metadata::UNDEFINED_FRAMES_NUMBER,
            timestamp = vmf::Metadata::UNDEFINED_TIMESTAMP, duration = vmf::Metadata::UNDEFINED_DURATION, id = INVALID_ID;

        for (xmlAttr* cur_prop = metadataNode->properties; cur_prop; cur_prop = cur_prop->next)
        {
            if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_SCHEMA))
                schema_name = (char*)xmlGetProp(metadataNode, cur_prop->name);
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_DESCRIPTION))
                desc_name = (char*)xmlGetProp(metadataNode, cur_prop->name);
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_FRAME_IDX))
                frameIndex = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_NFRAMES))
                nFrames = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_TIMESTAMP))
                timestamp = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_METADATA_DURATION))
                duration = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_ID))
                id = ATOLL((char*)xmlGetProp(metadataNode, cur_prop->name));
        }

        MetadataInternal mdi(desc_name, schema_name);
        mdi.id = id;

        if (frameIndex != vmf::Metadata::UNDEFINED_FRAME_INDEX)
        {
            mdi.frameIndex = frameIndex;
            if (nFrames != vmf::Metadata::UNDEFINED_FRAMES_NUMBER)
                mdi.frameNum = nFrames;
        }
        if (timestamp != vmf::Metadata::UNDEFINED_TIMESTAMP)
        {
            mdi.timestamp = timestamp;
            if (duration != vmf::Metadata::UNDEFINED_DURATION)
                mdi.duration = duration;
        }

        for (xmlNode *fieldNode = metadataNode->children; fieldNode; fieldNode = fieldNode->next)
        {
            if (fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_FIELD))
            {
                std::string fieldName, fieldValue;
                for (xmlAttr* cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next)
                {
                    if (std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                        fieldName = (char*)xmlGetProp(fieldNode, cur_prop->name);
                    else if (std::string((char*)cur_prop->name) == std::string(ATTR_VALUE))
                        fieldValue = (char*)xmlGetProp(fieldNode, cur_prop->name);
                }
                mdi.fields[fieldName] = fieldValue;
            }
            else if (fieldNode->type == XML_ELEMENT_NODE && (char*)fieldNode->name == std::string(TAG_METADATA_REFERENCE))
            {
                std::string refName;
                long refId = -1;
                for (xmlAttr* cur_prop = fieldNode->properties; cur_prop; cur_prop = cur_prop->next)
                {
                    if (std::string((char*)cur_prop->name) == std::string(ATTR_ID))
                        refId = atol((char*)xmlGetProp(fieldNode, cur_prop->name));
                    else if (std::string((char*)cur_prop->name) == std::string(ATTR_NAME))
                        refName = (char*)xmlGetProp(fieldNode, cur_prop->name);
                }
                mdi.refs.push_back(std::make_pair(IdType(refId), refName));
            }
        }
        return mdi;
    }

    static std::shared_ptr<MetadataStream::VideoSegment> parseSegmentFromNode(xmlNodePtr segmentNode)
    {
        std::string title;
        double fps = 0;
        long long timestamp = -1, duration = 0;
        long width = 0, height = 0;
        for (xmlAttr* cur_prop = segmentNode->properties; cur_prop; cur_prop = cur_prop->next)
        {
            if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_TITLE))
                title = (char*)xmlGetProp(segmentNode, cur_prop->name);
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_FPS))
                fps = atof((char*)xmlGetProp(segmentNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_TIME))
                timestamp = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_DURATION))
                duration = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_WIDTH))
                width = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
            else if (std::string((char*)cur_prop->name) == std::string(ATTR_SEGMENT_HEIGHT))
                height = atol((char*)xmlGetProp(segmentNode, cur_prop->name));
        }

        if (title.empty())
            VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid title");
        if (fps <= 0)
            VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid fps value");
        if (timestamp < 0)
            VMF_EXCEPTION(vmf::InternalErrorException, "XML element has invalid time value");

        std::shared_ptr<MetadataStream::VideoSegment> spSegment(new MetadataStream::VideoSegment(title, fps, timestamp));
        if (duration > 0)
            spSegment->setDuration(duration);
        if (width > 0 && height > 0)
            spSegment->setResolution(width, height);

        return spSegment;
    }

    Format::ParseCounters FormatXML::parse(
        const std::string& text,
        std::vector<MetadataInternal>& metadata,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        //std::vector<Stat>& stats,
        AttribMap& attribs // nextId, checksum, etc
        )
    {
        Format::ParseCounters cnt = {};

        if (text.empty())
            VMF_EXCEPTION(IncorrectParamException, "Empty input XML string");

        xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
        if (ctxt == NULL)
            VMF_EXCEPTION(InternalErrorException, "Failed to allocate XML parser context");

        xmlDocPtr doc = xmlCtxtReadMemory(ctxt, text.c_str(), (int)text.size(), NULL, NULL, 0);
        if (doc == NULL)
            VMF_EXCEPTION(InternalErrorException, "Can't create XML document");

        xmlNodePtr root = xmlDocGetRootElement(doc);
        if (root == NULL)
            VMF_EXCEPTION(InternalErrorException, "XML tree has no root element");

        if ((char*)root->name == std::string(TAG_VMF))
        {
            for (xmlAttr* cur_prop = root->properties; cur_prop; cur_prop = cur_prop->next)
            {
                attribs[(char*)cur_prop->name] = (char*)xmlGetProp(root, cur_prop->name);
                cnt.attribs++;
            }

            for (xmlNodePtr node = root->children; node; node = node->next)
            {
                if (node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_STATS_ARRAY))
                {
                    /*
                    for (xmlNodePtr st = node->children; st; st = st->next)
                    {
                        if (st->type == XML_ELEMENT_NODE && (char*)st->name == std::string(TAG_STAT_OBJ))
                        {
                            try
                            {
                                Stat stat = parseStatFromNode(st);
                                stats.push_back(stat);
                                cnt.stats++;
                            }
                            catch (Exception& e)
                            {
                                VMF_LOG_ERROR("Exception parsing Stat object: %s", e.what());
                            }
                        }
                    }
                    */
                }
                else if (node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_VIDEO_SEGMENTS_ARRAY))
                {
                    for (xmlNodePtr seg = node->children; seg; seg = seg->next)
                    {
                        if (seg->type == XML_ELEMENT_NODE && (char*)seg->name == std::string(TAG_VIDEO_SEGMENT))
                        {
                            try
                            {
                                std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseSegmentFromNode(seg);
                                segments.push_back(spSegment);
                                cnt.segments++;
                            }
                            catch (Exception& e)
                            {
                                VMF_LOG_ERROR("Exception parsing segment: %s", e.what());
                            }
                        }
                    }
                }
                else if (node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_SCHEMAS_ARRAY))
                {
                    for (xmlNodePtr sc = node->children; sc; sc = sc->next)
                    {
                        if (sc->type == XML_ELEMENT_NODE && (char*)sc->name == std::string(TAG_SCHEMA))
                        {
                            try
                            {
                                std::shared_ptr<MetadataSchema> spSchema = parseSchemaFromNode(sc);
                                schemas.push_back(spSchema);
                                cnt.schemas++;
                            }
                            catch (Exception& e)
                            {
                                VMF_LOG_ERROR("Exception parsing schema: %s", e.what());
                            }
                        }
                    }
                }
                else if (node->type == XML_ELEMENT_NODE && (char*)node->name == std::string(TAG_METADATA_ARRAY))
                {
                    for (xmlNodePtr md = node->children; md; md = md->next)
                    {
                        if (md->type == XML_ELEMENT_NODE && (char*)md->name == std::string(TAG_METADATA))
                        {
                            try
                            {
                                metadata.push_back( parseMetadataFromNode(md) );
                                cnt.metadata++;
                            }
                            catch (Exception& e)
                            {
                                VMF_LOG_ERROR("Exception parsing metadata: %s", e.what());
                            }
                        }
                    }
                }
                else
                {
                    VMF_LOG_ERROR("Unknown XML element: %s", (char*)node->name);
                }
            }
        }
        else
        {
            VMF_EXCEPTION(IncorrectParamException, "Invalid XML document format. Root element of the XMLTree is not the <vmf> tag element");
        }

        xmlFreeDoc(doc);
        xmlFreeParserCtxt(ctxt);
        xmlCleanupParser();
        xmlMemoryDump();

        return cnt;
    }

}//vmf
