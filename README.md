curlP
=====

curlP is a personal c++ project based on the libcurl and boost libraries to be able to download many chunks of a a file in separated threads.


Dependencies
============

- clang - http://clang.llvm.org
- libcurl - http://curl.haxx.se/libcurl/
- cpp-netlib - http://cpp-netlib.org/


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
CXXFLAGS="-ftemplate-depth=256" cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .
make -C libs/network/src && cd ../..
```


Build project
=============
```
scons
./bin/curlP
```

TODO
====
- Implement a simple stable project
- Remove libcurl or cpp-netlib (both doing the same thing...)
