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
* \file reader_base.hpp
* \brief Implementation of %IReader interface common methods
*/

#pragma once

#ifndef VMF_READER_BASE_HPP
#define VMF_READER_BASE_HPP

#include "ireader.hpp"

namespace vmf
{

/*!
 * \class ReaderBase
 * \brief Class implementing common methods for all IReader implementations
 */
class VMF_EXPORT ReaderBase : public IReader
{
public:
    /*!
     * \brief Constructor of base class
     * \param _ignoreUnknownCompressor Flag specifying what to do with unknown compressor:
     * throw an exception (false) or pass compressed data as VMF metadata
     */
    ReaderBase(bool _ignoreUnknownCompressor = false) : IReader(),
    ignoreUnknownCompressor(_ignoreUnknownCompressor)
    { }

    // IReader implementation
    virtual bool parseAll(const std::string& text, IdType& nextId, std::string& filepath, std::string& checksum,
                          std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                          std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                          std::vector<std::shared_ptr<MetadataInternal>>& metadata);

    virtual bool parseSchemas(const std::string& text,
                              std::vector<std::shared_ptr<MetadataSchema>>& schemas);

    virtual bool parseMetadata(const std::string& text,
                               const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               std::vector<std::shared_ptr<MetadataInternal>>& metadata );

    virtual bool parseVideoSegments(const std::string& text,
                                    std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments);

    virtual std::string decompress(const std::string& input);

protected:
    virtual bool internalParseAll(const std::string& text, IdType& nextId, std::string& filepath, std::string& checksum,
                                  std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                                  std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                  std::vector<std::shared_ptr<MetadataInternal>>& metadata) = 0;

    virtual bool internalParseSchemas(const std::string& text,
                                      std::vector<std::shared_ptr<MetadataSchema>>& schemas) = 0;

    virtual bool internalParseMetadata(const std::string& text,
                                       const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                       std::vector<std::shared_ptr<MetadataInternal>>& metadata ) = 0;

    virtual bool internalParseVideoSegments(const std::string& text,
                                            std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments) = 0;

    bool ignoreUnknownCompressor;
};


}//vmf

#endif /* VMF_READER_BASE_HPP */
