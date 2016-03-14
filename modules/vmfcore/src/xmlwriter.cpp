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
#include "vmf/xmlwriter.hpp"
#include "vmf/rwconst.hpp"

#include "libxml/tree.h"

#include <algorithm>

namespace vmf
{

static void add(xmlNodePtr schemaNode, const std::shared_ptr<MetadataSchema>& spSchema)
{
    if(xmlNewProp(schemaNode, BAD_CAST ATTR_NAME , BAD_CAST spSchema->getName().c_str()) == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (schema name)" );

    if(xmlNewProp(schemaNode, BAD_CAST ATTR_SCHEMA_AUTHOR , BAD_CAST spSchema->getAuthor().c_str()) == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (schema author)" );

    if(spSchema->getUseEncryption())
    {
        if(xmlNewProp(schemaNode, BAD_CAST ATTR_ENCRYPTED_BOOL, BAD_CAST "true") == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (is schema encrypted)" );
    }

    auto vDescs = spSchema->getAll();
    for( auto spDescriptor = vDescs.begin(); spDescriptor != vDescs.end(); spDescriptor++)
    {
        xmlNodePtr descNode = xmlNewChild(schemaNode, NULL, BAD_CAST TAG_DESCRIPTION, NULL);
        if(descNode == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for description" );

        if(xmlNewProp(descNode, BAD_CAST ATTR_NAME , BAD_CAST spDescriptor->get()->getMetadataName().c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (description name)" );

        if(spDescriptor->get()->getUseEncryption())
        {
            if(xmlNewProp(descNode, BAD_CAST ATTR_ENCRYPTED_BOOL, BAD_CAST "true") == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (is description encrypted)" );
        }

        auto vFields = spDescriptor->get()->getFields();
        for( auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
        {
            xmlNodePtr fieldNode = xmlNewChild(descNode, NULL, BAD_CAST TAG_FIELD, NULL);
            if(fieldNode == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for field" );

            if(xmlNewProp(fieldNode, BAD_CAST ATTR_NAME , BAD_CAST fieldDesc->name.c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (field name)" );

            if(xmlNewProp(fieldNode, BAD_CAST ATTR_FIELD_TYPE , BAD_CAST Variant::typeToString(fieldDesc->type).c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (field type)" );

            if(fieldDesc->optional)
                if(xmlNewProp(fieldNode, BAD_CAST ATTR_FIELD_OPTIONAL , BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (field is optional)" );

            if(fieldDesc->useEncryption)
            {
                if(xmlNewProp(fieldNode, BAD_CAST ATTR_ENCRYPTED_BOOL, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't crate xmlNode property (is field desc. encrypted)" );
            }
        }

        auto vRefs = (*spDescriptor)->getAllReferenceDescs();
        for (auto refDesc = vRefs.begin(); refDesc != vRefs.end(); refDesc++)
        {
            if ((*refDesc)->name.empty())
                continue;

            xmlNodePtr refNode = xmlNewChild(descNode, NULL, BAD_CAST TAG_METADATA_REFERENCE, NULL);
            if (refNode == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for reference." );

            if (xmlNewProp(refNode, BAD_CAST ATTR_NAME, BAD_CAST (*refDesc)->name.c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (reference name)");

            if ((*refDesc)->isUnique)
                if (xmlNewProp(refNode, BAD_CAST ATTR_REFERENCE_UNIQUE, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (reference is unique)");

            if ((*refDesc)->isCustom)
                if (xmlNewProp(refNode, BAD_CAST ATTR_REFERENCE_CUSTOM, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (reference is custom)");
        }
    }
}


static void add(xmlNodePtr metadataNode, const std::shared_ptr<Metadata>& spMetadata)
{
    if(xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_SCHEMA , BAD_CAST spMetadata->getSchemaName().c_str()) == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata schema name)" );

    if(xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_DESCRIPTION, BAD_CAST spMetadata->getName().c_str()) == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata description name)" );

    if (xmlNewProp(metadataNode, BAD_CAST ATTR_ID, BAD_CAST to_string(spMetadata->getId()).c_str()) == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata id)" );

    if(!spMetadata->getEncryptedData().empty())
    {
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_ENCRYPTED_DATA, BAD_CAST spMetadata->getEncryptedData().c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (encrypted data)" );
    }

    if(spMetadata->getUseEncryption())
    {
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_ENCRYPTED_BOOL, BAD_CAST "true") == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (is encrypted)" );
    }

    if(spMetadata->getFrameIndex() != Metadata::UNDEFINED_FRAME_INDEX)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_FRAME_IDX, BAD_CAST to_string(spMetadata->getFrameIndex()).c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata frame index)" );

    if(spMetadata->getNumOfFrames() != Metadata::UNDEFINED_FRAMES_NUMBER)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_NFRAMES, BAD_CAST to_string(spMetadata->getNumOfFrames()).c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata number of frames)" );

    if(spMetadata->getTime() != Metadata::UNDEFINED_TIMESTAMP)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_TIMESTAMP, BAD_CAST to_string(spMetadata->getTime()).c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata timestamp)" );

    if(spMetadata->getDuration() != Metadata::UNDEFINED_DURATION)
        if (xmlNewProp(metadataNode, BAD_CAST ATTR_METADATA_DURATION, BAD_CAST to_string(spMetadata->getDuration()).c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata duration)" );

    auto vFields = spMetadata->getDesc()->getFields();
    for( auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
    {
        Variant val = spMetadata->getFieldValue(fieldDesc->name);

        xmlNodePtr metadataFieldNode = xmlNewChild(metadataNode, NULL, BAD_CAST TAG_FIELD, NULL);
        if (metadataFieldNode == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for metadata field");

        if (xmlNewProp(metadataFieldNode, BAD_CAST ATTR_NAME, BAD_CAST fieldDesc->name.c_str()) == NULL)
            VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata field name)");

        if (!val.isEmpty())
        {
            if (xmlNewProp(metadataFieldNode, BAD_CAST ATTR_VALUE, BAD_CAST val.toString().c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata field value)");
        }

        auto fieldIt = spMetadata->findField(fieldDesc->name);
        if(fieldIt != spMetadata->end())
        {
            if(fieldIt->getUseEncryption())
            {
                if(xmlNewProp(metadataFieldNode, BAD_CAST ATTR_ENCRYPTED_BOOL, BAD_CAST "true") == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (is field encrypted)");
            }

            const std::string& encData = fieldIt->getEncryptedData();
            if(!encData.empty())
            {
                if(xmlNewProp(metadataFieldNode, BAD_CAST ATTR_ENCRYPTED_DATA, BAD_CAST encData.c_str()) == NULL)
                    VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (encrypted field data)");
            }
        }
    }

    auto refs = spMetadata->getAllReferences();
    if(!refs.empty())
    {
        for( auto reference = refs.begin(); reference != refs.end(); reference++)
        {
            xmlNodePtr referenceNode = xmlNewChild(metadataNode, NULL, BAD_CAST TAG_METADATA_REFERENCE, NULL);
            if(referenceNode == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for metadata reference" );

            if (xmlNewProp(referenceNode, BAD_CAST ATTR_NAME, BAD_CAST reference->getReferenceDescription()->name.c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata reference name)");

            if (xmlNewProp(referenceNode, BAD_CAST ATTR_ID, BAD_CAST to_string((*reference).getReferenceMetadata().lock()->getId()).c_str()) == NULL)
                VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode property (metadata reference id)" );
        }
    }
}

static void add(xmlNodePtr segmentsNode, const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    if (spSegment->getTitle() == "" || spSegment->getFPS() <= 0 || spSegment->getTime() < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid segment. Segment must have not empty title, fps > 0 and start time >= 0");

    if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_TITLE, BAD_CAST spSegment->getTitle().c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_FPS, BAD_CAST to_string(spSegment->getFPS()).c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_TIME, BAD_CAST to_string(spSegment->getTime()).c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");

    if (spSegment->getDuration() > 0)
        if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_DURATION, BAD_CAST to_string(spSegment->getDuration()).c_str()) == NULL)
            VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");

    long width, height;
    spSegment->getResolution(width, height);
    if (width > 0 && height > 0)
    {
        if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_WIDTH, BAD_CAST to_string(width).c_str()) == NULL)
            VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");

        if (xmlNewProp(segmentsNode, BAD_CAST ATTR_SEGMENT_HEIGHT, BAD_CAST to_string(height).c_str()) == NULL)
            VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property (video segment title)");
    }
}

XMLWriter::XMLWriter() : IWriter() { }
XMLWriter::~XMLWriter() {}

std::string XMLWriter::store(const std::shared_ptr<MetadataSchema>& spSchema)
{
    if( spSchema == nullptr )
        VMF_EXCEPTION(vmf::Exception, "Schema pointer is null");

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr schemaNode = xmlNewNode(NULL, BAD_CAST TAG_SCHEMA);
    if(schemaNode == NULL)
        VMF_EXCEPTION(vmf::Exception, "Can't create xmlNode for schema" );

    if(xmlDocSetRootElement(doc, schemaNode) != 0)
        VMF_EXCEPTION(vmf::Exception, "Can't set root element to the document" );

    add(schemaNode, spSchema);

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(Exception, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const std::shared_ptr<Metadata>& spMetadata)
{
    if( spMetadata == nullptr )
        VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr metadataNode = xmlNewNode(NULL, BAD_CAST TAG_METADATA);
    if(metadataNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for metadata" );

    if(xmlDocSetRootElement(doc, metadataNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    add(metadataNode, spMetadata);

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    if(schemas.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input schemas vector is empty");

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr schemasArrayNode = xmlNewNode(NULL, BAD_CAST TAG_SCHEMAS_ARRAY);
    if(schemasArrayNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for schemas array");

    if(xmlDocSetRootElement(doc, schemasArrayNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    std::for_each(schemas.begin(), schemas.end(), [&](const std::shared_ptr<MetadataSchema>& spSchema)
    {
        if( spSchema == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");
        xmlNodePtr schemaNode = xmlNewChild(schemasArrayNode, NULL, BAD_CAST TAG_SCHEMA, NULL);
        add(schemaNode, spSchema);
    });

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const MetadataSet& set)
{
    if(set.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input MetadataSet is empty");

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr metadataArrayNode = xmlNewNode(NULL, BAD_CAST TAG_METADATA_ARRAY);
    if(metadataArrayNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for metadata array");

    if(xmlDocSetRootElement(doc, metadataArrayNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    std::for_each(set.begin(), set.end(), [&](const std::shared_ptr<Metadata>& spMetadata)
    {
        if( spMetadata == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");
        xmlNodePtr metadataNode = xmlNewChild(metadataArrayNode, NULL, BAD_CAST TAG_METADATA, NULL);
        add(metadataNode, spMetadata);
    });

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const IdType& nextId,
                             const std::string& filepath,
                             const std::string& checksum,
                             const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                             const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                             const MetadataSet& set,
                             const std::string& hint)
{
    if(schemas.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input schemas vector is empty");

    std::for_each(set.begin(), set.end(), [&](const std::shared_ptr<Metadata>& spMetadata)
    {
        bool NoSchemaForMetadata = true;
        std::for_each(schemas.begin(), schemas.end(), [&](const std::shared_ptr<MetadataSchema>& spSchema)
        {
            if(spMetadata->getSchemaName() == spSchema->getName())
                NoSchemaForMetadata = false;
        });
        if(NoSchemaForMetadata)
            VMF_EXCEPTION(vmf::IncorrectParamException, "MetadataSet item references unknown schema");
    });

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr vmfRootNode = xmlNewNode(NULL, BAD_CAST TAG_VMF);
    if(vmfRootNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for vmf root element");
    if(xmlDocSetRootElement(doc, vmfRootNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    if (xmlNewProp(vmfRootNode, BAD_CAST ATTR_VMF_NEXTID, BAD_CAST to_string(nextId).c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property. Next Id");
    if (xmlNewProp(vmfRootNode, BAD_CAST ATTR_VMF_FILEPATH , BAD_CAST filepath.c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property. Filepath");
    if (xmlNewProp(vmfRootNode, BAD_CAST ATTR_VMF_CHECKSUM, BAD_CAST checksum.c_str()) == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property. Checksum");
    if (xmlNewProp(vmfRootNode, BAD_CAST ATTR_VMF_HINT, BAD_CAST hint.c_str()) == NULL)
            VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode property. Hint");

    xmlNodePtr segmentsArrayNode = NULL;
    if (!segments.empty())
    {
        segmentsArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_VIDEO_SEGMENTS_ARRAY, NULL);
        if (segmentsArrayNode == NULL)
            VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for video segments array");
    }

    xmlNodePtr schemasArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_SCHEMAS_ARRAY, NULL);
    if(schemasArrayNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for schemas array");

    xmlNodePtr metadataArrayNode = xmlNewChild(vmfRootNode, NULL, BAD_CAST TAG_METADATA_ARRAY, NULL);
    if(metadataArrayNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for metadata array");

    std::for_each(segments.begin(), segments.end(), [&](const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
    {
        if (spSegment == nullptr)
            VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");
        xmlNodePtr segmentNode = xmlNewChild(segmentsArrayNode, NULL, BAD_CAST TAG_VIDEO_SEGMENT, NULL);
        add(segmentNode, spSegment);
    });

    std::for_each(schemas.begin(), schemas.end(), [&](const std::shared_ptr<MetadataSchema>& spSchema)
    {
        if( spSchema == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");
        xmlNodePtr schemaNode = xmlNewChild(schemasArrayNode, NULL, BAD_CAST TAG_SCHEMA, NULL);
        add(schemaNode, spSchema);
    });

    std::for_each(set.begin(), set.end(), [&](const std::shared_ptr<Metadata>& spMetadata)
    {
        if( spMetadata == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");
        xmlNodePtr metadataNode = xmlNewChild(metadataArrayNode, NULL, BAD_CAST TAG_METADATA, NULL);
        add(metadataNode, spMetadata);
    });

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    if (spSegment == nullptr)
        VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");

    xmlDocPtr doc = xmlNewDoc(NULL);

    xmlNodePtr segmentNode = xmlNewNode(NULL, BAD_CAST TAG_VIDEO_SEGMENT);
    if (segmentNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for video segments");

    if (xmlDocSetRootElement(doc, segmentNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    add(segmentNode, spSegment);

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

std::string XMLWriter::store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    if (segments.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input video segments vector is empty");

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr segmentsArrayNode = xmlNewNode(NULL, BAD_CAST TAG_VIDEO_SEGMENTS_ARRAY);
    if (segmentsArrayNode == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't create xmlNode for video segments array");

    if (xmlDocSetRootElement(doc, segmentsArrayNode) != 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't set root element to the document");

    std::for_each(segments.begin(), segments.end(), [&](const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
    {
        if (spSegment == nullptr)
            VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");
        xmlNodePtr segmentNode = xmlNewChild(segmentsArrayNode, NULL, BAD_CAST TAG_VIDEO_SEGMENT, NULL);
        add(segmentNode, spSegment);
    });

    xmlChar *buf;
    int size;
    xmlDocDumpMemory(doc, &buf, &size);
    if (buf == NULL)
        VMF_EXCEPTION(vmf::InternalErrorException, "Can't save xmlDoc into the buffer");

    std::string outputString = (char *)buf;

    xmlFree(buf);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return outputString;
}

}//vmf
