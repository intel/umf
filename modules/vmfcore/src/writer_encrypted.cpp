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

WriterEncrypted::WriterEncrypted(std::shared_ptr<IWriter> _writer,
                                 std::shared_ptr<Encryptor> _encryptor, bool encryptAll) :
                                 writer(_writer),
                                 encryptor(_encryptor),
                                 useEncryption(encryptAll)
{  }

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
                                   const MetadataSet& set, bool useEncryption, const std::string &hint)
{
    std::string text = writer->store(nextId, filepath, checksum, segments, schemas, set, false, hint);
    if(useEncryption)
        return encrypt(text);
    else
        return text;
}

std::string WriterEncrypted::store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    std::string text = writer->store(spSegment);
    return encrypt(text);
}

std::string WriterEncrypted::store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    std::string text = writer->store(segments);
    return encrypt(text);
}

std::string WriterEncrypted::encrypt(const std::string &input)
{
    VMF_EXCEPTION(NotImplementedException, "WriterEncrypted is not implemented yet!");
}
  
}
