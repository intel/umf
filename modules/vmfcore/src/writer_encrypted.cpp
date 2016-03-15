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

#include "vmf/writer_encrypted.hpp"
#include "vmf/rwconst.hpp"

namespace vmf
{

std::string WriterEncrypted::store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    std::string text = writer->store(schemas);
    return encrypt(text);
}

std::string WriterEncrypted::store(const MetadataSet& set)
{
    std::string text = writer->store(set);
    return encrypt(text);
}

std::string WriterEncrypted::store(const IdType& nextId,
                                   const std::string& filepath,
                                   const std::string& checksum,
                                   const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                                   const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                   const MetadataSet& set,
                                   const std::string &hint)
{
    std::string text = writer->store(nextId, filepath, checksum, segments, schemas, set, hint);
    return encrypt(text);
}

std::string WriterEncrypted::store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    std::string text = writer->store(segments);
    return encrypt(text);
}


class MetadataAccessor: public Metadata
{
public:
    MetadataAccessor( const std::shared_ptr< MetadataDesc >& spDescription )
      : Metadata(spDescription) { }
    MetadataAccessor( const Metadata& oMetadata )
      : Metadata(oMetadata) { }
    using Metadata::setId;
    virtual ~MetadataAccessor() {}
};


std::string WriterEncrypted::encrypt(const std::string &input)
{
    if(encryptor)
    {
        vmf_rawbuffer encryptedBuf;
        encryptor->encrypt(input, encryptedBuf);
        //Encrypted binary data should be represented in base64
        //because of \0 symbols
        std::string encrypted = Variant::base64encode(encryptedBuf);

        //Store encrypted data in a format of current implementation
        std::shared_ptr<vmf::MetadataSchema> eSchema;
        eSchema = std::make_shared<vmf::MetadataSchema>(ENCRYPTED_DATA_SCHEMA_NAME);
        VMF_METADATA_BEGIN(ENCRYPTED_DATA_DESC_NAME);
            VMF_FIELD_STR(ENCRYPTION_HINT_PROP_NAME);
            VMF_FIELD_STR(ENCRYPTED_DATA_PROP_NAME);
        VMF_METADATA_END(eSchema);

        std::shared_ptr<vmf::Metadata> eMetadata;
        eMetadata = std::make_shared<vmf::Metadata>(eSchema->findMetadataDesc(ENCRYPTED_DATA_DESC_NAME));
        eMetadata->push_back(FieldValue(ENCRYPTION_HINT_PROP_NAME, encryptor->getHint()));
        eMetadata->push_back(FieldValue(ENCRYPTED_DATA_PROP_NAME, encrypted));

        MetadataAccessor metadataAccessor(*eMetadata);
        metadataAccessor.setId(0);
        eMetadata = std::make_shared<vmf::Metadata>(metadataAccessor);

        MetadataSet eSet;
        eSet.push_back(eMetadata);
        std::vector< std::shared_ptr<MetadataSchema> > eSchemas;
        eSchemas.push_back(eSchema);

        const IdType nextId = 1;
        const std::string filePath = "";
        const std::string checksum = "";
        const std::string hint = "";
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;

        std::string outputString;
        outputString = getBackendWriter()->store(nextId, filePath, checksum, segments, eSchemas, eSet, hint);

        return outputString;
    }
    else
    {
        return input;
    }
}

}
