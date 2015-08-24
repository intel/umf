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
