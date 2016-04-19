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

/*!
* \file metadatainternal.hpp
* \brief %MetadataInternal class header file
*/

#ifndef __VMF_METADATAINTERNAL_H__
#define __VMF_METADATAINTERNAL_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "metadata.hpp"

namespace vmf
{
/*!
* struct MetadataInternal
* \brief %MetadataInternal is an intermediate stage of %Metadata used for incremental Metadata items deserialization (streaming case).
* Unlike the %Metadata it keeps justparsed non-validated data and is transformed to %Metadata at adding to MetadataStream time.
* Please don't use %MetadataInternal unless you develop custom %Format implementation.
*/
struct VMF_EXPORT MetadataInternal
{
    struct VMF_EXPORT FieldInternal
    {
        FieldInternal() :
            value(), useEncryption(false), encryptedData()
        { }

        std::string value;
        bool        useEncryption;
        std::string encryptedData;
    };

    MetadataInternal(const std::string& descName, const std::string& schemaName) :
        id(-1), descName(descName), schemaName(schemaName),
        frameIndex(Metadata::UNDEFINED_FRAME_INDEX), frameNum(Metadata::UNDEFINED_FRAMES_NUMBER),
        timestamp(Metadata::UNDEFINED_TIMESTAMP), duration(Metadata::UNDEFINED_DURATION),
        fields(), refs(),
        useEncryption(false), encryptedData()
    {}

    MetadataInternal(const MetadataInternal& mdi) :
        id(mdi.id), descName(mdi.descName), schemaName(mdi.schemaName),
        frameIndex(mdi.frameIndex), frameNum(mdi.frameNum),
        timestamp(mdi.timestamp), duration(mdi.duration),
        fields(mdi.fields), refs(mdi.refs),
        useEncryption(mdi.useEncryption), encryptedData(mdi.encryptedData)
    {}

    MetadataInternal(MetadataInternal&& mdi) :
        id(std::move(mdi.id)), descName(std::move(mdi.descName)), schemaName(std::move(mdi.schemaName)),
        frameIndex(mdi.frameIndex), frameNum(mdi.frameNum),
        timestamp(mdi.timestamp), duration(mdi.duration),
        fields(std::move(mdi.fields)), refs(std::move(mdi.refs)),
        useEncryption(mdi.useEncryption), encryptedData(std::move(mdi.encryptedData))
    {}

    IdType      id;
    std::string descName;
    std::string schemaName;
    long long   frameIndex;
    long long   frameNum;
    long long   timestamp;
    long long   duration;
    std::map<std::string, FieldInternal> fields;
    std::vector<std::pair<IdType, std::string>> refs;
    bool        useEncryption;
    std::string encryptedData;
};

}//vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //__VMF_METADATAINTERNAL_H__
