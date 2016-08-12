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

using namespace umf;

//Bad compressor that increases the size of input data
class BloatingCompressor : public umf::Compressor
{
public:
    static const int factor = 5;

    virtual void compress(const umf::umf_string& input, umf::umf_rawbuffer& output)
    {
        output.clear();
        for(auto it = input.begin(); it != input.end(); ++it)
        {
            for(int i = 0; i < factor; i++)
                output.push_back(*it);
        }
    }

    virtual void decompress(const umf::umf_rawbuffer& input, umf::umf_string &output)
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

    virtual umf::umf_string getId()
    {
        return "com.intel.vmf.compressor.test.bloating";
    }
};


class TestCompressor : public ::testing::TestWithParam<std::string>
{
protected:
    void SetUp()
    {
        //umf::initialize();
        //register bloating compressor
        std::shared_ptr<Compressor> bloating = std::make_shared<BloatingCompressor>();
        umf::Compressor::registerNew(bloating);
    }

    void TearDown()
    {
        umf::Compressor::unregister("com.intel.vmf.compressor.test.bloating");
        //umf::terminate();
    }

    std::string generateData(int nChars)
    {
        std::string result(nChars, '\0');
        std::generate_n(result.begin(), nChars, []()->char {return (rand()%254) + 1;});
        return result;
    }

    std::shared_ptr<Compressor> compressor;
};

TEST_P(TestCompressor, TryRegisterNewNull)
{
    std::shared_ptr<Compressor> newBloating = NULL;
    ASSERT_THROW(umf::Compressor::registerNew(newBloating), IncorrectParamException);
}

TEST_P(TestCompressor, TryUnregisterNonExisting)
{
    ASSERT_THROW(umf::Compressor::unregister("trampampam"), IncorrectParamException);
}

TEST_P(TestCompressor, CreateByName)
{
    std::string name = GetParam();

    if(name == "unregistered")
    {
        ASSERT_THROW(compressor = umf::Compressor::create(name), IncorrectParamException);
    }
    else
    {
        ASSERT_NE(compressor = umf::Compressor::create(name), nullptr);
    }
}


TEST_P(TestCompressor, LossesOnCompression)
{
    std::string name = GetParam();

    if(name != "unregistered")
    {
        compressor = umf::Compressor::create(name);
        int nChars = 0;
        do
        {
            std::string data = generateData(nChars);
            umf_rawbuffer compressed;
            compressor->compress(data, compressed);
            std::string result;
            compressor->decompress(compressed, result);
            ASSERT_EQ(data, result);
            nChars = nChars ? 2*nChars : 1;
        }
        while(nChars < (1 << 20));
    }
}


TEST_P(TestCompressor, DecompressionOfEmpty)
{
    std::string name = GetParam();

    if(name != "unregistered")
    {
        compressor = umf::Compressor::create(name);
        umf_rawbuffer compressed;
        std::string result;
        ASSERT_NO_THROW(compressor->decompress(compressed, result));
        ASSERT_TRUE(result.empty());
    }
}


TEST_P(TestCompressor, CheckRegisteredIds)
{
    std::vector<umf_string> regIds = umf::Compressor::getRegisteredIds();
    std::set<umf_string> registeredIds(regIds.begin(), regIds.end());
    std::set<umf_string> knownIds = { Compressor::builtinId(),
                                      "com.intel.vmf.compressor.test.bloating" };
    ASSERT_EQ(registeredIds, knownIds);
}


TEST_P(TestCompressor, TryRegisterExisting)
{
    std::string name = GetParam();
    std::shared_ptr<Compressor> fake = std::make_shared<FakeCompressor>();
    std::dynamic_pointer_cast<FakeCompressor>(fake)->setId(name);
    if(name == "unregistered")
    {
        ASSERT_NO_THROW(umf::Compressor::registerNew(fake));
        ASSERT_NO_THROW(umf::Compressor::unregister(name));
    }
    else
    {
        ASSERT_THROW(umf::Compressor::registerNew(fake), IncorrectParamException);
    }
}


INSTANTIATE_TEST_CASE_P(UnitTest, TestCompressor,
                        ::testing::Values(Compressor::builtinId(),
                                          "unregistered",
                                          "com.intel.vmf.compressor.test.bloating"));

