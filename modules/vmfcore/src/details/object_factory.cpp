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
#include "vmf/exceptions.hpp"
#include "object_factory.hpp"
#include "datasourcefactory.hpp"

namespace vmf
{
std::unique_ptr<ObjectFactory> ObjectFactory::instance;

void Initialize(std::shared_ptr<IDataSourceFactory> &dataSourceFactory)
{
    if (!dataSourceFactory)
    {
        VMF_EXCEPTION(IncorrectParamException, "Unable to initialize by empty DataSourceFactory instance");
    }

    ObjectFactory *instance = ObjectFactory::getInstance();

    if (instance == NULL)
    {
        VMF_EXCEPTION(InternalErrorException, "Can't get Object factory instance");
    }

    instance->setDataSourceFactory(dataSourceFactory);
}

void Uninitialize()
{
    ObjectFactory *instance = ObjectFactory::getInstance();

    if (instance == NULL)
    {
        VMF_EXCEPTION(InternalErrorException, "Can't get Object factory instance");
    }

    std::shared_ptr<IDataSourceFactory> emptyDataSourceFactoryPtr;
    instance->setDataSourceFactory(emptyDataSourceFactoryPtr);
}

ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
    Uninitialize();
}

ObjectFactory* ObjectFactory::getInstance()
{
    if (!instance)
    {
        instance = std::unique_ptr<ObjectFactory>(new ObjectFactory);
    }

    return instance.get();
}

std::shared_ptr<IDataSource> ObjectFactory::getDataSource()
{
    if (!dataSourceFactoryInstance)
    {
        vmf::initialize();
    }
    std::shared_ptr<IDataSource> dataSource =
            dataSourceFactoryInstance->createDataSource();
    return dataSource;
}

void ObjectFactory::setDataSourceFactory(std::shared_ptr<IDataSourceFactory>& dataSourceFactory)
{
    dataSourceFactoryInstance = dataSourceFactory;
}
} // namespace vmf
