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

#include "my_encryptor.hpp"
#include <random>

using namespace std;
using namespace vmf;

const size_t nSize = sizeof(vmf_integer);

void MyEncryptor::encrypt(const vmf_string &input, vmf_rawbuffer &output)
{
    std::mt19937_64 gen(getTimestamp());
    size_t plen = passphrase.length();
    std::string tempKey(plen, '\0');
    for(char& c : tempKey)
    {
        c = (char)gen();
    }

    vmf_integer a = gen(), b = gen();
    vector<vmf_integer> checkData(3);
    checkData[0] = a;
    checkData[1] = b;
    checkData[2] = a*b;
    std::string checkStr((char*)checkData.data(), 3*nSize);

    output = vmf_rawbuffer(plen + nSize*3 + input.size());
    size_t offset = 0;
    //save tempKey
    for(size_t i = offset; i < plen; i++)
    {
        output[i] = tempKey[i] ^ passphrase[i];
    }
    offset += plen;
    //save info for checks
    for(size_t i = offset; i < offset + checkStr.length(); i++)
    {
        output[i] = checkStr[i-offset] ^ tempKey[i % plen];
    }
    offset += checkStr.length();
    //save the data
    for(size_t i = offset; i < offset + input.size(); i++)
    {
        output[i] = input[i-offset] ^ tempKey[i % plen];
    }
}


void MyEncryptor::decrypt(const vmf_rawbuffer &input, vmf_string &output)
{
    output.clear();
    if(!input.empty())
    {
        size_t plen = passphrase.length();
        if(input.size() <= plen + nSize*3)
        {
            VMF_EXCEPTION(IncorrectParamException, "Encrypted message is too short");
        }

        std::string tempKey(plen, '\0');
        //decrypt tempKey
        size_t offset = 0;
        for(size_t i = offset; i < plen; i++)
        {
            tempKey[i] = input[i] ^ passphrase[i];
        }
        offset += plen;
        //decrypt info for checks
        std::string checkStr(nSize*3, '\0');
        for(size_t i = offset; i < offset + nSize*3; i++)
        {
            checkStr[i-offset] = input[i] ^ tempKey[i % plen];
        }
        offset += nSize*3;

        vmf_integer* checkData = (vmf_integer*)checkStr.data();
        vmf_integer a = checkData[0], b = checkData[1], c = checkData[2];
        if(a * b != c)
        {
            VMF_EXCEPTION(IncorrectParamException, "Failed to check the data!");
        }

        //decrypt the data
        output = vmf_string(input.size() - offset, '\0');
        for(size_t i = offset; i < input.size(); i++)
        {
            output[i-offset] = input[i] ^ tempKey[i % plen];
        }
    }
}
