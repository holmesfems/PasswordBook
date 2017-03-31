PasswordBook
============
Remember your password on various sites

# Description

This tool is to remember your passwords on various sites, and keeping your passwords Encrypted.

The encryption method is discussed [here](https://github.com/holmesfems/PasswordBook/blob/master/doc/research/PWD_PLAN.md)

We will use aes-256-cbc and pbkdf2.

# Download

Please download from [Release Page](https://github.com/holmesfems/PasswordBook/releases)

# Support Platform

At present, compilation only tested on [Archlinux](https://www.archlinux.org/) and [Funtoo](http://www.funtoo.org)/[Gentoo](https://gentoo.org/)

# Requirement

* c++11 compatible compiler
* [cmake](https://cmake.org/)>=3.0.2 (for building from source)
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) (to find library)
* [botan-2](https://github.com/randombit/botan) (Encryption support)
* [sqlite3](http://sqlite.org/src/doc/trunk/README.md) (Using database to save password)
* [boost](http://www.boost.org/) (Used to do some operation on files to save some configs)
* [libreadline](https://cnswww.cns.cwru.edu/php/chet/readline/rltop.html) (simple repl/shell support)
* [gtest](https://github.com/google/googletest) (If you want to run tests)
* [doxygen](https://github.com/doxygen/doxygen) (If you want to build doc)

# Installation

(Note that we prefer using clang++ than g++)

```sh
mkdir build && cd build
cmake -D CMAKE_CXX_COMPILER="/usr/bin/clang++" ..
make
make test # If you want tests
make doc # If you want to build doc
sudo make install
```

# Usage

# Changelog

# Licence

Copyright (c) 2017 holmesfems jerryjia

Released under MIT license

(http://opensource.org/licenses/mit-license.php)

# Author

[holmesfems](https://github.com/holmesfems)

[jerryjia](https://github.com/jerryjiahaha)
