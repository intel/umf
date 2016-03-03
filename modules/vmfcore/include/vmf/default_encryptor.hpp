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

#ifndef VMF_DEFAULT_ENCRYPTOR_HPP
#define VMF_DEFAULT_ENCRYPTOR_HPP

#include "encryptor.hpp"

namespace vmf
{

/*!
 * \file default_encryptor.hpp
 * \brief %DefaultEncryptor header files
 */

//TODO: rename it and impementent it

class VMF_EXPORT DefaultEncryptor : public Encryptor
{
public:
    DefaultEncryptor(const vmf_string& _passphrase) : passphrase(_passphrase)
    {
        //VMF_EXCEPTION(NotImplementedException, "Default encryption is to be implemented");
        //IncorrectParamException in case of incorrect encryption parameters
    }

    /*!
     * \brief Encrypt data
     * \param [in] input input text data
     * \param [out] output where to put binary encrypted data
     */
    virtual void encrypt(const vmf_string& input, vmf_rawbuffer& output)
    {
        //VMF_EXCEPTION(NotImplementedException, "Default encryption is to be implemented");
        //IncorrectParamException in case of incorrect encryption parameters
        output = vmf_rawbuffer(input.size());
        size_t plen = passphrase.size();
        for(size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] ^ passphrase[i%plen];
        }
    }

    /*!
     * \brief Decrypt data
     * \param [in] input binary encrypted input data
     * \param [out] output where to put decrypted text data
     */
    virtual void decrypt(const vmf_rawbuffer& input, vmf_string& output)
    {
        //VMF_EXCEPTION(NotImplementedException, "Default encryption is to be implemented");
        //IncorrectParamException in case of incorrect encryption parameters (like key, etc.)
        output = vmf_string(input.size(), '\0');
        size_t plen = passphrase.size();
        for(size_t i = 0; i < input.size(); i++)
        {
            output[i] = input[i] ^ passphrase[i%plen];
        }

    }

    /*!
     * \brief Gets the hint for the algorithm (for example, the name of the algorithm)
     */
    virtual vmf_string getHint()
    {
        return "out = in xor passphrase[i/passlength]";
       // VMF_EXCEPTION(NotImplementedException, "Default encryption is to be implemented");
    }

    /*!
     * \brief Default destructor
     */
    virtual ~DefaultEncryptor() { }

private:
    vmf_string passphrase;
};


}


#endif //VMF_DEFAULT_ENCRYPTOR_HPP
