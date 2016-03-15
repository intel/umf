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
* \file reader_compressed.hpp
* \brief Deserialization based on instance of IReader class
*/

#pragma once

#ifndef VMF_READER_COMPRESSED_HPP
#define VMF_READER_COMPRESSED_HPP

#include "ireader.hpp"

namespace vmf
{

/*!
 * \class ReaderCompressed
 * \brief ReaderCompressed class performs deserialization of compressed stream
 * based on provided instance of and IReader
 */
class VMF_EXPORT ReaderCompressed : public IReader
{
public:
    /*!
     * \brief Constructor of the class
     * \param _reader Shared pointer to instance of IReader class
     * \param _ignoreUnknownCompressor Flag specifying what to do with unknown compressor:
     * throw an exception (false) or pass compressed data as VMF metadata
     */
    ReaderCompressed(std::shared_ptr<IReader> _reader, bool _ignoreUnknownCompressor = false) :
    IReader(), reader(_reader), ignoreUnknownCompressor(_ignoreUnknownCompressor)
    { }
    
    virtual ~ReaderCompressed() { }

    // IReader implementation
    virtual bool parseAll(const std::string& text, IdType& nextId,
                          std::string& filepath, std::string& checksum, std::string& hint,
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

    virtual std::shared_ptr<IReader> getBackendReader()
    {
        return reader ? reader->getBackendReader() : nullptr;
    }

    /*!
     * \brief Performs decompression of previously compressed data
     * \param input Input string
     * \return Decompressed string
     */
    virtual std::string decompress(const std::string& input);

protected:
    std::shared_ptr<IReader> reader;
    bool ignoreUnknownCompressor;
};


}//vmf

#endif /* VMF_READER_COMPRESSED_HPP */
