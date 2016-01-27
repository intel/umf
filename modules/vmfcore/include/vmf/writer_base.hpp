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

/*!
* \file writer_base.hpp
* \brief Implementation of %IWriter interface common methods
*/

#pragma once

#ifndef VMF_WRITER_BASE_HPP
#define VMF_WRITER_BASE_HPP

#include "iwriter.hpp"

namespace vmf
{

/*!
 * \class WriterBase
 * \brief Class implementing common methods for all IWriter implementations
 */
class VMF_EXPORT WriterBase : public IWriter
{
public:
    /*!
     * \brief Constructor taking ID of compressor to be used at data writing
     * \param _compressorId String identifying compression algorithm
     */
    WriterBase(vmf_string _compressorId = vmf_string()) : IWriter(), compressorId(_compressorId)
    { }

    /*!
     * \brief Creates a new instance of the class
     * \return Smart pointer to new instance
     */
    virtual std::shared_ptr<WriterBase> createNewInstance() const = 0;

protected:
    virtual std::string compress(const std::string& input);

    vmf_string compressorId;
};


}//vmf

#endif /* VMF_WRITER_BASE_HPP */
