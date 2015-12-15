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
#ifndef ICOMPRESSOR_HPP
#define ICOMPRESSOR_HPP

/*!
* \file icompressor.hpp
* \brief %ICompressor header file
*/

#include "vmf/metadataschema.hpp"

namespace vmf
{

/*! \class ICompressor
 * \brief Interface for all compression algorithms
 */
class VMF_EXPORT ICompressor
{
public:
    /*!
     * \brief Compress data
     * \param [in] input input text data
     * \param [out] output where to put binary compressed data
     */
    virtual void compress(const vmf_string& input, vmf_rawbuffer& output) = 0;

    /*!
     * \brief Decompress data
     * \param [in] input binary compressed input data
     * \param [out] output where to put decompressed text data
     */
    virtual void decompress(const vmf_rawbuffer& input, vmf_string& output) = 0;

    /*!
     * \brief Get the ID of current algorithm
     */
    virtual const vmf_string& getId() = 0;

    /*!
     * \brief Default destructor
     */
    virtual ~ICompressor()
    {

    }
};

/*!
 * \brief Registers new algorithm before its use in decompression.
 * \param impl Pointer to the implementation of the algorithm
 */
void registerCompressor(std::shared_ptr<ICompressor> impl);

/*!
 * \brief Unregisters previously registered compression algorithm
 * \param impl
 */
void unregisterCompressor(std::shared_ptr<ICompressor> impl);

/*!
 * \brief Unregisters previously registered compression algorithm
 * \param id String id of the compression algorithm
 */
void unregisterCompressor(const vmf_string& id);

/*!
 * \brief Gets instance of compression identified by id registered before
 * \param id String identifying compression algorithm
 * \return Smart pointer to ICompressor instance
 */
std::shared_ptr<ICompressor> getCompressorById(const vmf_string& id);

} /* vmf */

#endif /* ICOMPRESSOR_HPP */
