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
#pragma once

#ifndef _FAKE_COMPRESSOR_HPP
#define _FAKE_COMPRESSOR_HPP

#include "vmf/compressor.hpp"

namespace vmf
{

//Compressor class to pass fake Id to registerNew() method
class FakeCompressor : public Compressor
{
public:
    virtual void compress(const vmf_string& input, vmf_rawbuffer& output)
    {
        //copies input to another buffer and writes result
        output = vmf_rawbuffer(input.c_str(), input.size());
    }

    virtual void decompress(const vmf_rawbuffer& input, vmf_string &output)
    {
        //copies code to another buffer and writes result
        output = vmf_string(input.data(), input.size());
    }

    std::shared_ptr<Compressor> createNewInstance() const
    {
        std::shared_ptr<Compressor> ptr(new FakeCompressor);
        std::dynamic_pointer_cast<FakeCompressor>(ptr)->setId(id);
        return ptr;
    }

    virtual vmf::vmf_string getId()
    {
        return id;
    }

    void setId(vmf_string s)
    {
        id = s;
    }

private:
    vmf_string id;
};

} //vmf

#endif //_FAKE_COMPRESSOR_HPP




