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

//! \ref http://stackoverflow.com/questions/1413445/reading-a-password-from-stdcin
void SetStdinEcho(bool enable = true);

#endif /* !MISCUTIL_H */
