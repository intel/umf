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
 * \file encryptor_default.hpp
 * \brief %EncryptorDefault header files
 */

/*!
 * \class EncryptorDefault
 * \brief The class representing default algorithms of encryption and decryption
 * (currently this is TripleDES with HMAC/SHA-1)
 */

class VMF_EXPORT EncryptorDefault : public Encryptor
{
public:
    EncryptorDefault(const vmf_string& _passphrase) : passphrase(_passphrase)
    { }

    /*!
     * \brief Encrypt data
     * \param [in] input input text data
     * \param [out] output where to put binary encrypted data
     */
    virtual void encrypt(const vmf_string& input, vmf_rawbuffer& output);

    /*!
     * \brief Decrypt data
     * \param [in] input binary encrypted input data
     * \param [out] output where to put decrypted text data
     */
    virtual void decrypt(const vmf_rawbuffer& input, vmf_string& output);

    /*!
     * \brief Gets the hint for the algorithm (for example, the name of the algorithm)
     */
    virtual vmf_string getHint()
    {
        return "Password-Based decryptor using TripleDES and HMAC/SHA-1";
    }

    /*!
     * \brief Default destructor
     */
    virtual ~EncryptorDefault() { }

private:
    vmf_string passphrase;
};


}


#endif //VMF_DEFAULT_ENCRYPTOR_HPP
