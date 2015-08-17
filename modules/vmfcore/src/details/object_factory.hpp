// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __VMF_OBJECT_FACTORY_HPP__
#define __VMF_OBJECT_FACTORY_HPP__

#include <memory>
#include "config.hpp"

namespace vmf
{

class IDataSourceFactory;

VMF_EXPORT void Initialize(std::shared_ptr<IDataSourceFactory>& dataSourceFactory);
VMF_EXPORT void Uninitialize();

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class IDataSource;
class IDataSourceFactory;

class VMF_EXPORT ObjectFactory
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
