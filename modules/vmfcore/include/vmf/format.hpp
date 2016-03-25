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

/*!
* \file format.hpp
* \brief %Format interface declaration
* \details The file contains declaration of %Format interface (for serialization/deserialization to string).
*/

#pragma once

#ifndef VMF_FORMAT_H
#define VMF_FORMAT_H

#include "vmf/metadatastream.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <array>

namespace vmf
{
/*!
* \class Format
* \brief %Format is an interface for metadata and schemas serialization/deserialization to various string formats.
*/
class VMF_EXPORT Format
{
public:
    Format()
    { }

    virtual ~Format()
    { }

    typedef std::map<std::string, std::string> AttribMap;
    /*!
    * \brief Export various metadata stream items to a string representation. Used in particular for "stream.serialize()" implementation.
    */
    virtual std::string store(
        const MetadataSet& set,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas = {},
        const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments = {},
        //const std::vector<Stat>& stats = {},
        const AttribMap& attribs = AttribMap() // nextId, checksum, etc
        ) = 0;

    typedef union {
            struct { int metadata, schemas, segments, stats, attribs; };
            int cnt[5];
        } ParseCounters;
    /*!
    * \brief Deserialize input string to stream metadata and related stuff. Used in particular for "stream.deserialize(text)" implementation.
    */
    virtual ParseCounters parse(
        const std::string& text,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        //std::vector<Stat>& stats,
        AttribMap& attribs // nextId, checksum, etc
        ) = 0;
};

}//vmf

#endif /* VMF_FORMAT_H */
