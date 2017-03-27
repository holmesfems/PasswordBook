#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include <readline/history.h>
#include <readline/readline.h>

#include "cmdSeparator.h"
#include "simpleColor.h"

int main()
{
    std::string cmd;
    using_history();
    read_history(NULL);
    int count = 0;
    std::ostringstream os;
    CmdSeparator::CmdSeparator cmdsep;
    while (1) {
        count++;
        char *text;
        os.str("");
        os.clear(std::stringstream::goodbit);
        os << COLOR_MAGENTA << "In [" << count << "]:" << COLOR_NORMAL;
        text = readline(os.str().c_str());
        if (text == NULL) {
            std::cout << std::endl;
            break;
        }
        cmd = text;
        std::cout << COLOR_GREEN << "Out[" << count << "]:" << COLOR_NORMAL;
        if (!(cmdsep.exec(cmd))) {
            free(text);
            break;
        }
        add_history(text);
        free(text);
    };
    return 0;
}
