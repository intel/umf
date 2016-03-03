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

#include "test_precomp.hpp"

using namespace vmf;

enum CryptAlgo
{
    DEFAULT, BAD, NONE
};

//Some testing class for encryption
class BadEncryptor : public vmf::Encryptor
{
public:
    BadEncryptor(char _key) : key(_key) { }

    virtual void encrypt(const vmf_string &input, vmf_rawbuffer &output)
    {
        output.clear();
        output.reserve(input.length());
        for(char c : input)
        {
            output.push_back(c ^ key);
        }
    }

    virtual void decrypt(const vmf_rawbuffer &input, vmf_string &output)
    {
        output.clear();
        output.reserve(input.size());
        for(char c : input)
        {
            output.push_back(c ^ key);
        }
    }

    virtual vmf_string getHint()
    {
        return vmf_string("bad encryptor for tests");
    }

    virtual ~BadEncryptor() { }

private:
    char key;
};

class TestEncryptor : public ::testing::TestWithParam<CryptAlgo>
{
protected:
    void SetUp()
    {
        vmf::initialize();
    }

    void TearDown()
    {
        vmf::terminate();
    }

    std::shared_ptr<vmf::Encryptor> getEncryptor(CryptAlgo algo)
    {
        switch(algo)
        {
            case CryptAlgo::DEFAULT:
                return std::make_shared<DefaultEncryptor>("thereisnospoon");
            case CryptAlgo::BAD:
                return std::make_shared<BadEncryptor>(42);
            default:
                return nullptr;
        }
    }

    std::string generateData(int nChars)
    {
        std::string result(nChars, '\0');
        std::generate_n(result.begin(), nChars, []()->char {return (rand()%254) + 1;});
        return result;
    }
};


TEST_P(TestEncryptor, LossesOnCompression)
{
    std::shared_ptr<Encryptor> encryptor = getEncryptor(GetParam());
    int nChars = 0;
    do
    {
        std::string data = generateData(nChars);
        vmf_rawbuffer encrypted;
        encryptor->encrypt(data, encrypted);
        std::string result;
        encryptor->decrypt(encrypted, result);
        ASSERT_EQ(data, result);
        nChars = nChars ? 2*nChars : 1;
    }
    while(nChars < (1 << 20));
}


TEST_P(TestEncryptor, DecryptEmpty)
{
    std::shared_ptr<Encryptor> encryptor = getEncryptor(GetParam());

    vmf_rawbuffer encrypted;
    std::string result;
    ASSERT_NO_THROW(encryptor->decrypt(encrypted, result));
    ASSERT_TRUE(result.empty());
}


INSTANTIATE_TEST_CASE_P(UnitTest, TestEncryptor,
                        ::testing::Values(CryptAlgo::DEFAULT, CryptAlgo::BAD));
