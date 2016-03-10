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
* \file ireader.hpp
* \brief %IReader interface declaration
* \details The file contains declaration of %IReader interface.
*/

#pragma once

#ifndef __VMF_IREADER_H__
#define __VMF_IREADER_H__

#include "metadatainternal.hpp"
#include "metadatastream.hpp"

namespace vmf
{
/*!
* \class IReader
* \brief %IReader is an interface for metadata and schemas deserialization from various formats.
*/
class VMF_EXPORT IReader
{
public:
    IReader()
    { }

    virtual ~IReader()
    { }

    /*!
    * \brief Deserialize input string to stream metadata and related stuff. Used for "stream.deserialize(text)" implementation.
    * String may be comressed, in that case use ICompressor field to decompress it.
    */
    virtual bool parseAll(const std::string& text, IdType& nextId, std::string& filepath,
                          std::string& checksum, std::string& hint,
                          std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                          std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                          std::vector<std::shared_ptr<MetadataInternal>>& metadata) = 0;

    /*!
    * \brief Deserialize input string to a schemas vector
    */
    virtual bool parseSchemas(const std::string& text,
                              std::vector<std::shared_ptr<MetadataSchema>>& schemas) = 0;

    /*!
    * \brief Deserialize input string to a metadata vector
    */
    virtual bool parseMetadata(const std::string& text,
                               const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               std::vector<std::shared_ptr<MetadataInternal>>& metadata) = 0;

    /*!
    * \brief Deserialize input string to a segment vector
    */
    virtual bool parseVideoSegments(const std::string& text,
                                    std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments) = 0;
};

}//vmf

#endif /* __VMF_IREADER_H__ */
