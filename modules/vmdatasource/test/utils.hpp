// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __TEST_UTILS_HPP__
#define __TEST_UTILS_HPP__

#include <string>

extern std::string workingPath;
#define VIDEO_FILE (workingPath + "/BlueSquare.avi")

#define atom(X) X (#X)

void copyFile(const std::string& src, const std::string& dest);

#endif // __TEST_UTILS_HPP__
