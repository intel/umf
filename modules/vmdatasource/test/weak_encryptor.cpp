#include "weak_encryptor.hpp"
#include "vmf/encryptor_default.hpp"

namespace vmf
{

const char WeakEncryptor::magic = 109;

void WeakEncryptor::encrypt(const vmf_string &input, vmf_rawbuffer &output)
{
    output.clear();
    output.reserve(input.length()+1);
    output.push_back(magic ^ key);
    for(char c : input)
    {
        output.push_back(c ^ key);
    }
}


void WeakEncryptor::decrypt(const vmf_rawbuffer &input, vmf_string &output)
{
    output.clear();
    if(!input.empty())
    {
        output.reserve(input.size()-1);
        if((input[0] ^ key) != magic)
        {
            throw IncorrectParamException("Incorrect decryption key!");
        }
        for(size_t i = 1; i < input.size(); i++)
        {
            output.push_back(input[i] ^ key);
        }
    }
}


std::shared_ptr<vmf::Encryptor> getEncryptor(CryptAlgo algo, bool wrong)
{
    std::string wrongKey = "goodbyemranderson";
    std::string rightKey = "thereisnospoon";
    switch(algo)
    {
        case CryptAlgo::DEFAULT:
            return std::make_shared<EncryptorDefault>(wrong ? wrongKey : rightKey);
        case CryptAlgo::WEAK:
            return std::make_shared<WeakEncryptor>(wrong ? 13 : 42);
        default:
            return nullptr;
    }
}

}
