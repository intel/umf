// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "test_precomp.hpp"

std::string workingPath;

int main(int argc, char **argv)
{
    std::string appPath = argv[0];
#ifdef WIN32
    char delim = '\\';
#else
    char delim = '/';
#endif
    size_t pos = appPath.find_last_of(delim);

    if (pos != std::string::npos)
    {
        workingPath = appPath.substr(0, pos);
    }
    else
    {
        workingPath = ".";
    }

    ::testing::InitGoogleTest(&argc, argv);
    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);
    return RUN_ALL_TESTS();
}
