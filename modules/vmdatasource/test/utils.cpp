// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include <fstream>
#include "test_precomp.hpp"

using namespace std;
using namespace vmf;

void copyFile(const string& src, const string& dest)
{
    ifstream source;
    ofstream destination;

    source.open(src, ios_base::binary | ios_base::in);
    destination.open(dest, ios_base::binary | ios_base::out);
    if (!source.is_open() || !destination.is_open())
        VMF_EXCEPTION(vmf::Exception, "Couldn't copy " + src + " to " + dest);
    destination << source.rdbuf();
    destination.close();
    source.close();
}
