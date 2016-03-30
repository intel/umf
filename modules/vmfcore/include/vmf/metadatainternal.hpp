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
* class MetadataInternal
* \brief %MetadataInternal is a %Metadata sub-class intended for incremental Metadata items deserialization (streaming case).
* Unlike the %Metadata parent it allows setting internal item ID and keeps unresolved yet references (when the object isn't in a stream).
* Please don't use %MetadataInternal objects unless you develop custom %IReader implementation, use %Metadata instances.
*/
class VMF_EXPORT MetadataInternal : public Metadata
{
public:
    /*!
    * \brief Class constructor
    * \param spDescription [in] metadata item description
    * \throw NullPointerException if metadata description pointer is null
    */
    MetadataInternal( const std::shared_ptr< MetadataDesc >& spDescription );

    /*!
    * \brief Class copy constructor
    * \param that [in] MetadataInternal object
    */
    MetadataInternal( const MetadataInternal& that );

    /*!
    * \brief Class constructor
    * \param base [in] Metadata object
    */
    MetadataInternal( const Metadata& base );

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataInternal();

    /*!
    * \brief Making internal ID setter public
    */
    using Metadata::setId;

    /*!
    * \brief vector of references of the object
    */
    std::vector<std::pair<IdType, std::string>> vRefs;
};

struct VMF_EXPORT MetadataInternal2
{
    MetadataInternal2(const std::string& descName, const std::string& schemaName) :
        id(-1), descName(descName), schemaName(schemaName),
        frameIndex(Metadata::UNDEFINED_FRAME_INDEX), frameNum(Metadata::UNDEFINED_FRAMES_NUMBER),
        timestamp(Metadata::UNDEFINED_TIMESTAMP), duration(Metadata::UNDEFINED_DURATION)
    {}

    MetadataInternal2(MetadataInternal2&& mdi) :
        id(std::move(mdi.id)), descName(std::move(mdi.descName)), schemaName(std::move(mdi.schemaName)),
        frameIndex(mdi.frameIndex), frameNum(mdi.frameNum),
        timestamp(mdi.timestamp), duration(mdi.duration),
        fields(std::move(mdi.fields)), refs(std::move(mdi.refs))
    {}

    IdType      id;
    std::string descName;
    std::string schemaName;
    long long   frameIndex;
    long long   frameNum;
    long long   timestamp;
    long long   duration;
    std::map<std::string, std::string> fields;
    std::vector<std::pair<IdType, std::string>> refs;
};

}//vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //__VMF_METADATAINTERNAL_H__
