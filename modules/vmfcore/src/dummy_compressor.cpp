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

#include "vmf/dummy_compressor.hpp"

namespace vmf {

void DummyCompressor::compress(const MetaString& input, vmf_rawbuffer& output)
{
    output = vmf_rawbuffer(input.c_str(), input.size());
}

void DummyCompressor::decompress(const vmf_rawbuffer& input, MetaString& output)
{
    output = MetaString(input.data.get(), input.size);
}

const MetaString& DummyCompressor::getId()
{
    return id;
}

class DummyCompressorRegistrator
{
public:
    DummyCompressorRegistrator()
    {
        std::shared_ptr<DummyCompressor> dc = std::make_shared<DummyCompressor>();
        registerCompressor(std::dynamic_pointer_cast<ICompressor>(dc));
    }
};

const MetaString DummyCompressor::id = "dummy";
static DummyCompressorRegistrator dcr;

} /* vmf */
