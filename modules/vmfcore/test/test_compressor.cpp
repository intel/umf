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

//Bad compressor that increases the size of input data
class BloatingCompressor : public vmf::Compressor
{
public:
    static const int factor = 5;

    virtual void compress(const vmf::vmf_string& input, vmf::vmf_rawbuffer& output)
    {
        output.clear();
        for(auto it = input.begin(); it != input.end(); ++it)
        {
            for(int i = 0; i < factor; i++)
                output.push_back(*it);
        }
    }

    virtual void decompress(const vmf::vmf_rawbuffer& input, vmf::vmf_string &output)
    {
        output.clear();
        for(auto it = input.begin(); it != input.end();)
        {
            char c = *it++;
            for(int i = 1; i < factor; i++)
            {
                char cc = *it++;
                if(cc != c)
                {
                    throw std::runtime_error("Incorrect input to bloating compressor");
                }
            }
            output.push_back(c);
        }
    }

    std::shared_ptr<Compressor> createNewInstance() const
    {
        return std::shared_ptr<Compressor>(new BloatingCompressor);
    }

    virtual vmf::vmf_string getId()
    {
        return "com.intel.vmf.compressor.test.bloating";
    }
};


class TestCompressor : public ::testing::TestWithParam<std::string>
{
protected:
    void SetUp()
    {
        vmf::initialize();
        //register bloating compressor
        std::shared_ptr<Compressor> bloating = std::make_shared<BloatingCompressor>();
        vmf::Compressor::registerNew(bloating);
    }

    void TearDown()
    {
        vmf::Compressor::unregister("com.intel.vmf.compressor.test.bloating");
        vmf::terminate();
    }

    std::string generateData(int nChars)
    {
        std::string result;
        srand(0);
        for(int i = 0; i < nChars; i++)
        {
            result.push_back( (char)(rand()%256) );
        }
        return result;
    }

    std::shared_ptr<Compressor> compressor;
};


TEST_P(TestCompressor, CreateByName)
{
    std::string name = GetParam();

    compressor = vmf::Compressor::create(name);
    if(name == "unregistered")
    {
        ASSERT_EQ(compressor, nullptr);
    }
    else
    {
        ASSERT_NE(compressor, nullptr);
    }
}


TEST_P(TestCompressor, LossesOnCompression)
{
    std::string name = GetParam();

    if(name != "unregistered")
    {
        compressor = vmf::Compressor::create(name);
        int nChars = 0;
        do
        {
            std::string data = generateData(nChars);
            vmf_rawbuffer compressed;
            compressor->compress(data, compressed);
            std::string result;
            compressor->decompress(compressed, result);
            ASSERT_EQ(data, result);
            nChars = nChars ? 2*nChars : 1;
        }
        while(nChars < (1 << 24));
    }
}


TEST_P(TestCompressor, DummyDecompression)
{
    std::string name = GetParam();

    //we can only expect undefined behaviour
    //on other compressors when decompressing random data
    if(name == "com.intel.vmf.compressor.dummy")
    {
        compressor = vmf::Compressor::create(name);

        int nChars = 0;
        do
        {
            std::string garbage = generateData(nChars);
            vmf_rawbuffer compressed(garbage.data(), garbage.size());
            std::string result;
            ASSERT_NO_THROW(compressor->decompress(compressed, result));
            ASSERT_EQ(garbage, result);
            nChars = nChars ? 2*nChars : 8;
        }
        while(nChars < (1 << 24));
    }
}


TEST_P(TestCompressor, DecompressionOfEmpty)
{
    std::string name = GetParam();

    if(name != "unregistered")
    {
        compressor = vmf::Compressor::create(name);
        vmf_rawbuffer compressed;
        std::string result;
        ASSERT_NO_THROW(compressor->decompress(compressed, result));
        ASSERT_TRUE(result.empty());
    }
}


TEST_P(TestCompressor, CheckRegisteredIds)
{
    std::vector<vmf_string> regIds = vmf::Compressor::getRegisteredIds();
    std::set<vmf_string> registeredIds(regIds.begin(), regIds.end());
    std::set<vmf_string> knownIds = {"com.intel.vmf.compressor.dummy",
                                     "com.intel.vmf.compressor.zlib",
                                     "com.intel.vmf.compressor.test.bloating"};
    ASSERT_EQ(registeredIds, knownIds);
}


//Compressor class to pass fake Id to registerNew() method
class FakeCompressor : public CompressorDummy
{
public:
    std::shared_ptr<Compressor> createNewInstance() const
    {
        return std::shared_ptr<Compressor>(new BloatingCompressor);
    }

    virtual vmf::vmf_string getId()
    {
        return id;
    }

    void setId(vmf_string s)
    {
        id = s;
    }

private:
    vmf_string id;
};


TEST_P(TestCompressor, TryRegisterExisting)
{
    std::string name = GetParam();
    std::shared_ptr<Compressor> fake = std::make_shared<FakeCompressor>();
    std::dynamic_pointer_cast<FakeCompressor>(fake)->setId(name);
    if(name == "unregistered")
    {
        ASSERT_NO_THROW(vmf::Compressor::registerNew(fake));
    }
    else
    {
        ASSERT_THROW(vmf::Compressor::registerNew(fake), IncorrectParamException);
    }
}


INSTANTIATE_TEST_CASE_P(UnitTest, TestCompressor,
                        ::testing::Values("com.intel.vmf.compressor.dummy",
                                          "com.intel.vmf.compressor.zlib",
                                          "unregistered",
                                          "com.intel.vmf.compressor.test.bloating"));

