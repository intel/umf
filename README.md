DISCONTINUATION OF PROJECT

This project will no longer be maintained by Intel.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project.  

Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project.  

Contact: webadmin@linux.intel.com

Universal Metadata Framework
------------------------

The Universal Metadata Framework (UMF) is a C++ library providing functionality for creating, editing and embedding of metadata into video files.
UMF allows developers to define and create metadata of any form, and about any resource type.
UMF embeds metadata within the host video file or video stream, so that no additional files are needed to store the metadata.

The UMF SDK is developed as a platform-independent C++ library. It is tested on the following operating system:
* MS Windows 7, 8, and 10
* GNU Linux (Ubuntu 14)
* Google Android 4.x, 5.x

The UMF SDK and the sample code use the following C++ 11 features:
* `auto`
* `nullptr`
* rvalue references and move semantics
* lambda expressions
* range `for`
* `std::thread`

The following C++ compilers are known to compile the UMF SDK and sample code successfully:
* MS VC++ 2013 and newer
* Intel C++ compiler 12.1 and newer
* GNU GCC 4.7 and newer


The library uses the following 3rd party components:
* Adobe XMP Toolkit SDK (http://www.adobe.com/devnet/xmp.html)
* Google C++ Testing Framework (aka GTest, https://code.google.com/p/googletest/)

The UMF SDK uses Doxygen (www.doxygen.org) for API Specification generation.

See docs/UMF_User_guide.pdf for instructions on building and using UMF.

Contact the project lead Erik Niemeyer (GitHub UserID "eaniemey") with questions or comments.
