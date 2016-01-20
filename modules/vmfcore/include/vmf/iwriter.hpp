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
* \file iwriter.hpp
* \brief %IWriter interface declaration
* \details The file contains declaration of %IWriter interface.
*/

#pragma once

#ifndef __VMF_IWRITER_H__
#define __VMF_IWRITER_H__

#include "metadatastream.hpp"

namespace vmf
{
/*!
* \class IWriter
* \brief %IWriter is an interface for metadata and schemas serialization to various formats.
*/
class VMF_EXPORT IWriter
{
public:
    IWriter()
    { }

    virtual ~IWriter()
    { }

    /*!
    * \brief Export a schemas vector to a string representation
    */
    virtual std::string store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas) = 0;

    /*!
    * \brief Export a MetadataSet to a string representation
    */
    virtual std::string store(const MetadataSet& set) = 0;

    /*!
    * \brief Export all the stream metadata including schemas to a string representation. Used for "stream.serialize()" implementation.
    */
    virtual std::string store( const IdType& nextId,
                               const std::string& filepath,
                               const std::string& checksum,
			       const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
			       const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               const MetadataSet& set ) = 0;

    /*!
    * \brief Export a video segment to a string representation
    */
    virtual std::string store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment) = 0;

    /*!
    * \brief Export a video segments vector to a string representation.
    */
    virtual std::string store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments) = 0;

    /*!
     * \brief Performs compression of text data
     * \param input Input string
     * \return Compressed string
     */
    virtual std::string compress(const std::string& input) = 0;
};

}//vmf

#endif /* __VMF_IWRITER_H__ */
