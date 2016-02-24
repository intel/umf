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
#include "xmpdatasource.hpp"

#include "vmf/metadatastream.hpp"

#include "xmpschemasource.hpp"
#include "xmpmetadatasource.hpp"
#include "xmpstatsource.hpp"

#include <XMPUtils.hpp>

#define VMF_GLOBAL_NEXT_ID "next-id"
#define VMF_GLOBAL_CHECKSUM "media-checksum"

#define VMF_INTERNAL "internal"
#define VMF_VIDEO_SEGMENTS "video-segments"
#define VMF_VIDEO_SEGMENT_NAME "name"
#define VMF_VIDEO_SEGMENT_FPS "fps"
#define VMF_VIDEO_SEGMENT_TIME "time"
#define VMF_VIDEO_SEGMENT_DURATION "duration"
#define VMF_VIDEO_SEGMENT_RESOLUTION_W "resolution_width"
#define VMF_VIDEO_SEGMENT_RESOLUTION_H "resolution_height"


using namespace std;
using namespace vmf;

void XMPDataSource::initialize()
{
    try
    {
        if(!SXMPMeta::Initialize())
        {
            VMF_EXCEPTION(DataStorageException, "Internal error during "
                "initialization of XMPMeta.");
        }
        SXMPMeta::RegisterNamespace(VMF_NS, "vmf", NULL);
        XMP_OptionBits xmpFilesOpts = static_cast<XMP_OptionBits>(0);
#ifdef __linux__
        xmpFilesOpts |= kXMPFiles_ServerMode;
#endif
        if(!SXMPFiles::Initialize(xmpFilesOpts))
        {
            VMF_EXCEPTION(DataStorageException, "Internal error during "
                "initialization of XMPFiles.");
        }
    }
    catch (const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch (const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}



void XMPDataSource::terminate()
{
    try
    {
        SXMPFiles::Terminate();
        SXMPMeta::Terminate();
    }
    catch (...)
    {
        // do nothing
    }
}

static const string compressionSchemaName   = "com.intel.vmf.compressed-metadata";
static const string compressedDescName      = "compressed-metadata";
static const string compressedDataPropName  = "data";
static const string compressionAlgoPropName = "algo";

XMPDataSource::XMPDataSource()
  : IDataSource(), xmp(nullptr), metadataSource(nullptr), compressor(nullptr)
{
    schemaCompression = make_shared<vmf::MetadataSchema>(compressionSchemaName);
    VMF_METADATA_BEGIN(compressedDescName);
        VMF_FIELD_STR(compressionAlgoPropName);
        VMF_FIELD_STR(compressedDataPropName);
    VMF_METADATA_END(schemaCompression);
}

XMPDataSource::~XMPDataSource()
{

}

void XMPDataSource::setCompressor(const vmf_string &id)
{
    if(!id.empty())
    {
        compressor = Compressor::create(id);
    }
    else
    {
        compressor = nullptr;
    }
}


void XMPDataSource::loadXMPstructs()
{
    std::shared_ptr<SXMPMeta> compressedXMP = make_shared<SXMPMeta>();
    xmpFile.GetXMP(compressedXMP.get());

    std::shared_ptr<XMPSchemaSource> cSchemaSource = make_shared<XMPSchemaSource>(compressedXMP);
    std::shared_ptr<XMPMetadataSource> cMetaSource = make_shared<XMPMetadataSource>(compressedXMP);
    if (!cMetaSource || !cSchemaSource)
    {
        VMF_EXCEPTION(DataStorageException, "Failed to create metadata source or schema source");
    }

    //load standard VMF metadata and decompress them if there is a corresponding schema
    //or pass them further
    try
    {
        std::map<MetaString, std::shared_ptr<MetadataSchema> > cSchemas;
        cSchemaSource->load(cSchemas);
        auto it = cSchemas.find(compressionSchemaName);
        if(it != cSchemas.end())
        {
            MetadataStream cStream;
            cStream.addSchema(it->second);
            cMetaSource->loadSchema(compressionSchemaName, cStream);
            MetadataSet cSet = cStream.queryBySchema(compressionSchemaName);
            std::shared_ptr<Metadata> cItem = cSet[0];
            vmf_string algo    = cItem->getFieldValue(compressionAlgoPropName);
            vmf_string encoded = cItem->getFieldValue(compressedDataPropName);
            try
            {
                std::shared_ptr<Compressor> decompressor = Compressor::create(algo);
                string decoded;
                XMPUtils::DecodeFromBase64(encoded.data(), encoded.length(), &decoded);
                vmf_rawbuffer compressed(decoded.c_str(), decoded.size());
                string theData;
                decompressor->decompress(compressed, theData);
                xmp->ParseFromBuffer(theData.c_str(), theData.size(), 0);
                schemaSource = make_shared<XMPSchemaSource>(xmp);
                metadataSource = make_shared<XMPMetadataSource>(xmp);
                statSource = make_shared<XMPStatSource>(xmp);
            }
            catch(IncorrectParamException& ce)
            {
                //if there's no such compressor and we're allowed to ignore that
                if(openMode & MetadataStream::OpenModeFlags::IgnoreUnknownCompressor)
                {
                    xmp = compressedXMP;
                    schemaSource = cSchemaSource;
                    metadataSource = cMetaSource;
                    statSource = make_shared<XMPStatSource>(xmp);
                }
                else
                {
                    VMF_EXCEPTION(IncorrectParamException, ce.what());
                }
            }
        }
        else
        {
            xmp = compressedXMP;
            schemaSource = cSchemaSource;
            metadataSource = cMetaSource;
            statSource = make_shared<XMPStatSource>(xmp);
        }
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}


void XMPDataSource::saveXMPstructs()
{
    std::shared_ptr<SXMPMeta> compressedXMP;
    //Sometimes compressed&encoded data is bigger than the source data
    //but there's no need to compare their sizes and write the smallest one.
    //Because due to RDF's verbosity it happens only when the source data is small.
    //That's why the economy wouldn't be significant.
    if(compressor)
    {
        compressedXMP = std::make_shared<SXMPMeta>();

        string buffer;
        XMP_OptionBits options = kXMP_ReadOnlyPacket | kXMP_UseCompactFormat;
        xmp->SerializeToBuffer(&buffer, options, 0, NULL);
        vmf_rawbuffer compressed;
        compressor->compress(buffer, compressed);
        string encoded;
        XMPUtils::EncodeToBase64 (compressed.data(), compressed.size(), &encoded);

        //save compressed data as VMF metadata with corresponding schema
        MetadataStream cStream;
        cStream.addSchema(schemaCompression);
        shared_ptr<Metadata> cMetadata;
        cMetadata = make_shared<Metadata>(schemaCompression->findMetadataDesc(compressedDescName));
        cMetadata->push_back(FieldValue(compressionAlgoPropName, compressor->getId()));
        cMetadata->push_back(FieldValue(compressedDataPropName,  encoded));
        cStream.add(cMetadata);

        std::shared_ptr<XMPMetadataSource> cMetaSource;
        std::shared_ptr<XMPSchemaSource> cSchemaSource;
        cSchemaSource = make_shared<XMPSchemaSource>(compressedXMP);
        cMetaSource = make_shared<XMPMetadataSource>(compressedXMP);
        if (!cMetaSource || !cSchemaSource)
        {
            VMF_EXCEPTION(DataStorageException, "Failed to create compressed metadata source or schema source");
        }

        try
        {
            cMetaSource->saveSchema(compressionSchemaName, cStream);
            cSchemaSource->save(schemaCompression);
        }
        catch(const XMP_Error& e)
        {
            VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
        }
        catch(const std::exception& e)
        {
            VMF_EXCEPTION(DataStorageException, e.what());
        }
        IdType cNextId = 1;
        compressedXMP->SetProperty_Int64(VMF_NS, VMF_GLOBAL_NEXT_ID, cNextId);
    }
    else
    {
        compressedXMP = xmp;
    }
    if(xmpFile.CanPutXMP(*compressedXMP))
    {
        xmpFile.PutXMP(*compressedXMP);
    }
    else
    {
        VMF_EXCEPTION(InternalErrorException, "Can't write XMP data to file");
    }
}


void XMPDataSource::openFile(const MetaString& fileName, MetadataStream::OpenMode mode)
{
    try
    {
        xmp = make_shared<SXMPMeta>();
        openMode = mode;
        metaFileName = fileName;
        XMP_OptionBits modeFlags;
        if (mode & MetadataStream::Update)
        {
            modeFlags = kXMPFiles_OpenForUpdate;
        }
        else
        {
            modeFlags = kXMPFiles_OpenForRead;
        }
        XMP_OptionBits opts = modeFlags | kXMPFiles_OpenUseSmartHandler;
        bool opened = xmpFile.OpenFile(metaFileName, kXMP_UnknownFile, opts);
        if (!opened)
        {
            opts = modeFlags | kXMPFiles_OpenUsePacketScanning;
            opened = xmpFile.OpenFile(metaFileName, kXMP_UnknownFile, opts);
            if (!opened)
            {
                VMF_EXCEPTION(DataStorageException, "Could not open XMP file.");
            }
        }

        loadXMPstructs();
   }
    catch (const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch (const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}



void XMPDataSource::closeFile()
{
    try
    {
        metadataSource.reset();
        schemaSource.reset();
        statSource.reset();
        xmp.reset();
        xmpFile.CloseFile();
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch (const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}


void XMPDataSource::loadSchema(const MetaString& schemaName, MetadataStream& stream)
{
    metadataSourceCheck();
    try
    {
        metadataSource->loadSchema(schemaName, stream);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}



void XMPDataSource::loadProperty(const MetaString &schemaName, const MetaString &propertyName, MetadataStream &stream)
{
    metadataSourceCheck();
    try
    {
        metadataSource->loadProperty(schemaName, propertyName, stream);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}


void XMPDataSource::saveSchema(const MetaString& schemaName, const MetadataStream& stream)
{
    metadataSourceCheck();
    try
    {
        metadataSource->saveSchema(schemaName, stream);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}



void XMPDataSource::save(const std::shared_ptr<vmf::MetadataSchema>& schema)
{
    if (schema == nullptr)
    {
        VMF_EXCEPTION(NullPointerException, "Couldn't save nullptr schema");
    }
    schemaSourceCheck();
    try
    {
        schemaSource->save(schema);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}


void XMPDataSource::serializeAndParse()
{
    string tempBuffer;
    XMP_OptionBits options = kXMP_ReadOnlyPacket | kXMP_UseCompactFormat;
    xmp->SerializeToBuffer(&tempBuffer, options, 0, NULL);

    xmp = make_shared<SXMPMeta>();
    xmp->ParseFromBuffer(tempBuffer.c_str(), tempBuffer.size(), 0);
    schemaSource = make_shared<XMPSchemaSource>(xmp);
    metadataSource = make_shared<XMPMetadataSource>(xmp);
}


void XMPDataSource::remove(const vector<IdType>& ids)
{
    metadataSourceCheck();
    try
    {
        metadataSource->remove(ids);

        /* TODO:
         * Existing implementation requires metadata re-reading here,
         * so a quick workaround is made below:
         * instead of saving to file we just put everything to buffer and get back.
         * To be fixed to eliminate re-reading in long term future.
         */
        serializeAndParse();
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}

void XMPDataSource::load(std::map<MetaString, std::shared_ptr<MetadataSchema> >& schemas)
{
    schemaSourceCheck();
    try
    {
        schemaSource->load(schemas);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}

void XMPDataSource::saveStats(const std::vector< Stat >& stats)
{
    statSourceCheck();
    try
    {
        statSource->save(stats);
        pushChanges();
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}

void XMPDataSource::loadStats(std::vector< Stat >& stats)
{
    statSourceCheck();
    try
    {
        statSource->load(stats);
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
}

void XMPDataSource::clear()
{
    metadataSourceCheck();
    schemaSourceCheck();
    statSourceCheck();
    try
    {
        metadataSource->clear();
        schemaSource->clear();
        statSource->clear();
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }

}

IdType XMPDataSource::loadId()
{
    XMP_Int64 id;
    if(!xmp->GetProperty_Int64(VMF_NS, VMF_GLOBAL_NEXT_ID, &id, nullptr))
    {
        return 0;
    }
    return id;
}

void XMPDataSource::save(const IdType &id)
{
    xmp->SetProperty_Int64(VMF_NS, VMF_GLOBAL_NEXT_ID, id);
}


void XMPDataSource::pushChanges()
{
    saveXMPstructs();

    closeFile();
    openFile(this->metaFileName, this->openMode);
}

void XMPDataSource::metadataSourceCheck()
{
    if (!metadataSource)
    {
        VMF_EXCEPTION(DataStorageException, "Metadata source doesn't exist");
    }
}

void XMPDataSource::schemaSourceCheck()
{
    if (!schemaSource)
    {
        VMF_EXCEPTION(DataStorageException, "Schema source doesn't exist");
    }
}

void XMPDataSource::statSourceCheck()
{
    if (!statSource)
    {
        VMF_EXCEPTION(DataStorageException, "Statistics source doesn't exist");
    }
}

void XMPDataSource::removeSchema(const MetaString &schemaName)
{
    /* TODO:
     * Existing implementation requires metadata re-reading here,
     * so a quick workaround is made below:
     * instead of saving to file we just put everything to buffer and get back.
     * To be fixed to eliminate re-reading in long term future.
     */
    schemaSource->remove(schemaName);
    serializeAndParse();
}

std::string XMPDataSource::computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset)
{
    try
    {
        loadXMPstructs();

        MetaString checksum;
        xmpFile.ComputeChecksum(&checksum, &XMPPacketSize, &XMPPacketOffset);
        return checksum;
    }
    catch(const XMP_Error& e)
    {
        VMF_EXCEPTION(DataStorageException, e.GetErrMsg());
    }
    catch(const std::exception& e)
    {
        VMF_EXCEPTION(DataStorageException, e.what());
    }
    catch(...)
    {
        VMF_EXCEPTION(DataStorageException, "Internal failure. Checksum can't be computed!");
    }
}

std::string XMPDataSource::loadChecksum()
{
    std::string checksum;
    if(!xmp->GetProperty(VMF_NS, VMF_GLOBAL_CHECKSUM, &checksum, NULL) )
        return "";
    return checksum;
}

void XMPDataSource::saveChecksum(const MetaString& checksum)
{
    xmp->SetProperty(VMF_NS, VMF_GLOBAL_CHECKSUM, checksum.c_str());
}

void XMPDataSource::saveVideoSegments(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    xmp->DeleteProperty(VMF_NS, VMF_INTERNAL);
    if (!segments.empty())
    {
        MetaString pathToInternalData;
        xmp->AppendArrayItem(VMF_NS, VMF_INTERNAL, kXMP_PropValueIsArray, NULL, kXMP_PropValueIsStruct);
        SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_INTERNAL, kXMP_ArrayLastItem, &pathToInternalData);
        xmp->SetStructField(VMF_NS, pathToInternalData.c_str(), VMF_NS, VMF_VIDEO_SEGMENTS, nullptr, kXMP_PropValueIsArray);

        std::for_each(segments.begin(), segments.end(), [&](const std::shared_ptr<MetadataStream::VideoSegment>& segment)
        {
            MetaString pathToSegmentsArray;
            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToInternalData.c_str(), VMF_NS, VMF_VIDEO_SEGMENTS, &pathToSegmentsArray);
            xmp->AppendArrayItem(VMF_NS, pathToSegmentsArray.c_str(), kXMP_PropValueIsArray, nullptr, kXMP_PropValueIsStruct);
            MetaString pathToSegment;
            SXMPUtils::ComposeArrayItemPath(VMF_NS, pathToSegmentsArray.c_str(), kXMP_ArrayLastItem, &pathToSegment);

            MetaString tmpPath;

            if (segment->getTitle().empty() || segment->getFPS() <= 0 || segment->getTime() < 0)
                VMF_EXCEPTION(DataStorageException, "Invalid video segment: title, fps or timestamp value(s) is/are invalid!");

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_NAME, &tmpPath);
            xmp->SetProperty(VMF_NS, tmpPath.c_str(), segment->getTitle().c_str());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_FPS, &tmpPath);
            xmp->SetProperty_Float(VMF_NS, tmpPath.c_str(), segment->getFPS());

            SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_TIME, &tmpPath);
            xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), segment->getTime());

            if (segment->getDuration() > 0)
            {
                SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_DURATION, &tmpPath);
                xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), segment->getDuration());
            }

            long width, height;
            segment->getResolution(width, height);
            if (width > 0 && height > 0)
            {
                SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_RESOLUTION_W, &tmpPath);
                xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), width);

                SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_RESOLUTION_H, &tmpPath);
                xmp->SetProperty_Int64(VMF_NS, tmpPath.c_str(), height);
            }
        });
    }
}

