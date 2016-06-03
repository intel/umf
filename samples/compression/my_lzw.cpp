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

#include "my_lzw.hpp"

using namespace std;
using namespace vmf;

// Straightforward implementation of LZW algorithm
// Based on this: http://rosettacode.org/wiki/LZW_compression#C.2B.2B

void MyLZWCompressor::compress(const vmf_string &input, vmf_rawbuffer& output)
{
    // Build the dictionary.
    int dictSize = 256;
    map< string, int > dictionary;
    for (int i = 0; i < 256; i++)
    {
        dictionary[std::string(1, i)] = i;
    }

    vector<int> codes;
    std::string w;
    for (auto it = input.begin(); it != input.end(); ++it)
    {
        char c = *it;
        std::string wc = w + c;
        if (dictionary.count(wc))
        {
            w = wc;
        }
        else
        {
            codes.push_back(dictionary[w]); //*result++ = dictionary[w];
            // Add wc to the dictionary.
            dictionary[wc] = dictSize++;
            w = std::string(1, c);
        }
    }

    // Output the code for w.
    if (!w.empty())
        codes.push_back(dictionary[w]); //*result++ = dictionary[w];

    //according to C++ standard, this should be correct way to convert vector to array
    output = vmf_rawbuffer((const char*)codes.data(), codes.size()*sizeof(int));
}

void MyLZWCompressor::decompress(const vmf_rawbuffer& input, vmf_string& output)
{
    if(input.empty())
    {
        output = vmf_string();
    }
    else
    {
        // Build the dictionary.
        int dictSize = 256;
        std::map< int, std::string > dictionary;
        for (int i = 0; i < 256; i++)
        {
            dictionary[i] = std::string(1, i);
        }

        //load codes from raw buffer to vector<int>
        size_t nCodes = input.size()/sizeof(int);
        int* bufCodes = (int*)input.data();
        vector<int> codes;
        copy_n(bufCodes, nCodes,  std::back_inserter(codes));

        size_t idx = 0;
        std::string w(1, codes[idx++]); //std::string w(1, *begin++);
        output = w;
        std::string entry;
        for ( ; idx < nCodes; idx++)
        {
            int k = codes[idx]; //int k = *begin;
            if (dictionary.count(k))
            {
                entry = dictionary[k];
            }
            else if (k == dictSize)
            {
                entry = w + w[0];
            }
            else
            {
                throw std::runtime_error("LZW decompression error: bad input index");
            }

            output += entry;

            // Add w+entry[0] to the dictionary.
            dictionary[dictSize++] = w + entry[0];

            w = entry;
        }
    }
}
