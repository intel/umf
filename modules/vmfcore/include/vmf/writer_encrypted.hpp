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
 * \file writer_encrypted.hpp
 * \brief Serialization based on instances of Encryptor and IWriter classes
 */

#pragma once

#ifndef VMF_WRITER_ENCRYPTED_HPP
#define VMF_WRITER_ENCRYPTED_HPP

#include "iwriter.hpp"

namespace vmf
{

class VMF_EXPORT WriterEncrypted : public IWriter
{
public:
    /*!
     * \brief Constructor taking encryptor and writer to be used at data writing
     * \param _writer Shared pointer to instance of IWriter
     * \param _encryptor Shared pointer to istance of Encryptor
     * \param encryptAll Specifies whether all data should be encrypted
     * or just selected items
     * should be encrypted at file saving or serialization
     */
    WriterEncrypted(std::shared_ptr<IWriter> _writer, std::shared_ptr<Encryptor> _encryptor,
                    bool encryptAll = false) :
                    writer(_writer),
                    encryptor(_encryptor),
                    useEncryption(encryptAll)
    { }

    /*!
     * \brief Class destructor
     */
    virtual ~WriterEncrypted()
    { }

    // IWriter implementation
    virtual std::string store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas);
    virtual std::string store(const MetadataSet& set);
    virtual std::string store(const IdType& nextId,
                              const std::string& filepath,
                              const std::string& checksum,
                              const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                              const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                              const MetadataSet& set,
                              const std::string& hint);
    virtual std::string store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

protected:
    /*!
     * \brief Performs encryption of text data
     * \param input Input string
     * \return Encrypted string
     */
    virtual std::string encrypt(const std::string& input);

    std::shared_ptr<IWriter> writer;
    std::shared_ptr<Encryptor> encryptor;
    bool useEncryption;
};

}


#endif //VMF_WRITER_ENCRYPTED_HPP
