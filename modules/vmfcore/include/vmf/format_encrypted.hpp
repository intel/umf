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
 * \file format_encrypted.hpp
 * \brief %FormatEncrypted class header file
 */

#ifndef VMF_FORMAT_ENCRYPTED_HPP
#define VMF_FORMAT_ENCRYPTED_HPP

#include "vmf/format.hpp"

namespace vmf
{

/*!
 * \class FormatEncrypted
 * \brief FormatEncrypted class is a %Format interface extension for %Encryptor use.
 * The class performs (de)serialization of the stream with encryption
 * based on provided instance of %Format and %Encryptor interfaces.
 */
class VMF_EXPORT FormatEncrypted : public Format
{
public:

    /*!
     * \brief Constructor taking encryptor and format to be used at data reading and writing
     * \note To use both compression and encryption pass the FormatCompressed as format
     * \param format Shared pointer to instance of Format
     * \param _encryptor Shared pointer to instance of Encryptor class
     * \param _ignoreUnknownEncryptor Flag specifying what to do with unknown encryptor:
     * throw an exception (false) or pass encrypted data as VMF metadata
     */
    FormatEncrypted(std::shared_ptr<Format> format,
                    std::shared_ptr<Encryptor> _encryptor,
                    bool _ignoreUnknownEncryptor = false);

    /*!
     * \brief Default destructor
     */
    virtual ~FormatEncrypted();

    /*!
     * \brief Serialize input metadata and related stuff to string
     */
    virtual std::string store(
            const MetadataSet &set,
            const std::vector<std::shared_ptr<MetadataSchema> > &schemas = {},
            const std::vector<std::shared_ptr<MetadataStream::VideoSegment> > &segments = {},
            const std::vector<std::shared_ptr<Stat>>& stats = {},
            const AttribMap &attribs = AttribMap() // nextId, checksum, etc.
            );

    /*!
     * \brief Deserialize input string to metadata and related stuff
     * \param text input string
     * \param metadata Metadata records
     * \param schemas Schemas of the metadata
     * \param segments Video segments
     * \param stats Statistical object
     * \param attribs Attributes like nextId, checksum, etc.
     * \return Numbers of items read by categories
     */
    virtual ParseCounters parse(
            const std::string &text,
            std::vector<MetadataInternal> &metadata,
            std::vector<std::shared_ptr<MetadataSchema> > &schemas,
            std::vector<std::shared_ptr<MetadataStream::VideoSegment> > &segments,
            std::vector<std::shared_ptr<Stat> >& stats,
            AttribMap &attribs
            );

    virtual std::shared_ptr<Format> getBackendFormat()
    {
        return format ? format->getBackendFormat() : nullptr;
    }

protected:
    /*!
     * \brief Performs encryption of text data
     * \param input Input string
     * \return Encrypted string
     */
    virtual std::string encrypt(const std::string& input);

    /*!
     * \brief Performs decryption of previously encrypted data
     * \param input Input string
     * \return Decrypted string
     */
    virtual std::string decrypt(const std::string& input);

    std::shared_ptr<Format> format;
    std::shared_ptr<Encryptor> encryptor;
    std::shared_ptr<vmf::MetadataSchema> eSchema;
    bool ignoreUnknownEncryptor;
};

}


#endif // VMF_FORMAT_ENCRYPTED_HPP
