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
ReaderEncrypted::ReaderEncrypted(std::shared_ptr<IReader> _reader,
                                 std::shared_ptr<Encryptor> _encryptor,
                                 bool _ignoreUnknownEncryptor)
{
    VMF_EXCEPTION(NotImplementedException, "ReaderEncrypted is not implemented yet!");
}

bool ReaderEncrypted::parseAll(const std::string& text, IdType& nextId,
                               std::string& filepath, std::string& checksum, std::string& hint,
                               std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                               std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    VMF_EXCEPTION(NotImplementedException, "ReaderEncrypted is not implemented yet!");
}


bool ReaderEncrypted::parseSchemas(const std::string& text,
                                   std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    VMF_EXCEPTION(NotImplementedException, "ReaderEncrypted is not implemented yet!");
}

bool ReaderEncrypted::parseMetadata(const std::string& text,
                                    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                    std::vector<std::shared_ptr<MetadataInternal>>& metadata )
{
    VMF_EXCEPTION(NotImplementedException, "ReaderEncrypted is not implemented yet!");
}

bool ReaderEncrypted::parseVideoSegments(const std::string& text,
                                         std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    VMF_EXCEPTION(NotImplementedException, "ReaderEncrypted is not implemented yet!");
}

}
