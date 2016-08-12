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

namespace umf
{

//Compressor class to pass fake Id to registerNew() method
class FakeCompressor : public Compressor
{
public:
    virtual void compress(const umf_string& input, umf_rawbuffer& output)
    {
        //copies input to another buffer and writes result
        output = umf_rawbuffer(input.c_str(), input.size());
    }

    virtual void decompress(const umf_rawbuffer& input, umf_string &output)
    {
        //copies code to another buffer and writes result
        output = umf_string(input.data(), input.size());
    }

    std::shared_ptr<Compressor> createNewInstance() const
    {
        std::shared_ptr<Compressor> ptr(new FakeCompressor);
        std::dynamic_pointer_cast<FakeCompressor>(ptr)->setId(id);
        return ptr;
    }

    virtual umf::umf_string getId()
    {
        return id;
    }

    void setId(umf_string s)
    {
        id = s;
    }

private:
    umf_string id;
};

} //vmf

#endif //_FAKE_COMPRESSOR_HPP




