// Copyright (C) 2013, Intel Corporation, all rights reserved.
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
        VMF_EXCEPTION(NotInitializedException, "ObjectFactory isn't initialized.");
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
