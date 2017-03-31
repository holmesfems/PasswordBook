PasswordBook
============
Remember your password on various sites

# Description

This tool is to remember your passwords on virous sites, and keeping your passwords Encrypted.
The encryption method is using [Blowfish algorithm](https://www.schneier.com/academic/blowfish/).

# Download

Please download from [Release Page](https://github.com/holmesfems/PasswordBook/releases)

# Requirement

* cmake (building from source)
* pkgconfig (to find library)
* c++11 (For regex support)
* [botan-2](https://github.com/randombit/botan) (Encryption support)
* sqlite (Using database to save your password)
* boost (Used to do some operation on files to save some configs)
* libreadline
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
