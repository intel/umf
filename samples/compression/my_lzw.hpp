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
#ifndef VMF_MY_LZW_HPP
#define VMF_MY_LZW_HPP

#include "vmf/vmf.hpp"

// User-created compression algorithm should implement Compressor interface
// Straightforward implementation of LZW algorithm
class MyLZWCompressor : public vmf::Compressor
{
public:
    virtual void compress(const vmf::vmf_string& input, vmf::vmf_rawbuffer& output);

    virtual void decompress(const vmf::vmf_rawbuffer& input, vmf::vmf_string &output);

    std::shared_ptr<Compressor> createNewInstance() const
    {
        return std::shared_ptr<Compressor>(new MyLZWCompressor);
    }

    virtual vmf::vmf_string getId()
    {
        return "com.intel.vmf.compressor.example.my_lzw";
    }
};


#endif /*VMF_MY_LZW_HPP */

