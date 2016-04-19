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

    std::string generateData(int nChars)
    {
        std::string result(nChars, '\0');
        std::generate_n(result.begin(), nChars, []()->char {return (rand()%254) + 1;});
        return result;
    }
};


TEST_P(TestEncryptor, LossesOnEncryption)
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


TEST_P(TestEncryptor, DecryptWrongPassword)
{
    std::shared_ptr<Encryptor> rightEncryptor = getEncryptor(GetParam(), false);
    std::shared_ptr<Encryptor> wrongEncryptor = getEncryptor(GetParam(), true);
    //decryption of empty input should always produce empty output, don't check it
    int nChars = 1;
    do
    {
        std::string data = generateData(nChars);
        vmf_rawbuffer encrypted;
        rightEncryptor->encrypt(data, encrypted);
        std::string wrongResult;
        ASSERT_THROW(wrongEncryptor->decrypt(encrypted, wrongResult), IncorrectParamException);
        ASSERT_NE(data, wrongResult);
        nChars *= 2;
    }
    while(nChars < (1 << 20));
}


INSTANTIATE_TEST_CASE_P(UnitTest, TestEncryptor,
                        ::testing::Values(CryptAlgo::DEFAULT, CryptAlgo::WEAK));
