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

#include "xmpstatsource.hpp"

#define VMF_STATISTICS               "statistics"

#define VMF_STAT                     "stat"

#define VMF_STAT_NAME                "name"
#define VMF_STAT_UPDATE_MODE         "update_mode"

#define VMF_STAT_FIELD               "field"

#define VMF_STAT_FIELD_NAME          "name"
#define VMF_STAT_FIELD_SCHEMA_NAME   "schema_name"
#define VMF_STAT_FIELD_METADATA_NAME "metadata_name"
#define VMF_STAT_FIELD_FIELD_NAME    "field_name"
#define VMF_STAT_FIELD_OP_NAME       "op_name"

using namespace std;
using namespace vmf;

XMPStatSource::XMPStatSource(const std::shared_ptr<SXMPMeta>& statStorage)
    : metadata(statStorage)
{
}

XMPStatSource::~XMPStatSource()
{
}

void XMPStatSource::save(const std::vector< Stat* >& stats)
{
    metadata->DeleteProperty(VMF_NS, VMF_STATISTICS);

    MetaString pathToStatData;
    metadata->AppendArrayItem(VMF_NS, VMF_STATISTICS, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);
    metadata->SetStructField(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, nullptr, kXMP_PropValueIsArray);

    std::for_each(stats.begin(), stats.end(), [&](const Stat* stat)
    {
        MetaString pathToStatArray;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, &pathToStatArray);
        metadata->AppendArrayItem(VMF_NS, pathToStatArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        MetaString pathToStat;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToStatArray.c_str(), kXMP_ArrayLastItem, &pathToStat);

        MetaString tmpPath;

        if (stat->getName().empty())
            VMF_EXCEPTION(DataStorageException, "Invalid stat object: name is invalid!");

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_NAME, &tmpPath);
        metadata->SetProperty(VMF_NS, tmpPath.c_str(), stat->getName().c_str());

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_UPDATE_MODE, &tmpPath);
        metadata->SetProperty(VMF_NS, tmpPath.c_str(), StatUpdateMode::toString(stat->getUpdateMode()).c_str());

        std::vector< std::string > fieldNames = stat->getAllFieldNames();
        std::for_each(fieldNames.begin(), fieldNames.end(), [&](const std::string& fieldName)
        {
            const StatField& field = stat->getField(fieldName);

            MetaString pathToFieldArray;
            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_FIELD, &pathToFieldArray);
            metadata->AppendArrayItem(VMF_NS, pathToFieldArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
            MetaString pathToField;
            SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToFieldArray.c_str(), kXMP_ArrayLastItem, &pathToField);

            if (field.getName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field name is invalid!");
            if (field.getFieldName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata field name is invalid!");
            if (field.getOpName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field operation name is invalid!");

            std::shared_ptr< MetadataDesc > metadataDesc = field.getMetadataDesc();
            if (metadataDesc == nullptr)
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata descriptor is null!");
            if (metadataDesc->getSchemaName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata schema name is invalid!");
            if (metadataDesc->getMetadataName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata name is invalid!");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_SCHEMA_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), metadataDesc->getSchemaName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_METADATA_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), metadataDesc->getMetadataName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_FIELD_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getFieldName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_OP_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getOpName().c_str());
        });
    });
}

void XMPStatSource::load(MetadataStream& stream)
{
    MetaString pathToStatData;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);

    MetaString pathToStatArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, &pathToStatArray);

    SXMPIterator statsIter(*metadata, VMF_NS, pathToStatArray.c_str(), kXMP_IterJustChildren);
    MetaString pathToStat;
    while (statsIter.Next(nullptr, &pathToStat))
    {
        MetaString tmpPath, statName, tmpStr;
        StatUpdateMode::Type updateMode;

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_NAME, &tmpPath);
        if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &statName, 0) )
            VMF_EXCEPTION(DataStorageException, "Broken stat name");

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_UPDATE_MODE, &tmpPath);
        if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &tmpStr, 0) )
            VMF_EXCEPTION(DataStorageException, "Broken stat update mode");
        updateMode = StatUpdateMode::fromString(tmpStr);

        std::vector< StatField > fields;

        MetaString pathToFieldArray;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_FIELD, &pathToFieldArray);

        SXMPIterator fieldsIter(*metadata, VMF_NS, pathToFieldArray.c_str(), kXMP_IterJustChildren);
        MetaString pathToField;
        while (fieldsIter.Next(nullptr, &pathToField))
        {
            MetaString fieldName, schemaName, metadataName, metadataFieldName, opName;

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_NAME, &tmpPath);
            if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &fieldName, 0) )
                VMF_EXCEPTION(DataStorageException, "Broken stat field name");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_SCHEMA_NAME, &tmpPath);
            if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &schemaName, 0) )
                VMF_EXCEPTION(DataStorageException, "Broken stat field metadata schema name");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_METADATA_NAME, &tmpPath);
            if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &metadataName, 0) )
                VMF_EXCEPTION(DataStorageException, "Broken stat field metadata name");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_FIELD_NAME, &tmpPath);
            if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &metadataFieldName, 0) )
                VMF_EXCEPTION(DataStorageException, "Broken stat field metadata field name");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_OP_NAME, &tmpPath);
            if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &opName, 0) )
                VMF_EXCEPTION(DataStorageException, "Broken stat field operation name");

            fields.push_back(StatField(fieldName, schemaName, metadataName, metadataFieldName, opName));
        }

        stream.addStat(statName, fields, updateMode);
    }
}

void XMPStatSource::clear()
{
    metadata->DeleteProperty(VMF_NS, VMF_STATISTICS);
}

