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

/*
 * File copying routine for use in samples.
 */

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

void copyFile(const string& srcName, const char *dstName)
{
    char buf[BUFSIZ];
    size_t size;

    FILE* source;
#if defined(__linux__) || defined(__APPLE__)
    if (!(source = fopen(srcName.c_str(), "rb")))
#else
    if (fopen_s(&source, srcName.c_str(), "rb") != 0)
#endif
    {
        cerr << "Unable to open source file '" << srcName << "'" << endl;
        exit(1);
    }

    FILE* dest;
#if defined(__linux__) || defined(__APPLE__)
    if (!(dest = fopen(dstName, "wb")))
#else
    if (fopen_s(&dest, dstName, "wb") != 0)
#endif
    {
        cerr << "Unable to open destination file '" << dstName << "'" << endl;
        fclose(source);
        exit(1);
    }

    while ((size = fread(buf, 1, BUFSIZ, source)))
    {
        fwrite(buf, 1, size, dest);
    }

    fclose(source);
    fclose(dest);
}
