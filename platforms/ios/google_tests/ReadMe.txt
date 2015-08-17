For generating google test Xcode project please use following format of command:
cmake -DVMF_FRAMEWORK=<path to vmf.framework> -DCMAKE_TOOLCHAIN_FILE=/Users/itseez/Projects/vmf/platforms/ios/cmake/Toolchains/Toolchain-iPhoneOS_Xcode.cmake /Users/itseez/Projects/vmf/platforms/ios/google_tests/ -G Xcode
