//      written and tested on Windows 11    //

//  self-created libs
#include "libs/argParser.h"
#include "libs/lineprc.h"
#include "libs/ezio.h"
#include "libs/stringbutcooler.h"

int main()
{
    string line = input();
    string *cmargs = getArg(line);
    processLine(cmargs);
}