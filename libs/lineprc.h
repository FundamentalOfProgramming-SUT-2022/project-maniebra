#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>

void CreateFolder(const char *path)
{
    if (!CreateDirectory(path, NULL))
    {
        return;
    }
}

char *rootFolder = "C:/Users/Public/Documents/root1/";

#define strEq(a, b) (!strcmp(a, b))

char *substr(const char *src, int begin, int end)
{
    int len = end - begin;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    for (int i = begin; i < end /* && (*(src + i) != '\0')*/; i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}
void processLine(char **cmargs)
{
    char *baseCmd = cmargs[0];
    if (strEq(baseCmd, "createfile"))
    {
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            if (argVal[0] == '"')
            {
                strcpy(argVal, substr(argVal, 1, strlen(argVal) - 1));
            }

            if (strEq(argKey, "-file"))
            {
                if (argVal[1] != ':' && (argVal[2] != '/' || argVal[2] != '\\'))
                {
                    char address[512];
                    strcpy(address, rootFolder);
                    strcat(address, argVal);
                    strcpy(argVal, address);
                    printf(argVal);
                }
                FILE *file;
                file = fopen(argVal, "w");

                if (file == NULL)
                {

                    printf("Unable to create file.\n");
                }
                else
                {
                    fputs("\0", file);
                    printf("File successfully created in %s\n", argVal);
                }
            }
        }
    }
}