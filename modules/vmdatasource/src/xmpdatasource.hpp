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
#include "vmf/compressor.hpp"
#include "vmf/encryptor.hpp"

#define TXMP_STRING_TYPE vmf::vmf_string
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
class XMPStatSource;

/*!
 * \class XMPDataSource
 * \brief Class implements IDataSource interface with usage of XMP
 */
class VMF_EXPORT XMPDataSource: public IDataSource {
public:
    XMPDataSource();

    ~XMPDataSource();

    virtual void openFile(const vmf::vmf_string& fileName, vmf::MetadataStream::OpenMode mode);

    virtual void closeFile();

    virtual void loadSchema(const vmf::vmf_string& schemaName, vmf::MetadataStream& stream);

    virtual void loadProperty(const vmf::vmf_string &schemaName, const vmf::vmf_string &propertyName, MetadataStream &stream);

    virtual void saveSchema(const std::shared_ptr<MetadataSchema>& schemaDesc, const MetadataSet& mdSet);

    virtual void save(const std::shared_ptr<vmf::MetadataSchema>& schema);

    virtual void remove(const std::vector<vmf::IdType>& ids);

    virtual void load(std::map<vmf_string, std::shared_ptr<vmf::MetadataSchema> >& schemas);

    virtual void clear();

    virtual void save(const vmf::IdType& id);

    virtual vmf::IdType loadId();

    virtual void removeSchema(const vmf_string &schemaName);

    virtual void saveChecksum(const vmf_string& checksum);

    virtual std::string loadChecksum();

    virtual std::string computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset);

    virtual void saveVideoSegments(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

    virtual void loadVideoSegments(std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

    virtual void setCompressor(const vmf_string& id);

    virtual void setEncryptor(std::shared_ptr<Encryptor> _encryptor);

    virtual std::string loadHintEncryption();

    virtual void saveHintEncryption(const vmf_string& hint);

    virtual void pushChanges();

    virtual void saveStats(const std::vector< std::shared_ptr<Stat> >& stats);

    virtual void loadStats(std::vector< std::shared_ptr<Stat> >& stats);

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

    virtual void schemaSourceCheck();

    virtual void metadataSourceCheck();

    virtual void statSourceCheck();

private:
    void loadXMPstructs();
    void saveXMPstructs();
    void serializeAndParse();

    SXMPFiles xmpFile;
    std::shared_ptr<SXMPMeta> xmp;
    std::shared_ptr<XMPMetadataSource> metadataSource;
    std::shared_ptr<XMPSchemaSource> schemaSource;
    std::shared_ptr<XMPStatSource> statSource;
    vmf::vmf_string metaFileName;
    vmf::MetadataStream::OpenMode openMode;
    std::shared_ptr<Compressor> compressor;
    std::shared_ptr<Encryptor>  encryptor;
    std::shared_ptr<vmf::MetadataSchema> schemaCompression;
    std::shared_ptr<vmf::MetadataSchema> schemaEncryption;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace vmf

#endif // __XMPDATASOURCE_HPP__
