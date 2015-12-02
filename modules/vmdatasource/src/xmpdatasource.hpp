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
#ifndef __XMPDATASOURCE_HPP__
#define __XMPDATASOURCE_HPP__

/*!
* \file xmpdatasource.hpp
* \brief %XMPDataSource header file
*/

#include "datasource.hpp"

#include "vmf/metadataschema.hpp"

#define TXMP_STRING_TYPE vmf::MetaString
#define XMP_INCLUDE_XMPFILES 1
#include <XMP.incl_cpp>
#include <XMP.hpp>

#define VMF_NS "http://ns.intel.com/vmf/2.0"

namespace vmf
{

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class XMPMetadataSource;
class XMPSchemaSource;

/*!
 * \brief Class implements IDataSource interface with usage of XMP
 */
class VMF_EXPORT XMPDataSource: public IDataSource {
public:
    XMPDataSource();

    virtual void openFile(const vmf::MetaString& fileName, vmf::MetadataStream::OpenMode mode);

    virtual void closeFile();

    virtual void loadSchema(const vmf::MetaString& schemaName, vmf::MetadataStream& stream);

    virtual void loadProperty(const vmf::MetaString &schemaName, const vmf::MetaString &propertyName, MetadataStream &stream);

    virtual void saveSchema(const vmf::MetaString& schemaName, const vmf::MetadataStream& stream);

    virtual void save(const std::shared_ptr<vmf::MetadataSchema>& schema);

    virtual void remove(const std::vector<vmf::IdType>& ids);

    virtual void load(std::map<MetaString, std::shared_ptr<vmf::MetadataSchema> >& schemas);

    virtual void clear();

    virtual void save(const vmf::IdType& id);

    virtual vmf::IdType loadId();

    virtual void removeSchema(const MetaString &schemaName);

    virtual void saveChecksum(const MetaString& checksum);

    virtual std::string loadChecksum();

    virtual std::string computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset);

    virtual void saveVideoSegments(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

    virtual void loadVideoSegments(std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

    /*!
     * \brief Initializes XMPDataSource class dependecies
     * \throws DataStorageException
     */
    static void initialize();

    /*!
     * \brief Uninitializes XMPDataSource class dependecies
     */
    static void terminate();

protected:

    virtual void pushChanges();

    virtual void schemaSourceCheck();

    virtual void metadataSourceCheck();

private:
    void loadXMPstructs(SXMPFiles& xmpFile, std::shared_ptr<SXMPMeta>& xmp);
    void saveXMPstructs(SXMPFiles& xmpFile, std::shared_ptr<SXMPMeta>& xmp);

    SXMPFiles xmpFile;
    std::shared_ptr<SXMPMeta> xmp;
    std::shared_ptr<XMPMetadataSource> metadataSource;
    std::shared_ptr<XMPSchemaSource> schemaSource;
    vmf::MetaString metaFileName;
    vmf::MetadataStream::OpenMode openMode;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace vmf

#endif // __XMPDATASOURCE_HPP__
