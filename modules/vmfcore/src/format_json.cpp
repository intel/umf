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

std::shared_ptr<Format> FormatJSON::getBackendFormat()
{
    return std::make_shared<FormatJSON>();
}

/*
** store() support
*/

static void add(JSONNode& schemaNode, const std::shared_ptr<MetadataSchema>& spSchema)
{
    schemaNode.push_back(JSONNode(ATTR_NAME, spSchema->getName()));
    schemaNode.push_back(JSONNode(ATTR_SCHEMA_AUTHOR, spSchema->getAuthor()));
    if(spSchema->getUseEncryption())
    {
        schemaNode.push_back( JSONNode(ATTR_ENCRYPTED_BOOL, "true") );
    }

    JSONNode descsArrayNode(JSON_ARRAY);
    descsArrayNode.set_name(TAG_DESCRIPTIONS_ARRAY);
    auto vDescs = spSchema->getAll();
    for (auto spDescriptor = vDescs.begin(); spDescriptor != vDescs.end(); spDescriptor++)
    {
        JSONNode descNode(JSON_NODE);
        descNode.push_back(JSONNode(ATTR_NAME, (*spDescriptor)->getMetadataName()));
        if(spDescriptor->get()->getUseEncryption())
        {
            descNode.push_back( JSONNode(ATTR_ENCRYPTED_BOOL, "true") );
        }

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
            if(fieldDesc->useEncryption)
                fieldNode.push_back(JSONNode(ATTR_ENCRYPTED_BOOL, "true"));

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
    metadataNode.push_back(JSONNode(ATTR_ID_HI, (unsigned long)(spMetadata->getId()>>32)));
    metadataNode.push_back(JSONNode(ATTR_ID_LO, (unsigned long)spMetadata->getId()));
    const std::string& encMetadata = spMetadata->getEncryptedData();
    if(!encMetadata.empty())
    {
        metadataNode.push_back( JSONNode(ATTR_ENCRYPTED_DATA, encMetadata) );
    }
    if(spMetadata->getUseEncryption())
    {
        metadataNode.push_back( JSONNode(ATTR_ENCRYPTED_BOOL, "true") );
    }
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
        auto fieldIt = spMetadata->findField(fieldDesc->name);
        if(fieldIt != spMetadata->end())
        {
            Variant val = spMetadata->getFieldValue(fieldDesc->name);
            const std::string& encData = fieldIt->getEncryptedData();
            if(!val.isEmpty() || !encData.empty())
            {
                JSONNode metadataFieldNode(JSON_NODE);
                metadataFieldNode.push_back( JSONNode(ATTR_NAME, fieldDesc->name) );
                if (!val.isEmpty())
                {
                    metadataFieldNode.push_back( JSONNode(ATTR_VALUE, val.toString()) );
                }
                if(fieldIt->getUseEncryption())
                {
                    metadataFieldNode.push_back( JSONNode(ATTR_ENCRYPTED_BOOL, "true") );
                }
                if(!encData.empty())
                {
                    metadataFieldNode.push_back( JSONNode(ATTR_ENCRYPTED_DATA, encData) );
                }
                metadataFieldsArrayNode.push_back(metadataFieldNode);
            }
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

static void add(JSONNode& statNode, std::shared_ptr<Stat> stat)
{
    if (stat->getName().empty())
        VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: name is invalid!");

    statNode.push_back(JSONNode(ATTR_STAT_NAME, stat->getName()));

    std::vector< std::string > fieldNames = stat->getAllFieldNames();
    if (!fieldNames.empty())
    {
        JSONNode fieldsArrayNode(JSON_ARRAY);
        fieldsArrayNode.set_name(TAG_STAT_FIELDS_ARRAY);

        for(auto& fieldName : fieldNames)
        {
            const StatField& field = stat->getField(fieldName);

            if (field.getName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field name is invalid!");
            if (field.getFieldName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata field name is invalid!");
            if (field.getOpName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field operation name is invalid!");

            /*std::shared_ptr< MetadataDesc > metadataDesc = field.getMetadataDesc();
            if (metadataDesc == nullptr)
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata descriptor is null!");*/
            if (field.getSchemaName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata schema name is invalid!");
            if (field.getMetadataName().empty())
                VMF_EXCEPTION(IncorrectParamException, "Invalid stat object: field metadata name is invalid!");

            JSONNode fieldNode(JSON_NODE);
            fieldNode.set_name(TAG_STAT_FIELD);

            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_NAME, field.getName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_SCHEMA_NAME, field.getSchemaName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_METADATA_NAME, field.getMetadataName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_FIELD_NAME, field.getFieldName()));
            fieldNode.push_back(JSONNode(ATTR_STAT_FIELD_OP_NAME, field.getOpName()));

            fieldsArrayNode.push_back(fieldNode);
        }

        statNode.push_back(fieldsArrayNode);
    }
}

std::string FormatJSON::store(
    const MetadataSet& set,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    const std::vector<std::shared_ptr<Stat>>& stats,
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
    if(!stats.empty())
    {
        JSONNode statsArrayNode(JSON_ARRAY);
        statsArrayNode.set_name(TAG_STATS_ARRAY);
        for (const auto& s : stats)
        {
            JSONNode statNode(JSON_NODE);
            add(statNode, s);
            statsArrayNode.push_back(statNode);
        }
        vmfRootNode.push_back(statsArrayNode);
    }

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
        //check if all the metadata records have corresponding schemas
        //shouldn't be checked when schemas are empty
        //for cases when user passes metadata records only
        for(const std::shared_ptr<Metadata>& spMetadata : set)
        {
            if (spMetadata == nullptr)
                VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata pointer is null");

            bool noSchemaForMetadata = true;
            for(const std::shared_ptr<MetadataSchema>& spSchema : schemas)
            {
                if (spSchema == nullptr)
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Schema pointer is null");

                if(spMetadata->getSchemaName() == spSchema->getName())
                    noSchemaForMetadata = false;
            }
            if(noSchemaForMetadata)
                VMF_EXCEPTION(vmf::IncorrectParamException, "MetadataSet item references unknown schema");
        }

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
    auto schemaEncryptedIter = schemaNode.find(ATTR_ENCRYPTED_BOOL);
    if (schemaNameIter != schemaNode.end())
    {
        std::string schemaName = schemaNameIter->as_string();
        std::string schemaAuthor;
        bool schemaUseEncryption = false;
        if(schemaAuthorIter != schemaNode.end())
            schemaAuthor = schemaAuthorIter->as_string();
        if(schemaEncryptedIter != schemaNode.end())
            schemaUseEncryption = (schemaEncryptedIter->as_string() == "true");

        spSchema = std::make_shared<vmf::MetadataSchema>(schemaName, schemaAuthor, schemaUseEncryption);
    }
    else
        VMF_EXCEPTION(IncorrectParamException, "Schema has no name");

    auto descsArrayIter = schemaNode.find(TAG_DESCRIPTIONS_ARRAY);
    if (descsArrayIter == schemaNode.end())
        VMF_EXCEPTION(IncorrectParamException, "Can't find descriptions-array JSON node");

    std::shared_ptr<vmf::MetadataDesc> spDesc;
    for (auto descNode = descsArrayIter->begin(); descNode != descsArrayIter->end(); descNode++)
    {
        std::string descName;
        auto descNameIter = descNode->find(ATTR_NAME);
        if (descNameIter != descNode->end())
            descName = descNameIter->as_string();
        else
            VMF_EXCEPTION(IncorrectParamException, "Description has no name");
        bool descUseEncryption = false;
        auto descEncryptedIter = descNode->find(ATTR_ENCRYPTED_BOOL);
        if(descEncryptedIter != descNode->end())
            descUseEncryption = (descEncryptedIter->as_string() == "true");

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
            bool field_use_encryption = false;
            auto fieldUseEncryptionIter = fieldNode->find(ATTR_ENCRYPTED_BOOL);
            if(fieldUseEncryptionIter != fieldNode->end())
                field_use_encryption = fieldUseEncryptionIter->as_string() == "true";
            vFields.push_back(FieldDesc(fieldNameIter->as_string(), field_type, field_optional,
                                        field_use_encryption));
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

        spDesc = std::make_shared<vmf::MetadataDesc>(descNameIter->as_string(), vFields, vReferences, descUseEncryption);
        spSchema->add(spDesc);
    }

    return spSchema;
}

static MetadataInternal parseMetadataFromNode(const JSONNode& metadataNode)
{
    auto schemaIter = metadataNode.find(ATTR_METADATA_SCHEMA);
    auto descIter   = metadataNode.find(ATTR_METADATA_DESCRIPTION);
    auto idIter     = metadataNode.find(ATTR_ID);
    if (schemaIter == metadataNode.end() || descIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "Metadata item has no schema name or description name");

    std::string encryptedMetadata;
    bool metadataUseEncryption = false;
    auto mdEncryptedBoolIter = metadataNode.find(ATTR_ENCRYPTED_BOOL);
    if(mdEncryptedBoolIter != metadataNode.end())
        metadataUseEncryption = mdEncryptedBoolIter->as_string() == "true";
    auto mdEncryptedDataIter = metadataNode.find(ATTR_ENCRYPTED_DATA);
    if(mdEncryptedDataIter != metadataNode.end())
        encryptedMetadata = mdEncryptedDataIter->as_string();
    if(metadataUseEncryption && encryptedMetadata.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "No encrypted data presented while the flag is set on");

    MetadataInternal mdi(descIter->as_string(), schemaIter->as_string());
    if(idIter != metadataNode.end())
        mdi.id = idIter->as_int();
    else
    {
        auto idLoIter = metadataNode.find(ATTR_ID_LO);
        auto idHiIter = metadataNode.find(ATTR_ID_HI);
                if (idLoIter != metadataNode.end() && idHiIter != metadataNode.end())
        {
            unsigned long lo = idLoIter->as_int();
            unsigned long hi = idHiIter->as_int();
            long long id = ((long long)hi << 32) | lo;
            mdi.id = id;
        }
    }

    mdi.useEncryption = metadataUseEncryption;
    mdi.encryptedData = encryptedMetadata;

    auto frameIdxLoIter = metadataNode.find(ATTR_METADATA_FRAME_IDX_LO);
    auto frameIdxHiIter = metadataNode.find(ATTR_METADATA_FRAME_IDX_HI);
    auto nFramesLoIter  = metadataNode.find(ATTR_METADATA_NFRAMES_LO);
    auto nFramesHiIter  = metadataNode.find(ATTR_METADATA_NFRAMES_HI);
    auto timeLoIter     = metadataNode.find(ATTR_METADATA_TIMESTAMP_LO);
    auto timeHiIter     = metadataNode.find(ATTR_METADATA_TIMESTAMP_HI);
    auto durationLoIter = metadataNode.find(ATTR_METADATA_DURATION_LO);
    auto durationHiIter = metadataNode.find(ATTR_METADATA_DURATION_HI);

    if (frameIdxLoIter != metadataNode.end() && frameIdxHiIter != metadataNode.end())
    {
        unsigned long lo = frameIdxLoIter->as_int();
        unsigned long hi = frameIdxHiIter->as_int();
        long long frmIdx = ((long long)hi << 32) | lo;
        mdi.frameIndex = frmIdx;

        if (nFramesLoIter != metadataNode.end() && nFramesHiIter != metadataNode.end())
        {
            unsigned long lo = nFramesLoIter->as_int();
            unsigned long hi = nFramesHiIter->as_int();
            long long numFrm = ((long long)hi << 32) | lo;
            mdi.frameNum = numFrm;
        }
    }
    if (timeLoIter != metadataNode.end() && timeHiIter != metadataNode.end())
    {
        unsigned long lo = timeLoIter->as_int();
        unsigned long hi = timeHiIter->as_int();
        long long time = ((long long)hi << 32) | lo;
        mdi.timestamp = time;

        if (durationLoIter != metadataNode.end() && durationHiIter != metadataNode.end())
        {
            unsigned long lo = durationLoIter->as_int();
            unsigned long hi = durationHiIter->as_int();
            long long dur = ((long long)hi << 32) | lo;
            mdi.duration = dur;
        }
    }

    auto metadataFieldsArrayIter = metadataNode.find(TAG_FIELDS_ARRAY);
    if (metadataFieldsArrayIter == metadataNode.end())
        VMF_EXCEPTION(vmf::IncorrectParamException, "No metadata fields array");

    for (auto fieldNode = metadataFieldsArrayIter->begin(); fieldNode != metadataFieldsArrayIter->end(); fieldNode++)
    {
        FieldDesc fieldDesc;
        auto fieldNameIter = fieldNode->find(ATTR_NAME);
        auto fieldValueIter = fieldNode->find(ATTR_VALUE);
        if(fieldNameIter == fieldNode->end())
            VMF_EXCEPTION(vmf::IncorrectParamException, "Missing field name");
        std::string fieldName = fieldNameIter->as_string();
        std::string fieldValueString;
        if(fieldValueIter != fieldNode->end())
            fieldValueString = fieldValueIter->as_string();

        std::string encryptedFieldData;
        bool fieldUseEncryption = false;
        auto fieldEncryptedBoolIter = fieldNode->find(ATTR_ENCRYPTED_BOOL);
        if(fieldEncryptedBoolIter != fieldNode->end())
            fieldUseEncryption = fieldEncryptedBoolIter->as_string() == "true";
        auto fieldEncryptedDataIter = fieldNode->find(ATTR_ENCRYPTED_DATA);
        if(fieldEncryptedDataIter != fieldNode->end())
            encryptedFieldData = fieldEncryptedDataIter->as_string();
        if(fieldUseEncryption && encryptedFieldData.empty())
            VMF_EXCEPTION(vmf::IncorrectParamException, "No encrypted data presented while the flag is set on");

        if(fieldValueString.empty() && encryptedFieldData.empty())
            VMF_EXCEPTION(vmf::IncorrectParamException, "Missing field value or encrypted data");
        
        mdi.fields[fieldName].value         = fieldValueString;
        mdi.fields[fieldName].useEncryption = fieldUseEncryption;
        mdi.fields[fieldName].encryptedData = encryptedFieldData;
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

            mdi.refs.push_back(std::make_pair(IdType(referenceIdIter->as_int()), referenceNameIter->as_string()));
        }
    }

    return mdi;
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

static std::shared_ptr<Stat> parseStatFromNode(const JSONNode& statNode)
{
    auto statNameIter = statNode.find(ATTR_STAT_NAME);

    if(statNameIter == statNode.end())
        VMF_EXCEPTION(vmf::InternalErrorException, "JSON element has no stat name");

    std::string statName = statNameIter->as_string();
    const Stat::UpdateMode::Type updateMode = Stat::UpdateMode::Disabled;

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

    return std::make_shared<Stat>(statName, fields, updateMode);
}

Format::ParseCounters FormatJSON::parse(
    const std::string& text,
    std::vector<MetadataInternal>& metadata,
    std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    std::vector<std::shared_ptr<Stat>>& stats,
    AttribMap& attribs // nextId, checksum, etc
    )
{
    Format::ParseCounters counter = {};
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
                attribs[at.name()] = at.as_string(), counter.attribs++;
        }
        else if (node.name() == TAG_STATS_ARRAY)
        {
            for (const auto& st : node)
                stats.push_back(parseStatFromNode(st)), counter.stats++;
        }
        else if (node.name() == TAG_VIDEO_SEGMENTS_ARRAY)
        {
            for (const auto& seg : node)
                segments.push_back(parseVideoSegmentFromNode(seg)), counter.segments++;
        }
        else if (node.name() == TAG_SCHEMAS_ARRAY)
        {
            for (const auto& sc : node)
                schemas.push_back(parseSchemaFromNode(sc)), counter.schemas++;
        }
        else if (node.name() == TAG_METADATA_ARRAY)
        {
            for (const auto& m : node)
                metadata.push_back(parseMetadataFromNode(m)), counter.metadata++;
        }
        else VMF_LOG_ERROR("Unexpected JSON element: " + node.name());
    }

    return counter;
}

}//vmf
