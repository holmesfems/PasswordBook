PasswordBook
====
Remember your password on virous sites

# Description

This tool is to remember your passwords on virous sites, and keeping your passwords Encrypted.

# Download

Please download from [Release Page](https://github.com/holmesfems/PasswordBook/releases)

# Requirement

* libreadline
* c++11 (For regex support)
* openssl (Encryption support)
* sqlite (Using database to save your password)
* cmake (optional, if you want to build with cmake)

# Installation

Here are two methods: Calling `make` directly or using [cmake](https://cmake.org/runningcmake/).

Note that we prefer using clang++ than g++

Method 1 - Using 'make':

```sh
make
sudo make install
```

Method 2 - Using cmake:

```sh
mkdir build && cd build
cmake -D CMAKE_CXX_COMPILER="/usr/bin/clang++" ..
make
sudo make install
```

# Usage

# Changelog

# Licence

Copyright (c) 2017 holmesfems

Released under MIT license

(http://opensource.org/licenses/mit-license.php)

# Author

[holmesfems](https://github.com/holmesfems)
