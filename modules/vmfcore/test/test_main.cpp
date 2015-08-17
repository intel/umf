// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "test_precomp.hpp"

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);

    return RUN_ALL_TESTS();
}
