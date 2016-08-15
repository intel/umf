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

#define UMF_STATISTICS               "statistics"

#define UMF_STAT                     "stat"

#define UMF_STAT_NAME                "name"

#define UMF_STAT_FIELD               "field"

#define UMF_STAT_FIELD_NAME          "name"
#define UMF_STAT_FIELD_SCHEMA_NAME   "schema_name"
#define UMF_STAT_FIELD_METADATA_NAME "metadata_name"
#define UMF_STAT_FIELD_FIELD_NAME    "field_name"
#define UMF_STAT_FIELD_OP_NAME       "op_name"

using namespace std;
using namespace umf;

XMPStatSource::XMPStatSource(const std::shared_ptr<SXMPMeta> statStorage)
    : metadata(statStorage)
{
}

XMPStatSource::~XMPStatSource()
{
}

void XMPStatSource::save(const std::vector< std::shared_ptr<Stat> >& stats)
{
    metadata->DeleteProperty(UMF_NS, UMF_STATISTICS);

    umf_string pathToStatData;
    metadata->AppendArrayItem(UMF_NS, UMF_STATISTICS, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
    SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);
    metadata->SetStructField(UMF_NS, pathToStatData.c_str(), UMF_NS, UMF_STAT, nullptr, kXMP_PropValueIsArray);

    for( auto& stat : stats )
    {
        umf_string pathToStatArray;
        SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStatData.c_str(), UMF_NS, UMF_STAT, &pathToStatArray);
        metadata->AppendArrayItem(UMF_NS, pathToStatArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
        umf_string pathToStat;
        SXMPUtils::ComposeArrayItemPath(UMF_NS, pathToStatArray.c_str(), kXMP_ArrayLastItem, &pathToStat);

        umf_string tmpPath;

        if (stat->getName().empty())
            UMF_EXCEPTION(DataStorageException, "Invalid stat object: name is invalid!");

        SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStat.c_str(), UMF_NS, UMF_STAT_NAME, &tmpPath);
        metadata->SetProperty(UMF_NS, tmpPath.c_str(), stat->getName().c_str());

        std::vector< std::string > fieldNames = stat->getAllFieldNames();
        for( auto fieldName : fieldNames )
        {
            const StatField& field = stat->getField(fieldName);

            umf_string pathToFieldArray;
            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStat.c_str(), UMF_NS, UMF_STAT_FIELD, &pathToFieldArray);
            metadata->AppendArrayItem(UMF_NS, pathToFieldArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
            umf_string pathToField;
            SXMPUtils::ComposeArrayItemPath(UMF_NS, pathToFieldArray.c_str(), kXMP_ArrayLastItem, &pathToField);

            if (field.getName().empty())
                UMF_EXCEPTION(DataStorageException, "Invalid stat object: field name is invalid!");
            if (field.getFieldName().empty())
                UMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata field name is invalid!");
            if (field.getOpName().empty())
                UMF_EXCEPTION(DataStorageException, "Invalid stat object: field operation name is invalid!");

            if (field.getSchemaName().empty())
                UMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata schema name is invalid!");
            if (field.getMetadataName().empty())
                UMF_EXCEPTION(DataStorageException, "Invalid stat object: field metadata name is invalid!");

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_NAME, &tmpPath);
            metadata->SetProperty(UMF_NS, tmpPath.c_str(), field.getName().c_str());

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_SCHEMA_NAME, &tmpPath);
            metadata->SetProperty(UMF_NS, tmpPath.c_str(), field.getSchemaName().c_str());

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_METADATA_NAME, &tmpPath);
            metadata->SetProperty(UMF_NS, tmpPath.c_str(), field.getMetadataName().c_str());

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_FIELD_NAME, &tmpPath);
            metadata->SetProperty(UMF_NS, tmpPath.c_str(), field.getFieldName().c_str());

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_OP_NAME, &tmpPath);
            metadata->SetProperty(UMF_NS, tmpPath.c_str(), field.getOpName().c_str());
        }
    }
}

void XMPStatSource::load(std::vector< std::shared_ptr<Stat> >& stats)
{
    umf_string pathToStatData;
    SXMPUtils::ComposeArrayItemPath(UMF_NS, UMF_STATISTICS, kXMP_ArrayLastItem, &pathToStatData);

    umf_string pathToStatArray;
    SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStatData.c_str(), UMF_NS, UMF_STAT, &pathToStatArray);

    SXMPIterator statsIter(*metadata, UMF_NS, pathToStatArray.c_str(), kXMP_IterJustChildren);
    umf_string pathToStat;
    while (statsIter.Next(nullptr, &pathToStat))
    {
        umf_string tmpPath, statName, tmpStr;
        const Stat::UpdateMode::Type updateMode = Stat::UpdateMode::Disabled;

        SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStat.c_str(), UMF_NS, UMF_STAT_NAME, &tmpPath);
        if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &statName, 0) )
            UMF_EXCEPTION(DataStorageException, "Broken stat name");

        std::vector< StatField > fields;

        umf_string pathToFieldArray;
        SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToStat.c_str(), UMF_NS, UMF_STAT_FIELD, &pathToFieldArray);

        SXMPIterator fieldsIter(*metadata, UMF_NS, pathToFieldArray.c_str(), kXMP_IterJustChildren);
        umf_string pathToField;
        while (fieldsIter.Next(nullptr, &pathToField))
        {
            umf_string fieldName, schemaName, metadataName, metadataFieldName, opName;

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_NAME, &tmpPath);
            if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &fieldName, 0) )
                UMF_EXCEPTION(DataStorageException, "Broken stat field name");

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_SCHEMA_NAME, &tmpPath);
            if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &schemaName, 0) )
                UMF_EXCEPTION(DataStorageException, "Broken stat field metadata schema name");

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_METADATA_NAME, &tmpPath);
            if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &metadataName, 0) )
                UMF_EXCEPTION(DataStorageException, "Broken stat field metadata name");

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_FIELD_NAME, &tmpPath);
            if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &metadataFieldName, 0) )
                UMF_EXCEPTION(DataStorageException, "Broken stat field metadata field name");

            SXMPUtils::ComposeStructFieldPath(UMF_NS, pathToField.c_str(), UMF_NS, UMF_STAT_FIELD_OP_NAME, &tmpPath);
            if(!metadata->GetProperty(UMF_NS, tmpPath.c_str(), &opName, 0) )
                UMF_EXCEPTION(DataStorageException, "Broken stat field operation name");

            fields.push_back(StatField(fieldName, schemaName, metadataName, metadataFieldName, opName));
        }

        stats.emplace_back(make_shared<Stat>(statName, fields, updateMode));
    }
}

void XMPStatSource::clear()
{
    metadata->DeleteProperty(UMF_NS, UMF_STATISTICS);
}

