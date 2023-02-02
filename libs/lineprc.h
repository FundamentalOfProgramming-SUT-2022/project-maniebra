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

int append(char *s, size_t size, char c)
{
    if (strlen(s) + 1 >= size)
    {
        return 1;
    }
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
    return 0;
}
int appendStr(char *s, size_t size, char str[])
{
    int i = 0;
    while (*(str + i) != '\0')
    {
        append(s, size, *(str + i));
        i++;
    }
    return 0;
}
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

char *rootFolder = "C:/Users/Public/Documents/root/";

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

#define strEq(a, b) (!strcmp(a, b))

char *removeQs(char *argVal)
{
    if (argVal[0] == '"')
    {
        strcpy(argVal, substr(argVal, 1, strlen(argVal) - 1));
    }
    return argVal;
}

//
// demanded cmds
//
char *createFile(char fileAddress[])
{
    CreateFolder(fileAddress);
    fileAddress = parsePath(fileAddress);
    FILE *file;
    file = fopen(fileAddress, "w");

    if (file == NULL)
    {

        return "Unable to create file.\n";
    }
    else
    {
        fputs("\0", file);
        static char s[] = "";
        sprintf(s, "File successfully created in %s\n", fileAddress);
        return (s);
    }
}
char *grep(char **files, char *pattern, int options)
{
    static char output[] = "";

    bool printIt = 0,
         count = 0, list = 0;
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
                        strcat(output, *(files + idx));
                        strcat(output, " : ");

                        strcat(output, line);
                        if (line[strlen(line) - 1] != '\n')
                        {
                            strcat(output, "\n");
                        }
                    }
                    else if (list)
                    {
                        strcat(output, *(files + idx));
                        strcat(output, "\n");

                        break;
                    }
                    counter++;
                }
            }
        }
        else
        {
            strcat(output, "Error! File ");
            strcat(output, *(files + idx));
            strcat(output, "has not been found!\n");
            strcat(output, "\n");
        }

        idx++;
    }
    if (count)
    {
        char lli[] = "";
        sprintf(lli, "%i", counter);
        strcat(output, lli);
        strcat(output, "\n");
    }

    return output;
}

char *insertstr(char *fileName, char *toBeInserted, int lineNo, int pos)
{
    if (!checkFileValidity(fileName))
        return "EROR: file not found!";

    FILE *fileToEdit = fopen(fileName, "r");

    char str[2048] = "\0";
    int i = 0;
    while (toBeInserted[i] != '\0')
    {

        while (toBeInserted[i] == '\\')
        {

            if (toBeInserted[i + 1] == 'n')
            {
                strcat(str, "\n\0");
            }

            else if (toBeInserted[i + 1] == '\\')
            {
                strcat(str, "\\\0");
            }

            else if (toBeInserted[i + 1] == '\"')
            {
                strcat(str, "\"\0");
            }

            i += 2;
        }
        i++;

        if (toBeInserted[i] == '\0')
        {
            break;
        }
        char c[2] = {toBeInserted[i], '\0'};
        strcat(str, c);
    }

    char file[4096];
    file[0] = '\0';
    for (int i = 1; i < lineNo; i++)
    {
        char thisLine[2048];
        if (fgets(thisLine, 2048, fileToEdit) == NULL)
        {
            strcat(file, "\n");
        }
        else
        {
            strcat(file, thisLine);
        }
    }

    char thisLine[2048];
    if (fgets(thisLine, 2048, fileToEdit) == NULL)
    {
        for (int i = 1; i <= pos; ++i)
        {
            strcat(file, " ");
        }

        strcat(file, str);
        fclose(fileToEdit);

        fileToEdit = fopen(fileName, "w");
        fputs(file, fileToEdit);
        fclose(fileToEdit);

        return "SUCCESS: sentence inserted!";
    }

    int length = strlen(thisLine) - 1;
    if (length < pos)
    {
        strncat(file, thisLine, length);
        for (int i = 1; i <= pos - length; i++)
        {
            strcat(file, " ");
        }
        strcat(file, str);
        strcat(file, "\n");
    }

    else
    {
        strncat(file, thisLine, pos);
        strcat(file, str);
        strcat(file, thisLine + pos);
    }

    for (int i = 1;; i++)
    {
        char restOfText[2048];
        if (fgets(restOfText, 2048, fileToEdit) == NULL)
        {
            break;
        }
        strcat(file, restOfText);
    }

    fclose(fileToEdit);

    fileToEdit = fopen(fileName, "w");
    fputs(file, fileToEdit);
    fclose(fileToEdit);

    return "SUCCESS: sentence inserted!";
}

//
//

