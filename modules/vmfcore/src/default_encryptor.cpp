#include "vmf/default_encryptor.hpp"

#include "dll.h"
#include "filters.h"
#include "default.h"

using namespace std;
using namespace CryptoPP;

namespace vmf
{

void DefaultEncryptor::encrypt(const vmf_string& input, vmf_rawbuffer& output)
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
    output = vmf_rawbuffer(outBuf.data(), outBuf.length());
}

void DefaultEncryptor::decrypt(const vmf_rawbuffer& input, vmf_string& output)
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
