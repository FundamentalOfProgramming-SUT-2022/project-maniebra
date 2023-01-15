#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define strEq(a, b) (!strcmp(a, b))

void processLine(char **cmargs)
{
    char *baseCmd = cmargs[0];
    if (strEq(baseCmd, "createfile"))
    {
        printf("1");
    }
}