char *processLine(char **cmargs)
{
    char *baseCmd = cmargs[0];
    if (strEq(baseCmd, "createfile"))
    {
        char fileAddress[] = "";
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
                strcpy(fileAddress, argVal);
            }
        }
        return (createFile(fileAddress));
    }
    else if (strEq(baseCmd, "insertstr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        char *str = malloc(128);
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            else if (strEq(argKey, "-str"))
            {
                strcpy(str, argVal);
            }
            else if (strEq(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        return insertstr(address, str, lineNo, pos);
    }
    else if (strEq(baseCmd, "cat"))
    {
        // needed
        char *address = malloc(128);
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
        }
    }
    else if (strEq(baseCmd, "removestr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        char fOrB = 'a';
        //
        int i = 1;

        for (i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            else if (strEq(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        if (strEq(cmargs[i], "-b"))
        {
            fOrB = 'b';
        }
        if (strEq(cmargs[i], "-f"))
        {
            fOrB = 'f';
        }
    }
    else if (strEq(baseCmd, "copystr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0, size = 0;
        ;
        char fOrB = 'a';
        //
        int i = 1;

        for (i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (strEq(argKey, "-size"))
            {
                sscanf(argVal, "%i", &size);
            }
            else if (strEq(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        if (strEq(cmargs[i], "-b"))
        {
            fOrB = 'b';
        }
        if (strEq(cmargs[i], "-f"))
        {
            fOrB = 'f';
        }
    }
    else if (strEq(baseCmd, "cutstr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0, size = 0;
        ;
        char fOrB = 'a';
        //
        int i = 1;

        for (i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (strEq(argKey, "-size"))
            {
                sscanf(argVal, "%i", &size);
            }
            else if (strEq(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
        if (strEq(cmargs[i], "-b"))
        {
            fOrB = 'b';
        }
        if (strEq(cmargs[i], "-f"))
        {
            fOrB = 'f';
        }
    }
    else if (strEq(baseCmd, "pastestr"))
    {
        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            else if (strEq(argKey, "-pos"))
            {
                sscanf(argVal, "%i:%i", &lineNo, &pos);
            }
        }
    }
    else if (strEq(baseCmd, "find"))
    {
        // needed
        char *address = malloc(128);
        char *str = malloc(128);
        int mode = 0;
        int mode2ndArg = -1;
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (strEq(argKey, "-str"))
            {
                strcpy(str, argVal);
            }
        }
        int i = 5;
        while (cmargs[i] != NULL)
        {
            if (strEq(cmargs[i], "-count"))
            {
                mode += 1;
            }
            else if (strEq(cmargs[i], "-at"))
            {
                mode += 10;
                sscanf("%d", cmargs[i + 1], &mode2ndArg);
                i++;
            }
            else if (strEq(cmargs[i], "-all"))
            {
                mode += 100;
            }
            else if (strEq(cmargs[i], "-byword"))
            {
                mode += 1000;
            }

            i++;
        }
    }
    else if (strEq(baseCmd, "replace"))
    {
        // needed
        char *address = malloc(128);
        char *str1 = malloc(128);
        char *str2 = malloc(128);
        int mode = 0;
        int mode2ndArg = -1;
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
            if (strEq(argKey, "-str1"))
            {
                strcpy(str1, argVal);
            }
            if (strEq(argKey, "-str2"))
            {
                strcpy(str2, argVal);
            }
        }
        int i = 5;
        while (cmargs[i] != NULL)
        {

            if (strEq(cmargs[i], "-at"))
            {
                mode += 10;
                sscanf("%d", cmargs[i + 1], &mode2ndArg);
                i++;
            }
            else if (strEq(cmargs[i], "-all"))
            {
                mode += 100;
            }

            i++;
        }
    }
    else if (strEq(baseCmd, "grep"))
    {

        // needed
        char *str = malloc(128);
        int mode = 0;
        char **listOfFiles = (char **)malloc(64 * sizeof(char *));
        //
        int currNumOfFiles = 0;
        if (strEq("-str", cmargs[1]))
        {
            strcpy(str, cmargs[2]);
            int i = 4;
            while (cmargs[i] != NULL)
            {
                *(listOfFiles + currNumOfFiles) = parsePath(removeQs(cmargs[i]));
                currNumOfFiles++;
                i++;
            }
        }
        else
        {
            if (strEq("-c", cmargs[1]))
            {
                mode = 1;
            }
            else if (strEq("-l", cmargs[1]))
            {
                mode = 2;
            }

            strcpy(str, cmargs[3]);
            int i = 5;
            while (cmargs[i] != NULL)
            {
                *(listOfFiles + currNumOfFiles) = parsePath(removeQs(cmargs[i]));
                currNumOfFiles++;
                i++;
            }
        }
        *(listOfFiles + currNumOfFiles) = NULL;
        int j = 0;

        // function used
        return grep(listOfFiles, str, mode);
    }
    else if (strEq(baseCmd, "undo"))
    {
        // needed
        char *address = malloc(128);
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
        }
    }
    else if (strEq(baseCmd, "auto-indent"))
    {
        // needed
        char *address = malloc(128);
        //
        for (int i = 1; cmargs[i] != NULL && cmargs[i + 1] != NULL; i += 2)
        {
            char *argKey = cmargs[i];
            char *argVal = cmargs[i + 1];

            argVal = removeQs(argVal);

            if (strEq(argKey, "-file"))
            {
                strcpy(address, argVal);
            }
        }
    }
    else if (strEq(baseCmd, "compare"))
    {
        // needed
        char *address1 = malloc(128);
        char *address2 = malloc(128);
        //
        for (int i = 1; i < 3; i++)
        {
            char *argVal = cmargs[i];

            if (i == 1)
            {
                strcpy(address1, argVal);
            }
            else
            {
                strcpy(address2, argVal);
            }
        }
    }
    else if (strEq(baseCmd, "tree"))
    {
        // needed
        int depth = 0;
        //
        sscanf(cmargs[1], "%d", &depth);
    }
}