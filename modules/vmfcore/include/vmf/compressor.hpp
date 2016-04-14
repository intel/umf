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
#ifndef VMF_COMPRESSOR_HPP
#define VMF_COMPRESSOR_HPP

/*!
* \file compressor.hpp
* \brief %Compressor header file
*/

#include <map>
#include "vmf/metadataschema.hpp"

namespace vmf
{

/*! \class Compressor
 * \brief Interface for all compression algorithms
 */
class VMF_EXPORT Compressor
{
public:
    /*!
     * \brief Compresses data
     * \param [in] input input text data
     * \param [out] output where to put binary compressed data
     */
    virtual void compress(const vmf_string& input, vmf_rawbuffer& output) = 0;

    /*!
     * \brief Decompresses data
     * \param [in] input binary compressed input data
     * \param [out] output where to put decompressed text data
     */
    virtual void decompress(const vmf_rawbuffer& input, vmf_string& output) = 0;

    /*!
     * \brief Creates a new instance of the compressor
     * \return Smart pointer to Compressor instance
     */
    virtual std::shared_ptr<Compressor> createNewInstance() const = 0;

    /*!
     * \brief Gets the ID of current algorithm
     */
    virtual vmf_string getId() = 0;

    /*!
     * \brief Default destructor
     */
    virtual ~Compressor() { }

    /*!
     * \brief Registers new algorithm before its use in decompression.
     * \param compressor Pointer to the implementation of the algorithm
     * \throw IncorrectParamException is thrown for a compressor with already registered ID
     */
    static void registerNew(std::shared_ptr<Compressor> compressor);

    /*!
     * \brief Unregisters previously registered compression algorithm
     * \param id String ID of the compression algorithm
     * \throw IncorrectParamException is thrown for unknown or built-in IDs
     */
    static void unregister(const vmf_string& id);

    /*!
     * \brief Creates new instance of previously registered compressor identified by ID
     * \param id String identifying compression algorithm
     * \return Smart pointer to Compressor instance
     * \throw IncorrectParamException is thrown for unknown (not built-in and not-registered) IDs
     */
    static std::shared_ptr<Compressor> create(const vmf_string& id);

    /*!
     * \brief Returns a list of currently registered compressors,
     * both built-in ones or user-defined
     * \return A list of registered compressors IDs
     */
    static std::vector<vmf_string> getRegisteredIds();
};

} /* vmf */

#endif /* VMF_COMPRESSOR_HPP */
