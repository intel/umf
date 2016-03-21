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

#ifndef _WEAK_ENCRYPTOR_HPP
#define _WEAK_ENCRYPTOR_HPP

#include "vmf/encryptor.hpp"

namespace vmf
{

enum CryptAlgo
{
    DEFAULT, WEAK, NONE
};

//Some testing class for encryption
class WeakEncryptor : public vmf::Encryptor
{
public:
    WeakEncryptor(char _key) : key(_key) { }

    virtual void encrypt(const vmf_string &input, vmf_rawbuffer &output)
    {
        output.clear();
        output.reserve(input.length()+1);
        output.push_back(magic ^ key);
        for(char c : input)
        {
            output.push_back(c ^ key);
        }
    }

    virtual void decrypt(const vmf_rawbuffer &input, vmf_string &output)
    {
        output.clear();
        if(!input.empty())
        {
            output.reserve(input.size()-1);
            if((input[0] ^ key) != magic)
            {
                throw IncorrectParamException("Incorrect decryption key!");
            }
            for(size_t i = 1; i < input.size(); i++)
            {
                output.push_back(input[i] ^ key);
            }
        }
    }

    virtual vmf_string getHint()
    {
        return vmf_string("bad encryptor for tests");
    }

    virtual ~WeakEncryptor() { }

private:
    char key;
    const char magic = 169;
};

}

#endif //_WEAK_ENCRYPTOR_HPP
