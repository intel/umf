rmdir /s /q build
mkdir build
chdir build
set make_utility="%ANDROID_NDK%\prebuilt\windows\bin\make.exe"
if not exist "%make_utility" (
    set make_utility="%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe"
)
cmake.exe -G"MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="..\cmake\android.toolchain.cmake" -DCMAKE_MAKE_PROGRAM="%make_utility%" -DANDROID_ABI=x86 -DANDROID_STL=gnustl_shared ..
cmake.exe --build .
chdir ..
