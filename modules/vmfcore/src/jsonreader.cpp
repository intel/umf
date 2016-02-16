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
#include "vmf/jsonreader.hpp"
#include "vmf/rwconst.hpp"

#include "libjson.h"

namespace vmf
{

static std::shared_ptr<MetadataSchema> parseSchemaFromNode(JSONNode& schemaNode)
{
    std::shared_ptr<vmf::MetadataSchema> spSchema;
    auto schemaNameIter = schemaNode.find(ATTR_NAME);
    auto schemaAuthorIter = schemaNode.find(ATTR_SCHEMA_AUTHOR);
    if(schemaNameIter != schemaNode.end())
    {
        if(schemaAuthorIter != schemaNode.end())
            spSchema = std::make_shared<vmf::MetadataSchema>(schemaNameIter->as_string(), schemaAuthorIter->as_string());
        else
            spSchema = std::make_shared<vmf::MetadataSchema>(schemaNameIter->as_string());
    }
    else
        VMF_EXCEPTION(IncorrectParamException, "Schema has no name");

    auto descsArrayIter = schemaNode.find(TAG_DESCRIPTIONS_ARRAY);
    if(descsArrayIter == schemaNode.end())
        VMF_EXCEPTION(IncorrectParamException, "Can't find descriptions-array JSON node");

    std::shared_ptr<vmf::MetadataDesc> spDesc;
    for(auto descNode = descsArrayIter->begin(); descNode != descsArrayIter->end(); descNode++)
    {
        auto descNameIter = descNode->find(ATTR_NAME);
        if(descNameIter == descNode->end())
            VMF_EXCEPTION(IncorrectParamException, "Description has no name");

        auto fieldsArrayIter = descNode->find(TAG_FIELDS_ARRAY);
        if(fieldsArrayIter == descNode->end())
            VMF_EXCEPTION(IncorrectParamException, "Description has no fields array");

        std::vector<FieldDesc> vFields;
        for(auto fieldNode = fieldsArrayIter->begin(); fieldNode != fieldsArrayIter->end(); fieldNode++)
        {
            auto fieldNameIter = fieldNode->find(ATTR_NAME);
            auto fieldTypeIter = fieldNode->find(ATTR_FIELD_TYPE);
            if(fieldNameIter == fieldNode->end() || fieldTypeIter == fieldNode->end() )
                VMF_EXCEPTION(IncorrectParamException, "Field has no 'name' or 'type' attribute");
            vmf::Variant::Type field_type = Variant::typeFromString(fieldTypeIter->as_string());
            bool field_optional = false;
            auto fieldsOptionalityIter = fieldNode->find(ATTR_FIELD_OPTIONAL);
            if(fieldsOptionalityIter != fieldNode->end())
            {
                if(fieldsOptionalityIter->as_string() == "true")
                    field_optional = true;
                else if (fieldsOptionalityIter->as_string() == "false")
                    field_optional = false;
                else
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'optional'");
            }
            vFields.push_back(FieldDesc(fieldNameIter->as_string(), field_type, field_optional));
        }

        auto refsArrayIter = descNode->find(TAG_METADATA_REFERENCES_ARRAY);
        if (refsArrayIter == descNode->end())
            VMF_EXCEPTION(IncorrectParamException, "Description has no references array");

        std::vector<std::shared_ptr<ReferenceDesc>> vReferences;
        for (auto refNode = refsArrayIter->begin(); refNode != refsArrayIter->end(); refNode++)
        {
            auto refNameIter = refNode->find(ATTR_NAME);
            if (refNameIter == refNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Field has no 'name' attribute");

            auto refUniqueIter = refNode->find(ATTR_REFERENCE_UNIQUE);
            bool isUnique = false;
            if (refUniqueIter != refNode->end())
            {
                if (refUniqueIter->as_string() == "true")
                    isUnique = true;
                else if (refUniqueIter->as_string() == "false")
                    isUnique = false;
                else
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'isUnique'");
            }

            auto refCustomIter = refNode->find(ATTR_REFERENCE_CUSTOM);
            bool isCustom = false;
            if (refCustomIter != refNode->end())
            {
                if (refCustomIter->as_string() == "true")
                    isCustom = true;
                else if (refCustomIter->as_string() == "false")
                    isCustom = false;
                else
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid value of boolean attribute 'isCustom'");
            }

            vReferences.emplace_back(std::make_shared<ReferenceDesc>(refNameIter->as_string(), isUnique, isCustom));
        }

        spDesc = std::make_shared<vmf::MetadataDesc>(descNameIter->as_string(), vFields, vReferences);
        spSchema->add(spDesc);
    }

    return spSchema;
}

static std::shared_ptr<MetadataInternal> parseMetadataFromNode(JSONNode& metadataNode, const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    auto schemaIter = metadataNode.find(ATTR_METADATA_SCHEMA);
    auto descIter = metadataNode.find(ATTR_METADATA_DESCRIPTION);
    auto idIter = metadataNode.find(ATTR_ID);
    if(schemaIter == metadataNode.end() || descIter == metadataNode.end() || idIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata item has no schema name, description name or id");

    auto schema = schemas.begin();
    for(; schema != schemas.end(); schema++)
        if((*schema)->getName() == schemaIter->as_string())
            break;

    if(schema == schemas.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Unknown schema for metadata item");

    std::shared_ptr<MetadataDesc> spDesc;
    if( (spDesc = (*schema)->findMetadataDesc(descIter->as_string())) == nullptr)
        VMF_EXCEPTION(vmf::IncorrectParamException, "Unknown description for metadata item");

    std::shared_ptr<MetadataInternal> spMetadataInternal(new MetadataInternal(spDesc));
    spMetadataInternal->setId(idIter->as_int());

    auto frameIdxLoIter = metadataNode.find(ATTR_METADATA_FRAME_IDX_LO);
    auto frameIdxHiIter = metadataNode.find(ATTR_METADATA_FRAME_IDX_HI);
    auto nFramesLoIter = metadataNode.find(ATTR_METADATA_NFRAMES_LO);
    auto nFramesHiIter = metadataNode.find(ATTR_METADATA_NFRAMES_HI);
    auto timeLoIter = metadataNode.find(ATTR_METADATA_TIMESTAMP_LO);
    auto timeHiIter = metadataNode.find(ATTR_METADATA_TIMESTAMP_HI);
    auto durationLoIter = metadataNode.find(ATTR_METADATA_DURATION_LO);
    auto durationHiIter = metadataNode.find(ATTR_METADATA_DURATION_HI);

    if (frameIdxLoIter != metadataNode.end() && frameIdxHiIter != metadataNode.end())
    {
        unsigned long lo = frameIdxLoIter->as_int();
        unsigned long hi = frameIdxHiIter->as_int();
        long long frmIdx = ((long long)hi << 32) | lo;

        if (nFramesLoIter != metadataNode.end() && nFramesHiIter != metadataNode.end())
        {
            unsigned long lo = nFramesLoIter->as_int();
            unsigned long hi = nFramesHiIter->as_int();
            long long numFrm = ((long long)hi << 32) | lo;
            spMetadataInternal->setFrameIndex(frmIdx, numFrm);
        }
        else
            spMetadataInternal->setFrameIndex(frmIdx);
    }
    if (timeLoIter != metadataNode.end() && timeHiIter != metadataNode.end())
    {
        unsigned long lo = timeLoIter->as_int();
        unsigned long hi = timeHiIter->as_int();
        long long time = ((long long)hi << 32) | lo;

        if (durationLoIter != metadataNode.end() && durationHiIter != metadataNode.end())
        {
            unsigned long lo = durationLoIter->as_int();
            unsigned long hi = durationHiIter->as_int();
            long long dur = ((long long)hi << 32) | lo;
            spMetadataInternal->setTimestamp(time, dur);
        }
        else
            spMetadataInternal->setTimestamp(time);
    }

    auto metadataFieldsArrayIter = metadataNode.find(TAG_FIELDS_ARRAY);
    if(metadataFieldsArrayIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "No metadata fields array");

    for(auto fieldNode = metadataFieldsArrayIter->begin(); fieldNode != metadataFieldsArrayIter->end(); fieldNode++)
    {
        FieldDesc fieldDesc;
        auto fieldNameIter = fieldNode->find(ATTR_NAME);
        auto fieldValueIter = fieldNode->find(ATTR_VALUE);
        if(fieldNameIter == fieldNode->end() || fieldValueIter == fieldNode->end() )
            VMF_EXCEPTION(vmf::IncorrectParamException, "Missing field name or field value");

        vmf::Variant field_value;
        spDesc->getFieldDesc(fieldDesc, fieldNameIter->as_string());
        field_value.fromString(fieldDesc.type, fieldValueIter->as_string());
        spMetadataInternal->setFieldValue(fieldNameIter->as_string(), field_value);
    }

    auto referencesArrayIter = metadataNode.find(TAG_METADATA_REFERENCES_ARRAY);
    if(referencesArrayIter != metadataNode.end())
    {
        for(auto referenceNode = referencesArrayIter->begin(); referenceNode != referencesArrayIter->end(); referenceNode++)
        {
            auto referenceIdIter = referenceNode->find(ATTR_ID);
            if(referenceIdIter == referenceNode->end()) VMF_EXCEPTION(vmf::IncorrectParamException, "Missing reference 'id'");

            auto referenceNameIter = referenceNode->find(ATTR_NAME);
            if(referenceNameIter == referenceNode->end()) VMF_EXCEPTION(vmf::IncorrectParamException, "Missing reference 'name'");

            spMetadataInternal->vRefs.push_back(std::make_pair(IdType(referenceIdIter->as_int()), referenceNameIter->as_string()));
        }
    }

    return spMetadataInternal;
}

static std::shared_ptr<MetadataStream::VideoSegment> parseVideoSegmentFromNode(JSONNode& segmentNode)
{
    auto segmentTitleIter = segmentNode.find(ATTR_SEGMENT_TITLE);
    auto segmentFPSIter = segmentNode.find(ATTR_SEGMENT_FPS);
    auto segmentTimeIter = segmentNode.find(ATTR_SEGMENT_TIME);
    auto segmentDurationIter = segmentNode.find(ATTR_SEGMENT_DURATION);
    auto segmentWidthIter = segmentNode.find(ATTR_SEGMENT_WIDTH);
    auto segmentHeightIter = segmentNode.find(ATTR_SEGMENT_HEIGHT);

    if(segmentTitleIter == segmentNode.end())
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no title");
    if(segmentFPSIter == segmentNode.end())
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no fps value");
    if(segmentTimeIter == segmentNode.end())
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no time value");

    std::string title = segmentTitleIter->as_string();
    double fps = segmentFPSIter->as_float();
    long long timestamp = segmentTimeIter->as_int();

    if(title.empty())
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid title");
    if(fps <= 0)
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid fps value");
    if(timestamp < 0)
	VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid time value");

    std::shared_ptr<MetadataStream::VideoSegment> spSegment(new MetadataStream::VideoSegment(title, fps, timestamp));

    if(segmentDurationIter != segmentNode.end())
    {
	long long duration = segmentDurationIter->as_int();
	if(duration > 0)
	    spSegment->setDuration(duration);
    }
    if(segmentWidthIter != segmentNode.end() && segmentHeightIter != segmentNode.end())
    {
	long width = segmentWidthIter->as_int(), height = segmentHeightIter->as_int();
	if(width > 0 && height > 0)
	    spSegment->setResolution(width, height);
    }

    return spSegment;

}

static void parseStatFromNode(JSONNode& statNode, std::vector< Stat >& stats)
{
    auto statNameIter = statNode.find(ATTR_STAT_NAME);

    if(statNameIter == statNode.end())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no stat name");

    std::string statName = statNameIter->as_string();
    const StatUpdateMode::Type updateMode = StatUpdateMode::Disabled;

    if(statName.empty())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid stat name");

    std::vector< StatField > fields;

    auto fieldsArrayIter = statNode.find(TAG_STAT_FIELDS_ARRAY);
    if(fieldsArrayIter != statNode.end())
    {
        for(auto fieldNode = fieldsArrayIter->begin(); fieldNode != fieldsArrayIter->end(); fieldNode++)
        {
            auto fieldNameIter = fieldNode->find(ATTR_STAT_FIELD_NAME);
            if(fieldNameIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Stat field has no name");

            auto schemaNameIter = fieldNode->find(ATTR_STAT_FIELD_SCHEMA_NAME);
            if(schemaNameIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Stat field has no metadata schema name");

            auto metadataNameIter = fieldNode->find(ATTR_STAT_FIELD_METADATA_NAME);
            if(metadataNameIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Stat field has no metadata name");

            auto metadataFieldNameIter = fieldNode->find(ATTR_STAT_FIELD_FIELD_NAME);
            if(metadataFieldNameIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Stat field has no metadata field name");

            auto opNameIter = fieldNode->find(ATTR_STAT_FIELD_OP_NAME);
            if(opNameIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Stat field has no operation name");

            std::string fieldName, schemaName, metadataName, metadataFieldName, opName;

            fieldName = fieldNameIter->as_string();
            schemaName = schemaNameIter->as_string();
            metadataName = metadataNameIter->as_string();
            metadataFieldName = metadataFieldNameIter->as_string();
            opName = opNameIter->as_string();

            fields.push_back(StatField(fieldName, schemaName, metadataName, metadataFieldName, opName));
        }
    }

    stats.emplace_back(statName, fields, updateMode);
}

JSONReader::JSONReader(){}

bool JSONReader::parseSchemas(const std::string& text,
                                      std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input JSON string");
        return false;
    }

    schemas.clear();

    JSONNode root(JSON_NODE);
    try
    {
        root = libjson::parse(text);
    }
    catch(...)
    {
        VMF_LOG_ERROR("JSON document has no root element");
        return false;
    }

    if(root.size() != 1)
    {
        VMF_LOG_ERROR("More than one JSON root");
        return false;
    }

    JSONNode localRootNode = root[0];

    if( localRootNode.name() == TAG_SCHEMA )
    {
        try
        {
            std::shared_ptr<MetadataSchema> spSchema = parseSchemaFromNode(localRootNode);
            schemas.push_back(spSchema);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( localRootNode.name() == TAG_SCHEMAS_ARRAY )
    {
        for(auto node : localRootNode)
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
    else if( localRootNode.name() == TAG_VMF )
    {
        for(auto rootChildNode : localRootNode)
            if(rootChildNode.name() == TAG_SCHEMAS_ARRAY )
                for(auto node : rootChildNode)
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
    return true;
}

bool JSONReader::parseMetadata(const std::string& text,
                                       const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                       std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input JSON string");
        return false;
    }

    metadata.clear();

    JSONNode root;
    try
    {
        root = libjson::parse(text);
    }
    catch(...)
    {
        VMF_LOG_ERROR("Can't get JSON root");
        return false;
    }

    if(root.size() != 1)
    {
        VMF_LOG_ERROR("More than one JSON root");
        return false;
    }

    JSONNode localRootNode = root[0];

    if( localRootNode.name() == TAG_METADATA )
    {
        try
        {
            std::shared_ptr<MetadataInternal> spMetadata = parseMetadataFromNode(localRootNode, schemas);
            metadata.push_back(spMetadata);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( localRootNode.name() == TAG_METADATA_ARRAY )
    {
        for(auto node : localRootNode)
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
    else if( localRootNode.name() == TAG_VMF )
    {
        for(auto rootChildNode : localRootNode)
            if(rootChildNode.name() == TAG_METADATA_ARRAY )
                for(auto node : rootChildNode)
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

    return true;
}

bool JSONReader::parseAll(const std::string& text, IdType& nextId, std::string& filepath, std::string& checksum,
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    std::vector<std::shared_ptr<MetadataInternal>>& metadata,
    std::vector< Stat >& stats)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input JSON string");
        return false;
    }

    schemas.clear();
    metadata.clear();

    JSONNode root;
    try
    {
        root = libjson::parse(text);
    }
    catch(...)
    {
        VMF_LOG_ERROR("Can't get JSON root");
        return false;
    }

    if(root.size() != 1)
    {
        VMF_LOG_ERROR("More than one JSON root");
        return false;
    }

    JSONNode localRootNode = root[0];

    if( localRootNode.name() == TAG_VMF )
    {
        auto nextIdIter = localRootNode.find(ATTR_VMF_NEXTID);
        if(nextIdIter != localRootNode.end() )
            nextId = nextIdIter->as_int();
        auto filepathIter = localRootNode.find(ATTR_VMF_FILEPATH);
        if(filepathIter != localRootNode.end() )
            filepath = filepathIter->as_string();
        auto checksumIter = localRootNode.find(ATTR_VMF_CHECKSUM);
        if(checksumIter != localRootNode.end() )
            checksum = checksumIter->as_string();

        if(!parseStats(text, stats))
            return false;
        if(!parseVideoSegments(text, segments))
            return false;
        if(!parseSchemas(text, schemas))
            return false;
        if(!parseMetadata(text, schemas, metadata))
            return false;
    }
    else
    {
        VMF_LOG_ERROR("Root JSON element isn't 'vmf'");
        return false;
    }

    return true;
}


bool JSONReader::parseVideoSegments(const std::string& text, std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input JSON string");
        return false;
    }

    segments.clear();

    JSONNode root(JSON_NODE);
    try
    {
        root = libjson::parse(text);
    }
    catch(...)
    {
        VMF_LOG_ERROR("JSON document has no root element");
        return false;
    }

    if(root.size() != 1)
    {
        VMF_LOG_ERROR("More than one JSON root");
        return false;
    }

    JSONNode localRootNode = root[0];

    if( localRootNode.name() == TAG_VIDEO_SEGMENT )
    {
	try
	{
	    std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseVideoSegmentFromNode(localRootNode);
	    segments.push_back(spSegment);
	}
	catch(Exception& e)
	{
	    VMF_LOG_ERROR("Exception: %s", e.what());
	    return false;
	}
    }
    else if( localRootNode.name() == TAG_VIDEO_SEGMENTS_ARRAY )
    {
        for(auto node : localRootNode)
	try
	{
            std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseVideoSegmentFromNode(node);
	    segments.push_back(spSegment);
	}
	catch(Exception& e)
	{
	    VMF_LOG_ERROR("Exception: %s", e.what());
	    return false;
	}
    }
    else if( localRootNode.name() == TAG_VMF )
    {
        for(auto rootChildNode : localRootNode)
            if(rootChildNode.name() == TAG_VIDEO_SEGMENTS_ARRAY )
                for(auto node : rootChildNode)
		try
		{
                    std::shared_ptr<MetadataStream::VideoSegment> spSegment = parseVideoSegmentFromNode(node);
		    segments.push_back(spSegment);
		}
		catch(Exception& e)
		{
		    VMF_LOG_ERROR("Exception: %s", e.what());
		    return false;
		}
    }
    return true;
}

bool JSONReader::parseStats(const std::string& text, std::vector< Stat >& stats)
{
    if(text.empty())
    {
        VMF_LOG_ERROR("Empty input JSON string");
        return false;
    }

    JSONNode root(JSON_NODE);
    try
    {
        root = libjson::parse(text);
    }
    catch(...)
    {
        VMF_LOG_ERROR("JSON document has no root element");
        return false;
    }

    if(root.size() != 1)
    {
        VMF_LOG_ERROR("More than one JSON root");
        return false;
    }

    JSONNode localRootNode = root[0];

    if( localRootNode.name() == TAG_STAT )
    {
        try
        {
            parseStatFromNode(localRootNode, stats);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( localRootNode.name() == TAG_STATS_ARRAY )
    {
        for(auto node : localRootNode)
        try
        {
            parseStatFromNode(node, stats);
        }
        catch(Exception& e)
        {
            VMF_LOG_ERROR("Exception: %s", e.what());
            return false;
        }
    }
    else if( localRootNode.name() == TAG_VMF )
    {
        for(auto rootChildNode : localRootNode)
            if(rootChildNode.name() == TAG_STATS_ARRAY )
                for(auto node : rootChildNode)
                try
                {
                    parseStatFromNode(node, stats);
                }
                catch(Exception& e)
                {
                    VMF_LOG_ERROR("Exception: %s", e.what());
                    return false;
                }
    }
    return true;
}

}//vmf
