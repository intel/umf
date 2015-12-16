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
* \file reader_base.hpp
* \brief %IReader interface declaration
* \details The file contains declaration of %IReader interface.
*/

#pragma once

#ifndef READER_BASE_HPP
#define READER_BASE_HPP

#include "ireader.hpp"

namespace vmf
{

/*!
 * \class ReaderBase
 * \brief Class implementing common methods for all IReader implementations
 */
class VMF_EXPORT ReaderBase : public IReader
{
public:
    ReaderBase(std::shared_ptr<Compressor> impl = nullptr) : IReader(impl)
    { }

protected:
    virtual std::string decompress(const std::string& input);
};


}//vmf

#endif /* READER_BASE_HPP */
