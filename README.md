[![Build Status](https://travis-ci.org/apavlenko/vmf.svg?branch=vmf-3.0)](https://travis-ci.org/apavlenko/vmf)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/apavlenko/vmf?branch=vmf-3.0&svg=true)](https://ci.appveyor.com/project/apavlenko/vmf/branch/vmf-3.0)
[![Coverage Status](https://coveralls.io/repos/github/apavlenko/vmf/badge.svg?branch=vmf-3.0)](https://coveralls.io/github/apavlenko/vmf?branch=vmf-3.0)
[![codecov.io](https://codecov.io/github/apavlenko/vmf/coverage.svg?branch=vmf-3.0)](https://codecov.io/github/apavlenko/vmf?branch=vmf-3.0)

Video Metadata Framework
------------------------

Video Metadata Framework (VMF) is a C++ library providing functionality for creating, editing and embedding of metadata into video files.
VMF allows developers to define and create metadata of any form, and about any resource type.
VMF embeds metadata within the host video file or video stream, so that no additional files are needed to store the metadata.

VMF SDK is developed as a platform-independent C++ library. It is tested on the following operating system:
* MS Windows 7, 8, and 10
* GNU Linux (Ubuntu 14)
* Google Android 4.x

VMF SDK and the samples code uses the following C++ 11 features:
* `auto`
* `nullptr`
* rvalue references and move semantics
* lambda expressions

The following C++ compilers are known to compile the VMF code successfully:
* MS VC++ 2013 and newer
* Intel C++ compiler 12.1 and newer
* GNU GCC 4.6 and newer


The library uses the following 3rd party components:
* Adobe XMP Toolkit SDK (http://www.adobe.com/devnet/xmp.html)
* Google C++ Testing Framework (aka GTest, https://code.google.com/p/googletest/)

VMF uses Doxygen (www.doxygen.org) for API Specification generation.

See docs/VMF_User_guide.pdf for instructions on building and using VMF.

For the less adventurous, pre-built SDK snapshots may be found here -> https://software.intel.com/en-us/articles/intel-video-metadata-framework-sdk
