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

#include "vmf/reader_encrypted.hpp"
#include "vmf/rwconst.hpp"

namespace vmf
{

bool ReaderEncrypted::parseAll(const std::string& text, IdType& nextId,
                               std::string& filepath, std::string& checksum, std::string& hint,
                               std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                               std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    std::string decrypted = decrypt(text);
    return reader->parseAll(decrypted, nextId, filepath, checksum, hint, segments, schemas, metadata);
}


bool ReaderEncrypted::parseSchemas(const std::string& text,
                                   std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    std::string decrypted = decrypt(text);
    return reader->parseSchemas(decrypted, schemas);
}

bool ReaderEncrypted::parseMetadata(const std::string& text,
                                    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                    std::vector<std::shared_ptr<MetadataInternal>>& metadata )
{
    std::string decrypted = decrypt(text);
    return reader->parseMetadata(decrypted, schemas, metadata);
}

bool ReaderEncrypted::parseVideoSegments(const std::string& text,
                                         std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    std::string decrypted = decrypt(text);
    return reader->parseVideoSegments(decrypted, segments);
}

std::string ReaderEncrypted::decrypt(const std::string& input)
{
    //parse it as usual serialized VMF XML, search for specific schemas
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    if(!getBackendReader()->parseSchemas(input, schemas))
    {
        VMF_EXCEPTION(vmf::InternalErrorException, "Failed to parse schemas in input data");
    }

    if(schemas.size() == 1 && schemas[0]->getName() == ENCRYPTED_DATA_SCHEMA_NAME)
    {
        std::vector<std::shared_ptr<MetadataInternal>> metadata;
        if(!getBackendReader()->parseMetadata(input, schemas, metadata))
        {
            VMF_EXCEPTION(vmf::InternalErrorException, "Failed to parse schemas in input data");
        }

        std::shared_ptr<Metadata> eMetadata = metadata[0];
        vmf_string hint = eMetadata->getFieldValue(ENCRYPTION_HINT_PROP_NAME);
        vmf_string encoded = eMetadata->getFieldValue(ENCRYPTED_DATA_PROP_NAME);
        if(!encryptor)
        {
            if(!ignoreUnknownEncryptor)
            {
                VMF_EXCEPTION(IncorrectParamException, "No decryptor provided for encrypted data");
            }
            else
            {
                return input;
            }
        }
        else
        {
            try
            {
                std::string decrypted;
                // Encrypted binary data should be represented in base64
                // because of '\0' symbols
                vmf_rawbuffer encrypted = Variant::base64decode(encoded);
                encryptor->decrypt(encrypted, decrypted);
                return decrypted;
            }
            catch(IncorrectParamException& ee)
            {
                if(ignoreUnknownEncryptor)
                {
                    return input;
                }
                else
                {
                    VMF_EXCEPTION(IncorrectParamException, "Failed to decrypt data, the hint is \"" +
                                  hint + "\": " + ee.what());
                }
            }
        }
    }
    else
    {
        return input;
    }
}

}
