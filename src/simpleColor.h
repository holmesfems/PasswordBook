#ifndef _SIMPLECOLOR
#define _SIMPLECOLOR
#if defined(WIN32)||defined(WIN64)
#define COLOR_MAGENTA ""
#define COLOR_NORMAL ""
#define COLOR_RED ""
#define COLOR_GREEN ""
#else
#define COLOR_MAGENTA "\033[35;1m"
#define COLOR_NORMAL "\033[0m"
#define COLOR_RED "\033[31;1m"
#define COLOR_GREEN "\033[32;1m"
#endif
#endif //_SIMPLECOLOR
