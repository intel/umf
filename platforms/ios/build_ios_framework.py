#!/usr/bin/env python
"""
The script builds vmf.framework for iOS.

Usage:
    ./build_ios_framework.py <outputdir>
"""

import glob, re, os, os.path, shutil, string, sys

def build_vmf(src, build, target, arc, config):
    "builds VMF for device or simulator"

    build_dir = os.path.join(build, target + '-' + arc)
    if not os.path.isdir(build_dir):
        os.makedirs(build_dir)
    curr_dir = os.getcwd()
    os.chdir(build_dir)

    cmakeargs = ("-GXcode " +
                "-DCMAKE_BUILD_TYPE=%s " +
                "-DCMAKE_TOOLCHAIN_FILE=%s/platforms/ios/cmake/Toolchains/Toolchain-%s_Xcode.cmake " +
                "-DCMAKE_C_FLAGS=\"-Wno-implicit-function-declaration\" " +
                "-DCMAKE_INSTALL_PREFIX=install") % (config, src, target)

    if os.path.isfile(os.path.join(build_dir, "CMakeCache.txt")):
        os.system("cmake %s ." % (cmakeargs,))
    else:
        os.system("cmake %s %s" % (cmakeargs, src))

    os.system("xcodebuild IPHONEOS_DEPLOYMENT_TARGET=7.0 -parallelizeTargets ARCHS=%s -jobs 8 -sdk %s -configuration %s -target ALL_BUILD" % (arc, target.lower(), config))
    os.system("xcodebuild IPHONEOS_DEPLOYMENT_TARGET=7.0 ARCHS=%s -sdk %s -configuration %s -target install install" % (arc, target.lower(), config))
    os.chdir(curr_dir)

def construct_framework(src, dst, config):

    # find the list of targets (basically, ["iPhoneOS"])
    targetlist = glob.glob(os.path.join(dst, "build", "*"))
    targetlist = [os.path.basename(t) for t in targetlist]

    # set the current dir to the dst
    currentdir = os.getcwd()
    framework = dst + "/vmf.framework"
    if os.path.isdir(framework):
        shutil.rmtree(framework)
    os.makedirs(framework)
    os.chdir(framework)

    # form the directory tree
    dst_dir = "Versions/A"
    os.makedirs(dst_dir + "/Resources")

    temp_dir = "../build/" + targetlist[0]

    # copy headers
    shutil.copytree(temp_dir + "/install/include/vmf", dst_dir + "/Headers")

    # make universal static lib
    if config == "Release":
        wlist = " ".join(["../build/" + t + "/lib/Release/libvmf.a" for t in targetlist])
    else:
        wlist = " ".join(["../build/" + t + "/lib/Debug/libvmfd.a" for t in targetlist])

    os.system("lipo -create " + wlist + " -o " + dst_dir + "/vmf")

    # copy Info.plist
    shutil.copyfile(temp_dir + "/ios/Info.plist", dst_dir + "/Resources/Info.plist")

    # make symbolic links
    os.symlink("A", "Versions/Current")
    os.symlink("Versions/Current/Headers", "Headers")
    os.symlink("Versions/Current/Resources", "Resources")
    os.symlink("Versions/Current/vmf", "vmf")


def build_vmf_framework(src, dst, config):

    targets = ["iPhoneOS", "iPhoneOS", "iPhoneOS"]
    arc = ["armv7", "armv7s", "arm64"]
    for i in range(len(targets)):
        build_vmf(src, os.path.join(dst, "build"), targets[i], arc[i], config)

    construct_framework(src, dst, config)


if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print "Usage:\n\t./build_ios_framework.py <outputdir> [--debug]\n\n"
        sys.exit(0)
    Config = "Release"
    if len(sys.argv) == 3 and sys.argv[2] == "--debug":
        Config = "Debug"

    build_vmf_framework(os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), "../..")), os.path.abspath(sys.argv[1]), Config)
