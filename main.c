//      written and tested on Windows 11    //

//  self-created libs
#include "libs/argParser.h"
#include "libs/lineprc.h"
#include "libs/ezio.h"
#include "libs/stringbutcooler.h"

int main()
{
    while (true)
    {
        CreateFolder(rootFolder);
        string line = input();
        if (strstr(line, "=D") == NULL)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line = substr(line, 0, strlen(line) - 1);
            }
            if (strEq(line, "exit"))
            {
                return 0;
            }
            string *cmargs = getArg(line);
            string response = malloc(512);
            response = processLine(cmargs);
            puts(response);
        }
    }
}