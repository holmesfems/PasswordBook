PasswordBook
====
Remember your password on various sites

# Description

This tool is to remember your passwords on virous sites, and keeping your passwords Encrypted.

# Download

Please download from [Release Page](https://github.com/holmesfems/PasswordBook/releases)

# Requirement

* cmake (building from source)
* libreadline
* c++11 (For regex support)
* openssl (Encryption support)
* sqlite (Using database to save your password)
* boost (Used to do some operation on files to save some configs)

# Installation

(Note that we prefer using clang++ than g++)

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
