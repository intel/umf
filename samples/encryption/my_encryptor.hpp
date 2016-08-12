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

#ifndef VMF_MY_ENCRYPTOR_HPP
#define VMF_MY_ENCRYPTOR_HPP

#include "umf/vmf.hpp"

//User-created encryption algorithm should implement Encryptor interface
class MyEncryptor : public umf::Encryptor
{
public:

    MyEncryptor(std::string _passphrase) : passphrase(_passphrase){ }

    virtual ~MyEncryptor() { }

    virtual umf::umf_string getHint()
    {
        return "Simple PRNG-based XOR encryption";
    }

    virtual void encrypt(const umf::umf_string& input, umf::umf_rawbuffer& output);

    virtual void decrypt(const umf::umf_rawbuffer& input, umf::umf_string& output);

private:
    std::string passphrase;
};


#endif //VMF_MY_ENCRYPTOR_HPP