void XMPDataSource::loadVideoSegments(std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    MetaString pathToInternalData;
    SXMPUtils::ComposeArrayItemPath(VMF_NS, VMF_INTERNAL, kXMP_ArrayLastItem, &pathToInternalData);

    MetaString pathToSegmentsArray;
    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToInternalData.c_str(), VMF_NS, VMF_VIDEO_SEGMENTS, &pathToSegmentsArray);

    SXMPIterator segmentsIter(*xmp, VMF_NS, pathToSegmentsArray.c_str(), kXMP_IterJustChildren);
    MetaString pathToSegment;
    while (segmentsIter.Next(nullptr, &pathToSegment))
    {
	MetaString tmpPath, segmentTitle;
	double fps;
	long long timestamp, duration;
	XMP_Int32 width, height;

	SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_NAME, &tmpPath);
	if(!xmp->GetProperty(VMF_NS, tmpPath.c_str(), &segmentTitle, 0) )
	    VMF_EXCEPTION(DataStorageException, "Broken video segment's title");

	SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_FPS, &tmpPath);
	if(!xmp->GetProperty_Float(VMF_NS, tmpPath.c_str(), &fps, 0) )
	    VMF_EXCEPTION(DataStorageException, "Broken video segment's FPS");

	SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_TIME, &tmpPath);
	if(!xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &timestamp, 0) )
	    VMF_EXCEPTION(DataStorageException, "Broken video segment's timestamp");

	std::shared_ptr<MetadataStream::VideoSegment> segment = std::make_shared<MetadataStream::VideoSegment>(segmentTitle, fps, timestamp);

	SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_DURATION, &tmpPath);
	if(xmp->GetProperty_Int64(VMF_NS, tmpPath.c_str(), &duration, 0) )
	    segment->setDuration(duration);

	SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_RESOLUTION_W, &tmpPath);

	if(xmp->GetProperty_Int(VMF_NS, tmpPath.c_str(), &width, 0) )
	{
	    SXMPUtils::ComposeStructFieldPath(VMF_NS, pathToSegment.c_str(), VMF_NS, VMF_VIDEO_SEGMENT_RESOLUTION_H, &tmpPath);
	    if(xmp->GetProperty_Int(VMF_NS, tmpPath.c_str(), &height, 0))
		segment->setResolution((long)width, (long)height);
	}
	segments.push_back(segment);
    }
}
