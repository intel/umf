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

#ifndef VMF_ENCRYPTOR_HPP
#define VMF_ENCRYPTOR_HPP

#include "vmf/metadataschema.hpp"

namespace vmf
{

/*!
 * \file encryptor.hpp
 * \brief %Encryptor header files
 */

/*!
 * \class Encryptor
 * \brief Interface for all encryption algorithms
 */
class VMF_EXPORT Encryptor
{
public:
    /*!
     * \brief Encrypt data
     * \param [in] input input text data
     * \param [out] output where to put binary encrypted data
     */
    virtual void encrypt(const vmf_string& input, vmf_rawbuffer& output) = 0;

    /*!
     * \brief Decrypt data
     * \param [in] input binary encrypted input data
     * \param [out] output where to put decrypted text data
     */
    virtual void decrypt(const vmf_rawbuffer& input, vmf_string& output) = 0;

    /*!
     * \brief Gets the hint for the algorithm (for example, the name of the algorithm)
     */
    virtual vmf_string getHint() = 0;

    /*!
     * \brief Default destructor
     */
    virtual ~Encryptor() { }
};

}

#endif //VMF_ENCRYPTOR_HPP
