# VMF desktop demo application

This application demonstrates key features of VMF 3.0: compression, encryption, statistics, etc.
The app should be run with the Android and iOS RTSP server parts to display the data they stream.

## How to build:

Make sure 'VLC' player 2.2.1+ is installed in your system.<br>
Download it from https://www.videolan.org/ or use `apt-get` respectively.

Preparation step for **Windows only**:<br>
Create the `gen-lib.cmd` file with the content below in the 'vmf/demo/desktop/vmf-player/3dparty/QmlVlc/libvlc_wrapper/libvlc-sdk/lib/msvc' directory:
```cmd
set VS2013_DIR=C:\Program Files (x86)\Microsoft Visual Studio 12.0
set VLC_DIR=w:\apps\vlc-2.2.2

call "%VS2013_DIR%\VC\vcvarsall.bat" x86_amd64

echo EXPORTS > libvlc.def
for /f "usebackq tokens=4,* delims=_ " %%i in (`dumpbin /exports "%VLC_DIR%\libvlc.dll"`) do if %%i==libvlc echo %%i_%%j >> libvlc.def
lib /def:libvlc.def /out:libvlc.lib /machine:x64
```
Make sure the `VS2013_DIR` and `VLC_DIR` values are correct, replace `x86_amd64` with `x86` in case of Win-x32 build instead of Win-x64.<br>
Run the script, it should generate correct 'libvlc.lib' for the build.

1. make sure all the git submodules are cloned (`git submodule update --init --recursive`)
2. build VMF library (`install` target)
3. Open `vmf-player.pro` in QtCreator, setup the project (use Qt 5.6+) and update path to VMF includes and libs in the '.pro' file
4. run the build
5. to run the application<br>
   on Windows make sure the following folders are in `PATH`:
 * **Qt 5.6** binary dir (e.g. `C:\Qt\Qt5.6.0\5.6\msvc2013_64\bin`), can be downloaded from http://download.qt.io/archive/qt/5.6/5.6.0/
 * **OpenSSL** 1.0 dir (e.g. `C:\OpenSSL-Win64`), can be downloaded from http://openssl.org
  * **VLC player** 2.2.1+ dir (e.g. `W:\Apps\vlc-2.2.2`), can be downloaded from http://download.videolan.org/pub/videolan/vlc/2.2.2/
 * **VMF 3.0** dll dir (e.g. `W:\Dev\git\VMF\vmf3-build-x64-vc12-dyn\install\x64\vc12\bin`), should be built from sources
