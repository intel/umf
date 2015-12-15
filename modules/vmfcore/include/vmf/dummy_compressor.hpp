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
#ifndef DUMMY_COMPRESSOR_HPP
#define DUMMY_COMPRESSOR_HPP

#include "icompressor.hpp"

namespace vmf {

/*!
 * \class DummyCompressor
 * \brief Example of a compression algorithm. Actually doesn't change the input/output data.
 */
class VMF_EXPORT DummyCompressor : public ICompressor
{
public:
    /*!
     * \brief Compress data
     * \param [in] input input text data
     * \param [out] output where to put binary compressed data
     */
    virtual void compress(const vmf_string& input, vmf_rawbuffer& output);

    /*!
     * \brief Decompress data
     * \param [in] input binary compressed input data
     * \param [out] output where to put decompressed text data
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

#endif /* DUMMY_COMPRESSOR_HPP */

