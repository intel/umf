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
#include <fstream>
#include "test_precomp.hpp"

using namespace std;
using namespace umf;

void copyFile(const string& src, const string& dest)
{
    ifstream source;
    ofstream destination;

    source.open(src, ios_base::binary | ios_base::in);
    destination.open(dest, ios_base::binary | ios_base::out);
    if (!source.is_open() || !destination.is_open())
        UMF_EXCEPTION(umf::Exception, "Couldn't copy " + src + " to " + dest);
    destination << source.rdbuf();
    destination.close();
    source.close();
}
