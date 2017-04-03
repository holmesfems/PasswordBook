/*
 * miscUtil.h
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MISCUTIL_H
#define MISCUTIL_H

#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <algorithm>
#include <iostream>

//! \ref
// http://stackoverflow.com/questions/1413445/reading-a-password-from-stdcin
/*! \brief hide/show input characters (useful for password input)
 */
void SetStdinEcho(bool enable = true);

// class T should be iterable (has 'push_back()', 'begin()', 'end()')
template <typename T>
T ReadPassword(char stop = '\n') {
  SetStdinEcho(false);
  T input;
  char c = getchar();
  while (c != stop) {
    input.push_back(c);
    c = getchar();
    std::cout << "**";
  }
  SetStdinEcho(true);
#ifdef DEBUG
  std::cout << std::endl << "you have input: ";
  std::for_each(input.begin(), input.end(), putchar);
  std::cout << std::endl;
#endif
  return input;
}

#endif /* !MISCUTIL_H */

/* vim: set et: */
