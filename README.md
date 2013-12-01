curlP
=====

curlP is a personal c++ project based on the libcurl and boost libraries to be able to download many chunks of a a file in separated threads.


Dependencies
============

libcurl - http://curl.haxx.se/libcurl/
cpp-netlib - http://cpp-netlib.org/


Init submodules
===============

```
git submodule update --init
```

Build bundled dependencies
==========================

### CPP-NETLIB BUILD
```
cd lib/cpp-netlib
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .
make -C libs/network/src && cd ../..
```


Build project
=============
```
scons
./bin/curlP
```
