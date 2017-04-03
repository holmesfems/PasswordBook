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

interactive shell: pbook, use *help* to get available commands

Example:

Plan by jerryjia:
```
opendb test.db
addpasswd test.db google.com
(then input PWD0 PWDOUT, will save your password)
loadpass test.db google.com
(then input PWD0, will show your password)
exit
```
(The command line interface is a little bit ugly... Orz)

Plan by holmesfems:

While you open this,you will be asked for main password PWD0 for once.
There is no alart while you put in a wrong password,but may cause wrong
result to read your encrypted text in pbook database.To define which database
to read,just change the configuation file.

To add an item to pbook just type this:
```
add domain="mail.google.com"

```
or
```
add "mail.google.com"
```
Then it will ask your password to save,type twice then it will be saved.
or just type:
```
add
```
Then it will ask the domain and password,just type what it asks.

To change the value of an item,you can use `add` command and the value of
specific item will be changed as your type in.

To look the item contained in db,you can just type this:
```
list
```
It will show the id and domain of all item that contained. Or type:
```
list "google"
```
or
```
list domain="google"
```
Then will show the item that cotains specific domain.
Note that it doesn't and won't support searching
by your saved password.

The index of the item helps you to delete a certain item as below:
```
del 0
```
or
```
del id=0
```
or
```
del id=0,1,2,3
```

If you are hard of deciding a strong password of your account,
you can use the built-in pseudo-random (mt19937) based password generator.
Type
```
gen
```
Then you can get a random series of charactor. You can also generate by spacifing
some variables,like as below:
```
gen pwdLength=12 pwdList="abcdABCD1234!@#$ "'.,"
```
or
```
genSet pwdLength=12 pwdList="abcdABCD1234!@#$ "'.,"
gen
```
or
```
genSet pwdList=sa+ba+nu+sg
gen
```
sa=Small Alphabet,ba=Big Alphabet,nu=Numeric,sg=Signature.


To exit this application,type as below:
```
exit
```

# Changelog

# Licence

Copyright (c) 2017 holmesfems jerryjia

Released under MIT license

(http://opensource.org/licenses/mit-license.php)

# Author

[holmesfems](https://github.com/holmesfems)

[jerryjia](https://github.com/jerryjiahaha)
