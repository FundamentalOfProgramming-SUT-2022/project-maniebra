//      written and tested on Windows 11    //

//  self-created libs
#include "libs/argParser.h"
#include "libs/lineprc.h"
#include "libs/ezio.h"
#include "libs/stringbutcooler.h"

int main()
{
    CreateFolder(rootFolder);
    string line = input();
    string *cmargs = getArg(line);
    string response = malloc(512);
    response = processLine(cmargs);
    puts(response);
}