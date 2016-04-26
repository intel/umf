# VMF-3 demo application

## Overview
This directory contains a demo app for VMF 3.0 release.
It consists of 3 parts:
1. **Android** part: RTSP server application streaming live camera video
   and VMF metadata (GPS locations) from Android 4+ phone
2. **iOS** part: RTSP server application streaming live camera video
   and VMF metadata (GPS locations) from iOS 7+ iPhone
3. **desktop** part: Qt5-based UI player of RTSP video stream with VMF metadata

This demo isn't built as a part of the whole VMF source tree (like samples do).
Instead each part has its own build instructions inside README.md file in its directory.

## Android part
The Android part application is developed with Eclipse + ADT IDE using Android SDK.
It uses VMF via Java API.<br>
The application uses a 3rd-party Android library (GPL) for RTSP video streaming implementation and in parallel with video streams GPS locations in VMF metadata form via TCP socket.<br>
Touching the screen shows options that include RTSP port (1234 by default),
VMF port (4321 by default), text format for metadata serialization (JSON or XML),
GPS emulation option (real GPS may be unavailable in a building), use compression option and use encryption option.
All the options except RTSP port take effect on the next connection session.

## iOS part
The iOS part application is developed with XCode IDE on MacOS system.
It has similar capabilities as the Android part above but less settings to change
(due to higher complexity of settings screen development for iOS).
It also uses 3rd-party code (with non-restrictive license) for RTSP streaming implementation.

## Desktop part
The desktop part is a Qt5-based application that can play 1 or 2 RTSP video streams, receive
GPS locations via TCP sockets and show the corresponding tracks on Google Maps view.
It also shows some metadata info and statistics (using recently added `vmf::Stat` API).
In particular it shows the metadata serialization format (XML or JSON),
compression algo used (if any), encryption usage (algo and passphrase are pre-defined).
Statistics info includes metadata `count`, `min`, `max` and `last` values of latitude.<br>
The application has the following dependencies:
* Qt 5.6+ (http://download.qt.io/archive/qt/5.6/5.6.0/)
* OpenSSL 1.x used by Google Maps (http://openssl.org)
* VLC player 2.2.1+ used for receiving and showing RTSP video (http://download.videolan.org/pub/videolan/vlc/2.2.2/)
* and of course VMF 3.0
