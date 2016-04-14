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

#define VMF_STAT_FIELD               "field"

#define VMF_STAT_FIELD_NAME          "name"
#define VMF_STAT_FIELD_SCHEMA_NAME   "schema_name"
#define VMF_STAT_FIELD_METADATA_NAME "metadata_name"
#define VMF_STAT_FIELD_FIELD_NAME    "field_name"
#define VMF_STAT_FIELD_OP_NAME       "op_name"

using namespace std;
using namespace vmf;

XMPStatSource::XMPStatSource(const std::shared_ptr<SXMPMeta> statStorage)
    : metadata(statStorage)
{
}

XMPStatSource::~XMPStatSource()
{
}

void XMPStatSource::save(const std::vector< std::shared_ptr<Stat> >& stats)
{
    metadata->DeleteProperty(VMF_NS, VMF_STATISTICS);

    vmf_string pathToStatData;
    metadata->AppendArrayItem(VMF_NS, VMF_STATISTICS, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);
    metadata->SetStructField(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, nullptr, kXMP_PropValueIsArray);

    for( auto& stat : stats )
    {
        vmf_string pathToStatArray;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, &pathToStatArray);
        metadata->AppendArrayItem(VMF_NS, pathToStatArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        vmf_string pathToStat;
        SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToStatArray.c_str(), kXMP_ArrayLastItem, &pathToStat);

        vmf_string tmpPath;

        if (stat->getName().empty())
            VMF_EXCEPTION(DataStorageException, "Invalid stat object: name is invalid!");

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_NAME, &tmpPath);
        metadata->SetProperty(VMF_NS, tmpPath.c_str(), stat->getName().c_str());

        std::vector< std::string > fieldNames = stat->getAllFieldNames();
        for( auto fieldName : fieldNames )
        {
            const StatField& field = stat->getField(fieldName);

            vmf_string pathToFieldArray;
            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_FIELD, &pathToFieldArray);
            metadata->AppendArrayItem(VMF_NS, pathToFieldArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
            vmf_string pathToField;
            SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToFieldArray.c_str(), kXMP_ArrayLastItem, &pathToField);

            if (field.getName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field name is invalid!");
            if (field.getFieldName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata field name is invalid!");
            if (field.getOpName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field operation name is invalid!");

            if (field.getSchemaName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata schema name is invalid!");
            if (field.getMetadataName().empty())
                VMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata name is invalid!");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_SCHEMA_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getSchemaName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_METADATA_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getMetadataName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_FIELD_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getFieldName().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToField.c_str(), VMF_NS, VMF_STAT_FIELD_OP_NAME, &tmpPath);
            metadata->SetProperty(VMF_NS, tmpPath.c_str(), field.getOpName().c_str());
        }
    }
}

void XMPStatSource::load(std::vector< std::shared_ptr<Stat> >& stats)
{
    vmf_string pathToStatData;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);

    vmf_string pathToStatArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStatData.c_str(), VMF_NS, VMF_STAT, &pathToStatArray);

    SXMPIterator statsIter(*metadata, VMF_NS, pathToStatArray.c_str(), kXMP_IterJustChildren);
    vmf_string pathToStat;
    while (statsIter.Next(nullptr, &pathToStat))
    {
        vmf_string tmpPath, statName, tmpStr;
        const Stat::UpdateMode::Type updateMode = Stat::UpdateMode::Disabled;

        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_NAME, &tmpPath);
        if(!metadata->GetProperty(VMF_NS, tmpPath.c_str(), &statName, 0) )
            VMF_EXCEPTION(DataStorageException, "Broken stat name");

        std::vector< StatField > fields;

        vmf_string pathToFieldArray;
        SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToStat.c_str(), VMF_NS, VMF_STAT_FIELD, &pathToFieldArray);

        SXMPIterator fieldsIter(*metadata, VMF_NS, pathToFieldArray.c_str(), kXMP_IterJustChildren);
        vmf_string pathToField;
        while (fieldsIter.Next(nullptr, &pathToField))
        {
            vmf_string fieldName, schemaName, metadataName, metadataFieldName, opName;

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

        stats.emplace_back(make_shared<Stat>(statName, fields, updateMode));
    }
}

void XMPStatSource::clear()
{
    metadata->DeleteProperty(VMF_NS, VMF_STATISTICS);
}

