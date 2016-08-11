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
#ifndef __VMF_OBJECT_FACTORY_HPP__
#define __VMF_OBJECT_FACTORY_HPP__

#include <memory>
#include "config.hpp"

namespace vmf
{

class IDataSourceFactory;

UMF_EXPORT void Initialize(std::shared_ptr<IDataSourceFactory>& dataSourceFactory);
UMF_EXPORT void Uninitialize();

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class IDataSource;
class IDataSourceFactory;

class UMF_EXPORT ObjectFactory
{
private:
    ObjectFactory();

public:
    virtual ~ObjectFactory();

    static ObjectFactory* getInstance();
    std::shared_ptr<IDataSource> getDataSource();
    void setDataSourceFactory(std::shared_ptr<IDataSourceFactory>& dataSourceFactory);

protected:
    ObjectFactory& operator=(ObjectFactory&) {return *this; }

private:
    static std::unique_ptr<ObjectFactory> instance;
    std::shared_ptr<IDataSourceFactory> dataSourceFactoryInstance;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} /* vmf */

#endif /* __VMF_OBJECT_FACTORY_HPP__ */
