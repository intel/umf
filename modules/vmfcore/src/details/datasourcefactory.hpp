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
#ifndef __VMF_DATASOURCEFACTORY_HPP__
#define __VMF_DATASOURCEFACTORY_HPP__

/*!
 * \file datasourcefactory.hpp
 * \brief IDataSourceFactory interface header file
 */

#include "umf/umf.hpp"

namespace umf
{

class IDataSource;

/*! \class IDataSourceFactory
 * \brief Interface for factory, which creates IDataSource instances
 */
class IDataSourceFactory
{
public:
    /*!
      \brief create new data IDataSource instance
     */
    virtual std::shared_ptr<IDataSource> createDataSource() = 0;
private:
};

} // namespace umf

#endif // __VMF_DATASOURCEFACTORY_HPP__
