# VMF desktop demo application

This application demonstrates key features of VMF 3.0: compression, encryption, statistics, etc.
The app should be run with the Android and iOS servers to display the data they stream.

## How to build:

Make sure 'VLC' player 2.2.1+ is installed in your system.<br>
Download it from https://www.videolan.org/ or use `apt-get` respectively.

Preparation step for **Windows only**:<br>
Create the `gen-lib.cmd` file with the content below in the 'vmf/demo/desktop/vmf-player/3dparty/QmlVlc/libvlc_wrapper/libvlc-sdk/lib/msvc' directory:
```cmd
set VC2013_DIR=C:\Program Files (x86)\Microsoft Visual Studio 12.0
set VLC_DIR=w:\apps\vlc-2.2.2

call "%VC2013_DIR%\VC\vcvarsall.bat" x86_amd64

echo EXPORTS > libvlc.def
for /f "usebackq tokens=4,* delims=_ " %%i in (`dumpbin /exports "%VLC_DIR%\libvlc.dll"`) do if %%i==libvlc echo %%i_%%j >> libvlc.def
lib /def:libvlc.def /out:libvlc.lib /machine:x64
```
Make sure the `VC2013_DIR` and `VLC_DIR` values are correct, replace `x86_amd64` with `x86` in case of Win-x32 build instead of Win-x64.<br>
Run the script, it should generate correct 'libvlc.lib' for the build.

1. make sure all the git submodules are cloned (`git submodule update --init --recursive`)
2. Open vmf-player.pro in QtCreator, setup the project (use Qt 5.6+) and run the build.
