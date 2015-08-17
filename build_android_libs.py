from subprocess import call
import sys
import os

BUILD_DIR = 'android_build'
OUTPUT_DIR = 'android_libs'
ARCHS = ['armeabi-v7a', 'armeabi', 'x86']

def rm_dir(directory):
    try:
        return call('rmdir /s /q ' + directory, shell=True)
    except:
        return -1

def mk_dir(directory):
    try:
        return call('mkdir ' + directory, shell=True)
    except:
        return -1

def compile_android_lib(arch, build_type):
    android_ndk = os.environ['ANDROID_NDK']
    make = android_ndk + r'\prebuilt\windows\bin\make.exe'
    if not os.path.isfile(make):
        make = android_ndk + r'\prebuilt\windows-x86_64\bin\make.exe'
    out_dir = OUTPUT_DIR + '\\' + arch
    mk_dir(BUILD_DIR)
    mk_dir(out_dir)

    os.chdir(BUILD_DIR)
    if call('cmake.exe -G"MinGW Makefiles" \
-DCMAKE_TOOLCHAIN_FILE="..\\cmake\\android.toolchain.cmake" \
-DANDROID_ABI="%s" \
-DANDROID_STL=gnustl_shared \
-DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DBUILD_SHARED_LIBS=ON \
-DCMAKE_MAKE_PROGRAM="%s" -DCMAKE_BUILD_TYPE="%s" ..' % (arch, make, build_type), shell=True) != 0:
        raise RuntimeError('cmake failed on arch ' + arch)
    if call('cmake.exe --build .', shell=True) != 0:
        raise RuntimeError('make failed on arch ' + arch)
    os.chdir('..')
    if call('copy ' + BUILD_DIR + r'\lib\libvmf.so' + ' ' + out_dir, shell=True) != 0 and call('copy ' + BUILD_DIR + r'\lib\libvmfd.so'+ ' ' + out_dir, shell=True) != 0:
        raise RuntimeError('failed to copy output lib')
    rm_dir(BUILD_DIR)
    rm_dir('libs')

if __name__ == '__main__':
    print 'Running in directory "' + os.getcwd() + '"'
    rm_dir(OUTPUT_DIR)
    for arch in ARCHS:
        compile_android_lib(arch, "Debug")
        compile_android_lib(arch, "Release")
    pass

