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
#include "vmf/format_compressed.hpp"

namespace vmf
{

FormatCompressed::FormatCompressed(std::shared_ptr<Format> format, const std::string& compressorId)
    : format(format), compressorId(compressorId)
{
}

FormatCompressed::~FormatCompressed()
{}

std::string FormatCompressed::store(
    const MetadataSet& set,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    //const std::vector<Stat>& stats,
    const AttribMap& attribs
)
{
    return format->store(set, schemas, segments, /*stats,*/ attribs);
}

std::array<int, 5> FormatCompressed::parse(
    const std::string& text,
    std::vector<std::shared_ptr<MetadataInternal>>& metadata,
    std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    //std::vector<Stat>& stats,
    AttribMap& attribs // nextId, checksum, etc
    )
{
    return format->parse(text, metadata, schemas, segments, /*stats,*/ attribs);
}

std::string FormatCompressed::compress(const std::string& input)
{
    VMF_EXCEPTION(NotImplementedException, "NYI");
}

std::string FormatCompressed::decompress(const std::string& input)
{
    VMF_EXCEPTION(NotImplementedException, "NYI");
}

}//vmf
