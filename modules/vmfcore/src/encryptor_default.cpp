#include "vmf/encryptor_default.hpp"

#include "filters.h"
#include "default.h"

using namespace std;
using namespace CryptoPP;

namespace vmf
{

void EncryptorDefault::encrypt(const umf_string& input, umf_rawbuffer& output)
{
    string outBuf;
    try
    {
        //or specify length of passphrase: encryptor(passphrase.c_str(), passphrase.length(), ...);
        DefaultEncryptorWithMAC encryptor(passphrase.c_str(), new StringSink(outBuf));
        encryptor.Put((byte*)input.data(), input.length());
        encryptor.MessageEnd();
    }
    catch (CryptoPP::Exception const& e)
    {
        VMF_EXCEPTION(IncorrectParamException, "CryptoPP::Exception caught:" +string(e.what()));
    }
    output = umf_rawbuffer(outBuf.data(), outBuf.length());
}

void EncryptorDefault::decrypt(const umf_rawbuffer& input, umf_string& output)
{
    if(input.size() == 0)
    {
        output = string();
    }
    else
    {
        try
        {
            //or specify length of passphrase: decryptor(passphrase.c_str(), passphrase.length(), ...);
            DefaultDecryptorWithMAC decryptor(passphrase.c_str(), new StringSink(output));
            decryptor.Put((byte*)input.data(), input.size());
            decryptor.MessageEnd();
        }
        catch (CryptoPP::Exception const& e)
        {
            VMF_EXCEPTION(IncorrectParamException, "CryptoPP::Exception caught:" + string(e.what()));
        }
    }
}

}
