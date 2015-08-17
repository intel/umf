// Copyright (C) 2013, Intel Corporation, all rights reserved.

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
