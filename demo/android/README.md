How to build

1. build VMF `install` target for all the supported Android ABIs
   (e.g. `x86` and `armeabi-v7a`)
2. copy (merge) `android-lib` dir-s from all builds of step 1
   into the dir containing this file
3. copy the `android-lib/libs` dir into `vmf-rstp-server` dir
   to include these native lib-s into the application APK package
4. clone the https://github.com/fyhertz/libstreaming git repo
   into the dir containing this file (`git clone URL`)
5. apply the `0001-libstreaming-fixes.patch` to it (`git apply < file-name.patch`)
6. create new Eclipse (+ADT) workspace
7. import 'Existing Projects into Workspace':
   * vmf/demo/android/libstreaming
   * vmf/demo/android/android-lib
   * vmf/demo/android/vmf-rtsp-server
   wait Eclipse to resolve and build these projects
8. run `vmf-rstp-server` as Android app on Android 4+ device
