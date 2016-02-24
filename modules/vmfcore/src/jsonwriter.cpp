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
#include "vmf/jsonwriter.hpp"
#include "vmf/rwconst.hpp"

#include "libjson.h"

#include <algorithm>

namespace vmf
{

static void add(JSONNode& schemaNode, const std::shared_ptr<MetadataSchema>& spSchema)
{
    schemaNode.push_back(JSONNode(ATTR_NAME, spSchema->getName()) );
    schemaNode.push_back(JSONNode(ATTR_SCHEMA_AUTHOR, spSchema->getAuthor()) );

    JSONNode descsArrayNode(JSON_ARRAY);
    descsArrayNode.set_name(TAG_DESCRIPTIONS_ARRAY);
    auto vDescs = spSchema->getAll();
    for( auto spDescriptor : vDescs )
    {
        JSONNode descNode(JSON_NODE);
        descNode.push_back(JSONNode(ATTR_NAME, spDescriptor->getMetadataName() ) );

        JSONNode fieldsArrayNode(JSON_ARRAY);
        fieldsArrayNode.set_name(TAG_FIELDS_ARRAY);
        auto vFields = spDescriptor.get()->getFields();
        for( auto fieldDesc : vFields )
        {
            JSONNode fieldNode(JSON_NODE);
            fieldNode.push_back(JSONNode(ATTR_NAME, fieldDesc.name) );
            fieldNode.push_back(JSONNode(ATTR_FIELD_TYPE, vmf::Variant::typeToString(fieldDesc.type) ) );
            if(fieldDesc.optional)
                fieldNode.push_back(JSONNode(ATTR_FIELD_OPTIONAL, "true") );

            fieldsArrayNode.push_back(fieldNode);
        }

        JSONNode referencesArrayNode(JSON_ARRAY);
        referencesArrayNode.set_name(TAG_METADATA_REFERENCES_ARRAY);
        auto vReference = spDescriptor->getAllReferenceDescs();
        for (auto refDesc : vReference )
        {
            if (refDesc->name.empty())
                continue;

            JSONNode refsNode(JSON_NODE);
            refsNode.push_back(JSONNode(ATTR_NAME, refDesc->name));
            if (refDesc->isUnique)
                refsNode.push_back(JSONNode(ATTR_REFERENCE_UNIQUE, "true"));

            if (refDesc->isCustom)
                refsNode.push_back(JSONNode(ATTR_REFERENCE_CUSTOM, "true"));

            referencesArrayNode.push_back(refsNode);
        }

        descNode.push_back(fieldsArrayNode);
        descNode.push_back(referencesArrayNode);
        descsArrayNode.push_back(descNode);
    }
    schemaNode.push_back(descsArrayNode);
}

static void add(JSONNode& metadataNode, const std::shared_ptr<Metadata>& spMetadata)
{
    metadataNode.push_back( JSONNode(ATTR_METADATA_SCHEMA, spMetadata->getSchemaName()) );
    metadataNode.push_back( JSONNode(ATTR_METADATA_DESCRIPTION, spMetadata->getName()) );
    metadataNode.push_back( JSONNode(ATTR_ID, spMetadata->getId()) );
    if (spMetadata->getFrameIndex() != Metadata::UNDEFINED_FRAME_INDEX)
    {
        long long llVal = spMetadata->getFrameIndex();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_FRAME_IDX_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_FRAME_IDX_HI, hi));
    }
    if(spMetadata->getNumOfFrames() != Metadata::UNDEFINED_FRAMES_NUMBER)
    {
        long long llVal = spMetadata->getNumOfFrames();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_NFRAMES_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_NFRAMES_HI, hi));
    }
    if(spMetadata->getTime() != Metadata::UNDEFINED_TIMESTAMP)
    {
        long long llVal = spMetadata->getTime();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_TIMESTAMP_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_TIMESTAMP_HI, hi));
    }
    if (spMetadata->getDuration() != Metadata::UNDEFINED_DURATION)
    {
        long long llVal = spMetadata->getDuration();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_DURATION_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_DURATION_HI, hi));
    }

    JSONNode metadataFieldsArrayNode(JSON_ARRAY);
    metadataFieldsArrayNode.set_name(TAG_FIELDS_ARRAY);
    auto vFields = spMetadata->getDesc()->getFields();
    for( auto fieldDesc : vFields )
    {
        Variant val = spMetadata->getFieldValue(fieldDesc.name);
        if (!val.isEmpty())
        {
            JSONNode metadataFieldNode(JSON_NODE);
            metadataFieldNode.push_back( JSONNode(ATTR_NAME, fieldDesc.name) );
            metadataFieldNode.push_back( JSONNode(ATTR_VALUE, val.toString()) );
            metadataFieldsArrayNode.push_back(metadataFieldNode);
        }
    }
    metadataNode.push_back(metadataFieldsArrayNode);

    auto refs = spMetadata->getAllReferences();
    if(!refs.empty())
    {
        JSONNode referencesArrayNode(JSON_ARRAY);
        referencesArrayNode.set_name(TAG_METADATA_REFERENCES_ARRAY);
        for( auto reference : refs )
        {
            JSONNode referenceNode(JSON_NODE);
            referenceNode.set_name(TAG_METADATA_REFERENCE);
            referenceNode.push_back(JSONNode(ATTR_NAME, reference.getReferenceDescription()->name));
            referenceNode.push_back(JSONNode(ATTR_ID, reference.getReferenceMetadata().lock()->getId()));
            referencesArrayNode.push_back(referenceNode);
        }
        metadataNode.push_back(referencesArrayNode);
    }
}

