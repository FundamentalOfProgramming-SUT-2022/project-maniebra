#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <windows.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#ifdef UNIX
#include <unistd.h>
#endif

char *parsePath(char *recievedPath)
{
    if (recievedPath[1] != ':' && (recievedPath[2] != '/' || recievedPath[2] != '\\'))
    {
        char address[512];
        strcpy(address, rootFolder);
        strcat(address, recievedPath);
        strcpy(recievedPath, address);
    }
}
void CreateFolder(const char *path)
{
    if (!CreateDirectory(path, NULL))
    {
        return;
    }
}
bool checkFileValidity(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}
void grep(char **files, char *pattern, int options)
{
    bool printIt = 0, count = 0, list = 0;
    switch (options)
    {
    case (0):
    {
        printIt = 1;
        break;
    }
    case (1):
    {
        count = 1;
        break;
    }
    case (2):
    {
        list = 1;
        break;
    }

    default:
        break;
    }
    int counter = 0;

    int idx = 0;
    while (*(files + idx) != NULL)
    {

        bool valid = checkFileValidity(*(files + idx));
        if (valid)
        {
            FILE *curr;
            curr = fopen(*(files + idx), "a+");
            char line[50];
            while (fgets(line, 50, curr) != NULL)
            {
                if (strstr(line, pattern))
                {
                    if (printIt)
                    {
                        printf("%s : %s", *(files + idx), line);
                    }
                    else if (list)
                    {
                        printf("%s\n", *(files + idx));
                        break;
                    }
                    counter++;
                }
            }
        }
        else
        {
            printf("Error! File %s has not been found!\n", *(files + idx));
        }

        idx++;
    }
    if (count)
    {
        printf("%i\n", counter);
    }
}

char *rootFolder = "C:/Users/Public/Documents/root1/";

#define strEq(a, b) (!strcmp(a, b))

char *substr(const char *src, int begin, int end)
{
    int len = end - begin;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    for (int i = begin; i < end /* && (*(src + i) != '\0')*/
         ;
         i++)
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