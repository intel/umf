#include "vmf/format_encrypted.hpp"
#include "vmf/format_const.hpp"

namespace vmf
{

FormatEncrypted::FormatEncrypted(std::shared_ptr<Format> format,
                                 std::shared_ptr<Encryptor> _encryptor,
                                 bool _ignoreUnknownEncryptor)
    : format(format),
      encryptor(_encryptor),
      ignoreUnknownEncryptor(_ignoreUnknownEncryptor)
{
    eSchema = std::make_shared<vmf::MetadataSchema>(ENCRYPTED_DATA_SCHEMA_NAME);
    VMF_METADATA_BEGIN(ENCRYPTED_DATA_DESC_NAME);
        VMF_FIELD_STR(ENCRYPTION_HINT_PROP_NAME);
        VMF_FIELD_STR(ENCRYPTED_DATA_PROP_NAME);
    VMF_METADATA_END(eSchema);
}


FormatEncrypted::~FormatEncrypted()
{ }


std::string FormatEncrypted::store(const MetadataSet &set,
                                   const std::vector<std::shared_ptr<MetadataSchema> > &schemas,
                                   const std::vector<std::shared_ptr<MetadataStream::VideoSegment> > &segments,
                                   const std::vector<std::shared_ptr<Stat> > &stats,
                                   const AttribMap &attribs)
{
    std::string text = format->store(set, schemas, segments, stats, attribs);
    return encrypt(text);
}


Format::ParseCounters FormatEncrypted::parse(const std::string &text,
                                             std::vector<MetadataInternal> &metadata,
                                             std::vector<std::shared_ptr<MetadataSchema> > &schemas,
                                             std::vector<std::shared_ptr<MetadataStream::VideoSegment> > &segments,
                                             std::vector<std::shared_ptr<Stat> > &stats,
                                             AttribMap &attribs)
{
    std::string decrypted = decrypt(text);
    return format->parse(decrypted, metadata, schemas, segments, stats, attribs);
}


//used to set ID of metadata record
class MetadataAccessor: public Metadata
{
public:
    MetadataAccessor( const std::shared_ptr< MetadataDesc >& spDescription )
      : Metadata(spDescription) { }
    MetadataAccessor( const Metadata& oMetadata )
      : Metadata(oMetadata) { }
    using Metadata::setId;
    virtual ~MetadataAccessor() {}
};


std::string FormatEncrypted::encrypt(const std::string &input)
{
    if(encryptor)
    {
        vmf_rawbuffer encryptedBuf;
        encryptor->encrypt(input, encryptedBuf);
        //Encrypted binary data should be represented in base64
        //because of \0 symbols
        std::string encrypted = Variant::base64encode(encryptedBuf);

        //Store encrypted data in a format of current implementation
        std::shared_ptr<vmf::Metadata> eMetadata;
        eMetadata = std::make_shared<vmf::Metadata>(eSchema->findMetadataDesc(ENCRYPTED_DATA_DESC_NAME));
        eMetadata->push_back(FieldValue(ENCRYPTION_HINT_PROP_NAME, encryptor->getHint()));
        eMetadata->push_back(FieldValue(ENCRYPTED_DATA_PROP_NAME, encrypted));

        MetadataAccessor metadataAccessor(*eMetadata);
        metadataAccessor.setId(0);
        eMetadata = std::make_shared<vmf::Metadata>(metadataAccessor);

        MetadataSet eSet;
        eSet.push_back(eMetadata);
        std::vector< std::shared_ptr<MetadataSchema> > eSchemas;
        eSchemas.push_back(eSchema);

        const IdType nextId = 1;
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
        std::vector<std::shared_ptr<Stat>> stats;
        AttribMap attribs{ {"nextId", to_string(nextId)} };

        //create writer with no wrapping (like compression or encryption) enabled
        std::string outputString;
        outputString = getBackendFormat()->store(eSet, eSchemas, segments, stats, attribs);

        return outputString;
    }
    else
    {
        return input;
    }
}


std::string FormatEncrypted::decrypt(const std::string &input)
{
    //parse it as usual serialized VMF data, search for specific  schemas
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
    std::vector<std::shared_ptr<Stat>> stats;
    AttribMap attribs;

    //any exceptions thrown inside will be passed further
    Format::ParseCounters counters;
    counters = getBackendFormat()->parse(input, metadata, schemas, segments, stats, attribs);

    if(counters.schemas == 1 && schemas.size() == 1 && schemas[0]->getName() == ENCRYPTED_DATA_SCHEMA_NAME)
    {
        MetadataInternal& eMetadata = metadata[0];
        vmf_string hint, data;
        auto hintIt = eMetadata.fields.find(ENCRYPTION_HINT_PROP_NAME);
        auto dataIt = eMetadata.fields.find(ENCRYPTED_DATA_PROP_NAME);
        if(hintIt != eMetadata.fields.end())
            hint = hintIt->second.value;
        if(dataIt != eMetadata.fields.end())
            data = dataIt->second.value;

        if(!encryptor)
        {
            if(!ignoreUnknownEncryptor)
            {
                VMF_EXCEPTION(IncorrectParamException, "No decryptor provided for encrypted data");
            }
            else
            {
                return input;
            }
        }
        else
        {
            try
            {
                std::string decrypted;
                // Encrypted binary data should be represented in base64
                // because of '\0' symbols
                vmf_rawbuffer encrypted = Variant::base64decode(data);
                encryptor->decrypt(encrypted, decrypted);
                return decrypted;
            }
            catch(IncorrectParamException& ee)
            {
                if(ignoreUnknownEncryptor)
                {
                    return input;
                }
                else
                {
                    VMF_EXCEPTION(IncorrectParamException, "Failed to decrypt data, the hint is \"" +
                                  hint + "\": " + ee.what());
                }
            }
        }
    }
    else
    {
        return input;
    }
}

} //vmf