static void add(JSONNode& segmentsNode, const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    if (spSegment->getTitle() == "" || spSegment->getFPS() <= 0 || spSegment->getTime() < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid video segment: title, fps or timestamp value(s) is/are invalid!");

    segmentsNode.push_back(JSONNode(ATTR_SEGMENT_TITLE, spSegment->getTitle()));
    segmentsNode.push_back(JSONNode(ATTR_SEGMENT_FPS, spSegment->getFPS()));
    segmentsNode.push_back(JSONNode(ATTR_SEGMENT_TIME, spSegment->getTime()));

    if (spSegment->getDuration() > 0)
        segmentsNode.push_back(JSONNode(ATTR_SEGMENT_DURATION, spSegment->getDuration()));

    long width, height;
    spSegment->getResolution(width, height);
    if (width > 0 && height > 0)
    {
        segmentsNode.push_back(JSONNode(ATTR_SEGMENT_WIDTH, width));
        segmentsNode.push_back(JSONNode(ATTR_SEGMENT_HEIGHT, height));
    }
}

static void add(JSONNode& statNode, const Stat& stat)
{
    if (stat.getName().empty())
        VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: name is invalid!");

    statNode.push_back(JSONNode(ATTR_STAT_NAME, stat.getName()));

    std::vector< std::string > fieldNames = stat.getAllFieldNames();
    if (!fieldNames.empty())
    {
        JSONNode fieldsArrayNode(JSON_ARRAY);
        fieldsArrayNode.set_name(TAG_STAT_FIELDS_ARRAY);

        for(auto& fieldName : fieldNames)
        {
            const StatField& field = stat.getField(fieldName);

            if (field.getName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field name is invalid!");
            if (field.getFieldName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata field name is invalid!");
            if (field.getOpName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field operation name is invalid!");

            std::shared_ptr< MetadataDesc > metadataDesc = field.getMetadataDesc();
            if (metadataDesc == nullptr)
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata descriptor is null!");
            if (metadataDesc->getSchemaName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata schema name is invalid!");
            if (metadataDesc->getMetadataName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata name is invalid!");

            JSONNode fieldNode(JSON_NODE);
            fieldNode.set_name(TAG_STAT_FIELD);

            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_NAME, field.getName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_SCHEMA_NAME, metadataDesc->getSchemaName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_METADATA_NAME, metadataDesc->getMetadataName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_FIELD_NAME, field.getFieldName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_OP_NAME, field.getOpName()));

            fieldsArrayNode.push_back(fieldNode);
        }

        statNode.push_back(fieldsArrayNode);
    }
}

std::string JSONWriter::store(const std::shared_ptr<MetadataSchema>& spSchema)
{
    if( spSchema == nullptr )
        VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");

    JSONNode schemaNode(JSON_NODE);
    schemaNode.set_name(TAG_SCHEMA);

    add(schemaNode, spSchema);

    JSONNode root(JSON_NODE);
    root.push_back(schemaNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const std::shared_ptr<Metadata>& spMetadata)
{
    if( spMetadata == nullptr )
        VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");

    JSONNode metadataNode(JSON_NODE);
    metadataNode.set_name(TAG_METADATA);

    add(metadataNode, spMetadata);

    JSONNode root(JSON_NODE);
    root.push_back(metadataNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    if(schemas.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input schemas vector is empty");

    JSONNode schemasArrayNode(JSON_ARRAY);
    schemasArrayNode.set_name(TAG_SCHEMAS_ARRAY);

    for(auto spSchema : schemas)
    {
        if( spSchema == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");
        JSONNode schemaNode(JSON_NODE);
        add(schemaNode, spSchema);
        schemasArrayNode.push_back(schemaNode);
    }

    JSONNode root(JSON_NODE);
    root.push_back(schemasArrayNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const MetadataSet& set)
{
    if(set.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input MetadataSet is empty");

    JSONNode metadataArrayNode(JSON_ARRAY);
    metadataArrayNode.set_name(TAG_METADATA_ARRAY);

    for(auto spMetadata : set)
    {
        if( spMetadata == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");
        JSONNode metadataNode(JSON_NODE);
        add(metadataNode, spMetadata);
        metadataArrayNode.push_back(metadataNode);
    }

    JSONNode root(JSON_NODE);
    root.push_back(metadataArrayNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const IdType& nextId,
    const std::string& filepath,
    const std::string& checksum,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const MetadataSet& set,
    const std::vector< Stat >& stats)
{
    if(schemas.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input schemas vector is empty");

    for(auto spMetadata : set)
    {
        bool NoSchemaForMetadata = true;
        for(auto spSchema : schemas)
        {
            if(spMetadata->getSchemaName() == spSchema->getName())
                NoSchemaForMetadata = false;
        }
        if(NoSchemaForMetadata)
            VMF_EXCEPTION(IncorrectParamException, "MetadataSet item references unknown schema");
    }

    JSONNode vmfRootNode(JSON_NODE);
    vmfRootNode.set_name(TAG_VMF);

    vmfRootNode.push_back( JSONNode(ATTR_VMF_NEXTID , nextId) );
    vmfRootNode.push_back( JSONNode(ATTR_VMF_FILEPATH , filepath) );
    vmfRootNode.push_back( JSONNode(ATTR_VMF_CHECKSUM , checksum) );

    if(!segments.empty())
    {
	JSONNode segmentsArrayNode(JSON_ARRAY);
	segmentsArrayNode.set_name(TAG_VIDEO_SEGMENTS_ARRAY);

        for(auto spSegment : segments)
	{
	    if( spSegment == nullptr )
		VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");
	    JSONNode segmentNode(JSON_NODE);
	    add(segmentNode, spSegment);
	    segmentsArrayNode.push_back(segmentNode);
        }

	vmfRootNode.push_back(segmentsArrayNode);
    }

    if(!stats.empty())
    {
        JSONNode statsArrayNode(JSON_ARRAY);
        statsArrayNode.set_name(TAG_STATS_ARRAY);

        for(auto& stat : stats)
        {
            JSONNode statNode(JSON_NODE);
            add(statNode, stat);
            statsArrayNode.push_back(statNode);
        }

        vmfRootNode.push_back(statsArrayNode);
    }

    JSONNode schemasArrayNode(JSON_ARRAY);
    schemasArrayNode.set_name(TAG_SCHEMAS_ARRAY);

    for(auto spSchema : schemas)
    {
        if( spSchema == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");
        JSONNode schemaNode(JSON_NODE);
        add(schemaNode, spSchema);
        schemasArrayNode.push_back(schemaNode);
    }

    vmfRootNode.push_back(schemasArrayNode);

    JSONNode metadataArrayNode(JSON_ARRAY);
    metadataArrayNode.set_name(TAG_METADATA_ARRAY);

    for(auto spMetadata : set)
    {
        if( spMetadata == nullptr )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");
        JSONNode metadataNode(JSON_NODE);
        add(metadataNode, spMetadata);
        metadataArrayNode.push_back(metadataNode);
    }

    vmfRootNode.push_back(metadataArrayNode);

    JSONNode root(JSON_NODE);
    root.push_back(vmfRootNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    if( spSegment == nullptr )
	VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");

    JSONNode segmentNode(JSON_NODE);
    segmentNode.set_name(TAG_VIDEO_SEGMENT);

    add(segmentNode, spSegment);

    JSONNode root(JSON_NODE);
    root.push_back(segmentNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    if(segments.empty())
	VMF_EXCEPTION(vmf::IncorrectParamException, "Input video segments vector is empty");

    JSONNode segmentsArrayNode(JSON_ARRAY);
    segmentsArrayNode.set_name(TAG_VIDEO_SEGMENTS_ARRAY);

    for(auto spSegment : segments)
    {
        if( spSegment == nullptr )
	        VMF_EXCEPTION(vmf::IncorrectParamException, "Video segment pointer is null");
        JSONNode segmentNode(JSON_NODE);
        add(segmentNode, spSegment);
        segmentsArrayNode.push_back(segmentNode);
    }

    JSONNode root(JSON_NODE);
    root.push_back(segmentsArrayNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

std::string JSONWriter::store(const std::vector< Stat >& stats)
{
    if(stats.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Input stat object vector is empty");

    JSONNode statsArrayNode(JSON_ARRAY);
    statsArrayNode.set_name(TAG_STATS_ARRAY);

    for(auto& stat : stats)
    {
        JSONNode statNode(JSON_NODE);
        add(statNode, stat);
        statsArrayNode.push_back(statNode);
    }

    JSONNode root(JSON_NODE);
    root.push_back(statsArrayNode);

    return root.write_formatted();
}

std::string JSONWriter::store(const Stat& stat)
{
    JSONNode statNode(JSON_NODE);
    statNode.set_name(TAG_STAT);

    add(statNode, stat);

    JSONNode root(JSON_NODE);
    root.push_back(statNode);

    return root.write_formatted();
}

}//vmf
