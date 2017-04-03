/*
 * miscUtil.cpp
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "miscUtil.h"

void SetStdinEcho(bool enable)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable)
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~(ECHO | ICANON);
    else
        tty.c_lflag |= (ECHO | ICANON);

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}
