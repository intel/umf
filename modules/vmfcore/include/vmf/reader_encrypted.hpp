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
 * \file reader_encrypted.hpp
 * \brief Deserialization based on instances of Encryptor and IReader classes
 */

#pragma once

#ifndef VMF_READER_ENCRYPTED_HPP
#define VMF_READER_ENCRYPTED_HPP

#include "ireader.hpp"

namespace vmf
{

/*!
 * \class ReaderEncrypted
 * \brief The class performs deserialization of encrypted stream
 * based on provided instance of IReader and
 */
class VMF_EXPORT ReaderEncrypted : public IReader
{
public:
    /*!
     * \brief Constructor of the class
     * \param _reader Shared pointer to instance of IReader class
     * \param _encryptor Shared pointer to instance of Encryptor class
     * \param _ignoreUnknownEncryptor Flag specifying what to do with unknown encryptor:
     * throw an exception (false) or pass encrypted data as VMF metadata
     */
    ReaderEncrypted(std::shared_ptr<IReader> _reader, std::shared_ptr<Encryptor> _encryptor,
                    bool _ignoreUnknownEncryptor = false);

    virtual ~ReaderEncrypted() { }

    // IReader implementation
    virtual bool parseAll(const std::string& text, IdType& nextId,
                          std::string& filepath, std::string& checksum,
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

protected:

};

} //vmf

#endif //VMF_READER_ENCRYPTED_HPP

