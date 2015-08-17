// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "vmf/vmdatasource.hpp"

#include "vmf/vmf.hpp"
#include "datasourcefactory.hpp"
#include "object_factory.hpp"
#include "xmpdatasource.hpp"

using namespace std;

namespace vmf
{

class XMPDataSourceFactory: public IDataSourceFactory
{
public:
    XMPDataSourceFactory()
    {
        XMPDataSource::initialize();
    }

    virtual shared_ptr<IDataSource> createDataSource()
    {
        shared_ptr<IDataSource> ds = make_shared<XMPDataSource>();
        return ds;
    }

    virtual ~XMPDataSourceFactory()
    {
        XMPDataSource::terminate();
    }
};

void initialize()
{
    shared_ptr<IDataSourceFactory> dsf(static_cast<IDataSourceFactory*>(new XMPDataSourceFactory()));
    Initialize(dsf);
}

void terminate()
{
    Uninitialize();
}

} // namespace vmf
