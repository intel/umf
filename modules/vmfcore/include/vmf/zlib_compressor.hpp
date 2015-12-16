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
#ifndef ZLIB_COMPRESSOR_HPP
#define ZLIB_COMPRESSOR_HPP

#include "icompressor.hpp"

namespace vmf {

/*!
 * \class ZLibCompressor
 * \brief Compression algorithm that uses ZLib library.
 * Currently runs Deflate algorithm with default settings.
 */
class VMF_EXPORT ZLibCompressor : public Compressor
{
public:
    /*!
     * \brief Compress data
     * \param [in]  input  input text data
     * \param [out] output binary buffer where to put compressed data
     */
    virtual void compress(const vmf_string& input, vmf_rawbuffer& output);

    /*!
     * \brief Decompress data
     * \param [in]  input  binary buffer with compressed input data
     * \param [out] output string where to put decompressed text data
     */
    virtual void decompress(const vmf_rawbuffer& input, vmf_string &output);

    /*!
     * \brief Get the ID of current algorithm
     */
    virtual const vmf_string& getId();

private:
    static const vmf_string id;
};

} /* vmf */

#endif /* ZLIB_COMPRESSOR_HPP */

