/*
* Copyright 2016 Intel(r) Corporation
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
#include "vmf/format_json.hpp"
#include "vmf/format_const.hpp"

#include "libjson.h"

#include <algorithm>

namespace vmf
{

FormatJSON::FormatJSON()
{}

FormatJSON::~FormatJSON()
{}

/*
** store() support
*/

static void add(JSONNode& schemaNode, const std::shared_ptr<MetadataSchema>& spSchema)
{
    schemaNode.push_back(JSONNode(ATTR_NAME, spSchema->getName()));
    schemaNode.push_back(JSONNode(ATTR_SCHEMA_AUTHOR, spSchema->getAuthor()));

    JSONNode descsArrayNode(JSON_ARRAY);
    descsArrayNode.set_name(TAG_DESCRIPTIONS_ARRAY);
    auto vDescs = spSchema->getAll();
    for (auto spDescriptor = vDescs.begin(); spDescriptor != vDescs.end(); spDescriptor++)
    {
        JSONNode descNode(JSON_NODE);
        descNode.push_back(JSONNode(ATTR_NAME, (*spDescriptor)->getMetadataName()));

        JSONNode fieldsArrayNode(JSON_ARRAY);
        fieldsArrayNode.set_name(TAG_FIELDS_ARRAY);
        auto vFields = spDescriptor->get()->getFields();
        for (auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
        {
            JSONNode fieldNode(JSON_NODE);
            fieldNode.push_back(JSONNode(ATTR_NAME, fieldDesc->name));
            fieldNode.push_back(JSONNode(ATTR_FIELD_TYPE, vmf::Variant::typeToString(fieldDesc->type)));
            if (fieldDesc->optional)
                fieldNode.push_back(JSONNode(ATTR_FIELD_OPTIONAL, "true"));

            fieldsArrayNode.push_back(fieldNode);
        }

        JSONNode referencesArrayNode(JSON_ARRAY);
        referencesArrayNode.set_name(TAG_METADATA_REFERENCES_ARRAY);
        auto vReference = (*spDescriptor)->getAllReferenceDescs();
        for (auto refDesc = vReference.begin(); refDesc != vReference.end(); refDesc++)
        {
            if ((*refDesc)->name.empty())
                continue;

            JSONNode refsNode(JSON_NODE);
            refsNode.push_back(JSONNode(ATTR_NAME, (*refDesc)->name));
            if ((*refDesc)->isUnique)
                refsNode.push_back(JSONNode(ATTR_REFERENCE_UNIQUE, "true"));

            if ((*refDesc)->isCustom)
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
    metadataNode.push_back(JSONNode(ATTR_METADATA_SCHEMA, spMetadata->getSchemaName()));
    metadataNode.push_back(JSONNode(ATTR_METADATA_DESCRIPTION, spMetadata->getName()));
    metadataNode.push_back(JSONNode(ATTR_ID, spMetadata->getId()));
    if (spMetadata->getFrameIndex() != Metadata::UNDEFINED_FRAME_INDEX)
    {
        long long llVal = spMetadata->getFrameIndex();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_FRAME_IDX_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_FRAME_IDX_HI, hi));
    }
    if (spMetadata->getNumOfFrames() != Metadata::UNDEFINED_FRAMES_NUMBER)
    {
        long long llVal = spMetadata->getNumOfFrames();
        unsigned long lo = llVal & 0xffffffff;
        unsigned long hi = llVal >> 32;
        metadataNode.push_back(JSONNode(ATTR_METADATA_NFRAMES_LO, lo));
        metadataNode.push_back(JSONNode(ATTR_METADATA_NFRAMES_HI, hi));
    }
    if (spMetadata->getTime() != Metadata::UNDEFINED_TIMESTAMP)
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
    for (auto fieldDesc = vFields.begin(); fieldDesc != vFields.end(); fieldDesc++)
    {
        Variant val = spMetadata->getFieldValue(fieldDesc->name);
        if (!val.isEmpty())
        {
            JSONNode metadataFieldNode(JSON_NODE);
            metadataFieldNode.push_back(JSONNode(ATTR_NAME, fieldDesc->name));
            metadataFieldNode.push_back(JSONNode(ATTR_VALUE, val.toString()));
            metadataFieldsArrayNode.push_back(metadataFieldNode);
        }
    }
    metadataNode.push_back(metadataFieldsArrayNode);

    auto refs = spMetadata->getAllReferences();
    if (!refs.empty())
    {
        JSONNode referencesArrayNode(JSON_ARRAY);
        referencesArrayNode.set_name(TAG_METADATA_REFERENCES_ARRAY);
        for (auto reference = refs.begin(); reference != refs.end(); reference++)
        {
            JSONNode referenceNode(JSON_NODE);
            referenceNode.set_name(TAG_METADATA_REFERENCE);
            referenceNode.push_back(JSONNode(ATTR_NAME, reference->getReferenceDescription()->name));
            referenceNode.push_back(JSONNode(ATTR_ID, reference->getReferenceMetadata().lock()->getId()));
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

/*
static void add(JSONNode& statNode, const Stat& stat)
{
    VMF_EXCEPTION(NotImplementedException, "Stat object serialization isn't yet implemented");
}
*/

std::string FormatJSON::store(
    const MetadataSet& set,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    //const std::vector<Stat>& stats,
    const AttribMap& attribs
    )
{
    JSONNode vmfRootNode(JSON_NODE);
    vmfRootNode.set_name(TAG_VMF);

    // attribs
    JSONNode attribsArrayNode(JSON_NODE);
    attribsArrayNode.set_name(TAG_ATTRIBS_ARRAY);
    for (const auto& a : attribs)
        attribsArrayNode.push_back(JSONNode(a.first, a.second));
    vmfRootNode.push_back(attribsArrayNode);

    // stats
    /*
    if(!stats.empty())
    JSONNode statsArrayNode(JSON_ARRAY);
    statsArrayNode.set_name(TAG_STATS_ARRAY);
    for (const auto& s : stats)
    {
        JSONNode statNode(JSON_NODE);
        add(statNode, s);
        statsArrayNode.push_back(statNode);
    }
    vmfRootNode.push_back(statsArrayNode);
    */

    // segments
    if (!segments.empty())
    {
        JSONNode segmentsArrayNode(JSON_ARRAY);
        segmentsArrayNode.set_name(TAG_VIDEO_SEGMENTS_ARRAY);
        for (const auto& spSegment : segments)
        {
            if (spSegment == nullptr) VMF_EXCEPTION(NullPointerException, "VideoSegment pointer is null");
            JSONNode segmentNode(JSON_NODE);
            add(segmentNode, spSegment);
            segmentsArrayNode.push_back(segmentNode);
        }
        vmfRootNode.push_back(segmentsArrayNode);
    }

    // schemas
    if (!schemas.empty())
    {
        JSONNode schemasArrayNode(JSON_ARRAY);
        schemasArrayNode.set_name(TAG_SCHEMAS_ARRAY);
        for (const auto& spSchema : schemas)
        {
            if (spSchema == nullptr) VMF_EXCEPTION(NullPointerException, "Schema pointer is null");
            JSONNode schemaNode(JSON_NODE);
            add(schemaNode, spSchema);
            schemasArrayNode.push_back(schemaNode);
        }
        vmfRootNode.push_back(schemasArrayNode);
    }

    // set
    if (!set.empty())
    {
        JSONNode metadataArrayNode(JSON_ARRAY);
        metadataArrayNode.set_name(TAG_METADATA_ARRAY);
        for (const auto& spMetadata : set)
        {
            if (spMetadata == nullptr) VMF_EXCEPTION(NullPointerException, "Metadata pointer is null");
            JSONNode metadataNode(JSON_NODE);
            add(metadataNode, spMetadata);
            metadataArrayNode.push_back(metadataNode);
        }
        vmfRootNode.push_back(metadataArrayNode);
    }

    JSONNode root(JSON_NODE);
    root.push_back(vmfRootNode);

    std::string formatted = root.write_formatted();
    return formatted;
}

/*
** parse() support
*/

static std::shared_ptr<MetadataSchema> parseSchemaFromNode(const JSONNode& schemaNode)
{
    std::shared_ptr<vmf::MetadataSchema> spSchema;
    auto schemaNameIter = schemaNode.find(ATTR_NAME);
    auto schemaAuthorIter = schemaNode.find(ATTR_SCHEMA_AUTHOR);
    if (schemaNameIter != schemaNode.end())
    {
        if (schemaAuthorIter != schemaNode.end())
            spSchema = std::make_shared<vmf::MetadataSchema>(schemaNameIter->as_string(), schemaAuthorIter->as_string());
        else
            spSchema = std::make_shared<vmf::MetadataSchema>(schemaNameIter->as_string());
    }
    else
        VMF_EXCEPTION(IncorrectParamException, "Schema has no name");

    auto descsArrayIter = schemaNode.find(TAG_DESCRIPTIONS_ARRAY);
    if (descsArrayIter == schemaNode.end())
        VMF_EXCEPTION(IncorrectParamException, "Can't find descriptions-array JSON node");

    std::shared_ptr<vmf::MetadataDesc> spDesc;
    for (auto descNode = descsArrayIter->begin(); descNode != descsArrayIter->end(); descNode++)
    {
        auto descNameIter = descNode->find(ATTR_NAME);
        if (descNameIter == descNode->end())
            VMF_EXCEPTION(IncorrectParamException, "Description has no name");

        auto fieldsArrayIter = descNode->find(TAG_FIELDS_ARRAY);
        if (fieldsArrayIter == descNode->end())
            VMF_EXCEPTION(IncorrectParamException, "Description has no fields array");

        std::vector<FieldDesc> vFields;
        for (auto fieldNode = fieldsArrayIter->begin(); fieldNode != fieldsArrayIter->end(); fieldNode++)
        {
            auto fieldNameIter = fieldNode->find(ATTR_NAME);
            auto fieldTypeIter = fieldNode->find(ATTR_FIELD_TYPE);
            if (fieldNameIter == fieldNode->end() || fieldTypeIter == fieldNode->end())
                VMF_EXCEPTION(IncorrectParamException, "Field has no 'name' or 'type' attribute");
            vmf::Variant::Type field_type = Variant::typeFromString(fieldTypeIter->as_string());
            bool field_optional = false;
            auto fieldsOptionalityIter = fieldNode->find(ATTR_FIELD_OPTIONAL);
            if (fieldsOptionalityIter != fieldNode->end())
            {
                if (fieldsOptionalityIter->as_string() == "true")
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

static std::shared_ptr<MetadataInternal> parseMetadataFromNode(const JSONNode& metadataNode, const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    auto schemaIter = metadataNode.find(ATTR_METADATA_SCHEMA);
    auto descIter = metadataNode.find(ATTR_METADATA_DESCRIPTION);
    auto idIter = metadataNode.find(ATTR_ID);
    if (schemaIter == metadataNode.end() || descIter == metadataNode.end() || idIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata item has no schema name, description name or id");

    auto schema = schemas.begin();
    for (; schema != schemas.end(); schema++)
        if ((*schema)->getName() == schemaIter->as_string())
            break;

    if (schema == schemas.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Unknown schema for metadata item");

    std::shared_ptr<MetadataDesc> spDesc;
    if ((spDesc = (*schema)->findMetadataDesc(descIter->as_string())) == nullptr)
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
    if (metadataFieldsArrayIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "No metadata fields array");

    for (auto fieldNode = metadataFieldsArrayIter->begin(); fieldNode != metadataFieldsArrayIter->end(); fieldNode++)
    {
        FieldDesc fieldDesc;
        auto fieldNameIter = fieldNode->find(ATTR_NAME);
        auto fieldValueIter = fieldNode->find(ATTR_VALUE);
        if (fieldNameIter == fieldNode->end() || fieldValueIter == fieldNode->end())
            VMF_EXCEPTION(vmf::IncorrectParamException, "Missing field name or field value");

        vmf::Variant field_value;
        spDesc->getFieldDesc(fieldDesc, fieldNameIter->as_string());
        field_value.fromString(fieldDesc.type, fieldValueIter->as_string());
        spMetadataInternal->setFieldValue(fieldNameIter->as_string(), field_value);
    }

    auto referencesArrayIter = metadataNode.find(TAG_METADATA_REFERENCES_ARRAY);
    if (referencesArrayIter != metadataNode.end())
    {
        for (auto referenceNode = referencesArrayIter->begin(); referenceNode != referencesArrayIter->end(); referenceNode++)
        {
            auto referenceIdIter = referenceNode->find(ATTR_ID);
            if (referenceIdIter == referenceNode->end()) VMF_EXCEPTION(vmf::IncorrectParamException, "Missing reference 'id'");

            auto referenceNameIter = referenceNode->find(ATTR_NAME);
            if (referenceNameIter == referenceNode->end()) VMF_EXCEPTION(vmf::IncorrectParamException, "Missing reference 'name'");

            spMetadataInternal->vRefs.push_back(std::make_pair(IdType(referenceIdIter->as_int()), referenceNameIter->as_string()));
        }
    }

    return spMetadataInternal;
}

static std::shared_ptr<MetadataStream::VideoSegment> parseVideoSegmentFromNode(const JSONNode& segmentNode)
{
    auto segmentTitleIter = segmentNode.find(ATTR_SEGMENT_TITLE);
    auto segmentFPSIter = segmentNode.find(ATTR_SEGMENT_FPS);
    auto segmentTimeIter = segmentNode.find(ATTR_SEGMENT_TIME);
    auto segmentDurationIter = segmentNode.find(ATTR_SEGMENT_DURATION);
    auto segmentWidthIter = segmentNode.find(ATTR_SEGMENT_WIDTH);
    auto segmentHeightIter = segmentNode.find(ATTR_SEGMENT_HEIGHT);

    if (segmentTitleIter == segmentNode.end())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no title");
    if (segmentFPSIter == segmentNode.end())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no fps value");
    if (segmentTimeIter == segmentNode.end())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no time value");

    std::string title = segmentTitleIter->as_string();
    double fps = segmentFPSIter->as_float();
    long long timestamp = segmentTimeIter->as_int();

    if (title.empty())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid title");
    if (fps <= 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid fps value");
    if (timestamp < 0)
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has invalid time value");

    std::shared_ptr<MetadataStream::VideoSegment> spSegment(new MetadataStream::VideoSegment(title, fps, timestamp));

    if (segmentDurationIter != segmentNode.end())
    {
        long long duration = segmentDurationIter->as_int();
        if (duration > 0)
            spSegment->setDuration(duration);
    }
    if (segmentWidthIter != segmentNode.end() && segmentHeightIter != segmentNode.end())
    {
        long width = segmentWidthIter->as_int(), height = segmentHeightIter->as_int();
        if (width > 0 && height > 0)
            spSegment->setResolution(width, height);
    }

    return spSegment;
}

/*
static Stat parseStatFromNode(const JSONNode& statNode)
{
    VMF_EXCEPTION(NotImplementedException, "parseStatFromNode NYI");
}
*/

std::array<int, 5> FormatJSON::parse(
    const std::string& text,
    std::vector<std::shared_ptr<MetadataInternal>>& metadata,
    std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    //std::vector<Stat>& stats,
    AttribMap& attribs // nextId, checksum, etc
    )
{
    std::array<int, 5> counter = {};
    if (text.empty()) VMF_EXCEPTION(IncorrectParamException, "Empty input JSON string");

    JSONNode root;
    try
    {
        root = libjson::parse(text);
    }
    catch (...)
    {
        VMF_EXCEPTION(IncorrectParamException, "Can't get JSON root");
    }
    if (root.size() != 1) VMF_EXCEPTION(IncorrectParamException, "More than one JSON root");

    JSONNode vmfNode = root[0];
    if (vmfNode.name() != TAG_VMF) VMF_LOG_ERROR("Unexpected root JSON element: " + vmfNode.name());
    for (const auto& node : vmfNode)
    {
        if (node.name() == TAG_ATTRIBS_ARRAY)
        {
            for (const auto& at : node)
                attribs[at.name()] = at.as_string(), counter[4]++;
        }
        else if (node.name() == TAG_STATS_ARRAY)
        {
            for (const auto& st : node)
                ;//stats.push_back(parseStatFromNode(st)), counter[3]++;
        }
        else if (node.name() == TAG_VIDEO_SEGMENTS_ARRAY)
        {
            for (const auto& seg : node)
                segments.push_back(parseVideoSegmentFromNode(seg)), counter[2]++;
        }
        else if (node.name() == TAG_SCHEMAS_ARRAY)
        {
            for (const auto& sc : node)
                schemas.push_back(parseSchemaFromNode(sc)), counter[1]++;
        }
        else if (node.name() == TAG_METADATA_ARRAY)
        {
            for (const auto& m : node)
                metadata.push_back(parseMetadataFromNode(m, schemas)), counter[0]++;
        }
        else VMF_LOG_ERROR("Unexpected JSON element: " + node.name());
    }

    return counter;
}

}//vmf
