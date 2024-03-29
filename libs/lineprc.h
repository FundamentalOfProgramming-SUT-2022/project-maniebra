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
char *targetFolder(char *address)
{

    int end = strlen(address);
    while (address[end] != '/' && address[end] != '\\')
    {
        end--;
    }
    address = substr(address, 0, end + 1);
    // address = parsePath(address);
    char *output = calloc(1, 256);
    if (address[1] != ':' && (address[2] != '/' || address[2] != '\\'))
    {
        strcat(output, rootFolder);
    }
    strcat(output, address);
    return output;
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
int strstrIdx(char *haystack, char *needle)
{
    return strstr(haystack, needle) - haystack;
}
//
// demanded cmds
//
char *createFile(char fileAddress[])
{
    CreateFolder(targetFolder(fileAddress));
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
    char *output = malloc(4096);
    output[0] = 0;

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
    while (files[idx])
    {

        bool valid = checkFileValidity(files[idx]);
        if (valid)
        {
            FILE *curr;

            curr = fopen(files[idx], "a+");

            char *line = malloc(512);
            line[0] = 0;
            while (fgets(line, 50, curr))
            {

                if (strstr(line, removeQs(pattern)) != NULL)
                {
                    if (printIt)
                    {
                        strcat(output, files[idx]);
                        strcat(output, " : ");

                        strcat(output, line);
                        if (line[strlen(line) - 1] != '\n')
                        {
                            strcat(output, "\n");
                        }
                    }
                    else if (list)
                    {
                        strcat(output, files[idx]);
                        strcat(output, "\n");

                        break;
                    }
                    counter++;
                }
            }
        }
        else
        {
            strcat(output, "Error! File \0");
            strcat(output, *(files + idx));
            strcat(output, " has not been found!\n\0");
        }

        idx++;
    }
    if (count)
    {
        char *lli = malloc(512);
        sprintf(lli, "%i", counter);
        strcat(output, lli);
        strcat(output, "\n");
    }

    return output;
}

char *insertstr(char *fileName, char *toBeInserted, int lineNo, int pos)
{
    lineNo--;
    pos--;
    if (!checkFileValidity(fileName))
        return "ERROR! File has not been found!";

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

        if (toBeInserted[i] == '\0')
        {
            break;
        }
        char c[2] = {toBeInserted[i], '\0'};
        strcat(str, c);
        i++;
    }

    char file[4096];
    file[0] = '\0';
    for (int i = 0; i < lineNo; i++)
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

        return "String successfully inserted!";
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

    return "String successfully inserted!";
}

char *removestr(char *address, int lineNo, int pos, int size, char bOrF)
{
    if (!checkFileValidity(address))
    {
        return "ERROR! File has not been found!";
    }
    FILE *file;
    file = fopen(address, "r");
    char *res = (char *)malloc(256);
    res[0] = 0;
    int currLine = 1;
    int bufferLength = 255;
    char buffer[256];
    while (fgets(buffer, bufferLength, file))
    {
        if (currLine != lineNo)
        {
            strcat(res, buffer);
        }
        else
        {
            int idx = 0;
            while (buffer[idx] != '\0')
            {
                char c[2] = {buffer[idx], '\0'};
                if (idx < pos - 1 || idx > pos - 2 + size)
                {
                    strcat(res, c);
                }
                idx++;
            }
        }
        currLine++;
    }

    fclose(file);
    file = fopen(address, "w");
    fputs(res, file);
    fclose(file);
    return "Characters successfully removed!";
}
char *cat(char *address)
{
    FILE *file;
    char line[256];

    if (!checkFileValidity(address))
        return "ERROR! File does not exist!";
    file = fopen(address, "r");
    if (file == NULL)
    {
        return ("ERROR! File cannot be opened!");
    }

    char *res = malloc(256);
    res[0] = 0;
    while (fgets(line, 256, file))
    {
        strcat(res, line);
    }

    fclose(file);

    return res;
}
char *copystr(char *address, int lineNo, int pos, int size, char bOrF)
{
    FILE *file = fopen(address, "r");
    char line[256];
    if (!checkFileValidity(address))
        return "ERROR! File does not exist!";
    char *res = malloc(256);
    res[0] = 0;
    int currLine = 1;
    while (fgets(line, 256, file))
    {
        if (lineNo == currLine)
        {
            if (bOrF == 'f')
            {
                for (int i = pos - 1; i < size + pos - 1; i++)
                {
                    char c[2] = {line[i]};
                    strcat(res, c);
                }
            }
            else
            {
                for (int i = pos - size; i < pos - 1; i++)
                {
                    char c[2] = {line[i]};
                    strcat(res, c);
                }
            }
        }
        currLine++;
    }
    const char *output = res;
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return "Text successfully copied to clipboard!";
}
char *cutstr(char *address, int lineNo, int pos, int size, char bOrF)
{
    if (!checkFileValidity(address))
        return "ERROR! File does not exist!";
    copystr(address, lineNo, pos, size, bOrF);
    removestr(address, lineNo, pos, size, bOrF);
    return "Text was successfully cut to clipboard!";
}
char *pastestr(char *address, int lineNo, int pos)
{
    if (!checkFileValidity(address))
        return "ERROR! File does not exist!";
    char *output = malloc(32);
    output[0] = 0;
    OpenClipboard(0);
    output = GetClipboardData(CF_TEXT);
    CloseClipboard();
    insertstr(address, output, lineNo, pos);
    return "Text successfully pasted!";
}
char *compare(char *address1, char *address2)
{
}

char *find(char *address, char *str, int byword, int at, int count, int all)
{
    // printf("%i %i %i %i", byword, at, count, all);
    if (byword)
    {
        FILE *file = fopen(address, "r");
        char *line = malloc(256);
        int allLen = 0;
        int listOfPoses[1024] = {0};
        int cnt = 0;
        while (fgets(line, 256, file))

        {
            char *token = strtok(line, " ");

            while (token != NULL)
            {
                if (strEq(token, str))
                {
                    listOfPoses[cnt++] = allLen + 1;
                }
                allLen++;

                token = strtok(NULL, " ");
            }
        }
        if (count)
        {
            char *output = calloc(128, 1);
            sprintf(output, "%i", cnt);
            return output;
        }
        if (cnt <= 0)
        {
            return "No results!";
        }

        else if (at)
        {
            if (cnt < at)
            {
                return "ERROR! not so many occurences found!";
            }
            char *output = calloc(128, 1);
            sprintf(output, "%i", listOfPoses[at - 1]);
            return output;
        }
        else if (all)
        {
            char *output = calloc(128 * 16, 1);
            char *temp = calloc(128, 1);
            sprintf(temp, "%i", listOfPoses[0]);
            strcat(output, temp);
            for (int i = 1; i < cnt; i++)
            {
                sprintf(temp, ", %i", listOfPoses[i]);
                strcat(output, temp);
            }
            return output;
        }
        else
        {
            char *output = calloc(128, 1);
            sprintf(output, "%i", listOfPoses[0]);
            return output;
        }
    }
    if (!byword)
    {
        char *data = cat(address);
        int strSize = strlen(str);
        int allLen = 0;
        int listOfPoses[1024] = {0};
        int cnt = 0;
        int firstFound = 0;
        if (!strstrIdx(data, str))
        {
            firstFound = 1;
            cnt++;
            listOfPoses[0] = 0;
        }
        char *token = strtok(data, str);

        while (token != NULL)
        {
            if (!((token[0] == '\n' || token[0] == ' ') && strlen(token) == 1))
            {
                // printf("**{%s}**\n", token);
                allLen += strlen(token);
                listOfPoses[cnt++] = allLen + 1;
                allLen += strSize;
            }
            token = strtok(NULL, str);
        }
        // cnt--;
        if (firstFound && cnt != 1)
            cnt--;
        if (cnt > 1)
            cnt--;
        if (count)
        {
            char *output = calloc(128, 1);
            sprintf(output, "%i", cnt);
            return output;
        }
        else if (cnt <= 0)
        {
            return "No results!";
        }
        else if (at)
        {
            if (cnt < at)
            {
                return "ERROR! not so many occurences found!";
            }
            char *output = calloc(128, 1);
            sprintf(output, "%i", listOfPoses[at - 1] + 1);
            return output;
        }

        else if (all)
        {
            char *output = calloc(128 * 16, 1);
            char *temp = calloc(128, 1);
            sprintf(temp, "%i", listOfPoses[0] + 1);
            strcat(output, temp);
            for (int i = 1; i < cnt; i++)
            {
                sprintf(temp, ", %i", listOfPoses[i] + 1);
                strcat(output, temp);
            }
            return output;
        }
        else
        {
            char *output = calloc(128, 1);
            sprintf(output, "%i", listOfPoses[0] + 1);
            return output;
        }
    }
}

char *replace(char *address, char *str1, char *str2, int at, int all)
{
    int size = strlen(str1);
    if (at)
    {
        int charPos = atoi(find(address, str1, 0, at, 0, 0));
        charPos--;
        printf("%i", charPos);
        char *data = calloc(8192, 1);
        strcpy(data, cat(address));

        int line = 1;
        int pos = 1;
        int cntr = 0;
        int i = 0;
        while (cntr < charPos)
        {
            pos++;
            if (data[i] == '\n')
            {

                line++;
                pos = 1;
                cntr -= 2;
            }
            i++;
            cntr++;
        }
        removestr(address, line, pos, size, 'f');
        insertstr(address, str2, line, pos);
        if (!all)
            return "Replace succeed!";
    }
    if (!strEq(find(address, str1, 0, 0, 0, 0), "No results!"))
    {
        replace(address, str1, str2, 0, 1);
    }
    return "All replaced!";
}

char *autoIndent(char *address)
{
}

//
//

char *
processLine(char **cmargs)
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
        return insertstr(parsePath(address), str, lineNo, pos);
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
        return cat(parsePath(address));
    }
    else if (strEq(baseCmd, "removestr"))
    {

        // needed
        char *address = malloc(128);
        int lineNo = 0, pos = 0;
        long long size = 0;
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
            else if (strEq(argKey, "-size"))
            {
                size = atoll(argVal);
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
        return removestr(parsePath(address), lineNo, pos, size, fOrB);
    }
    else if (strEq(baseCmd, "copystr"))
    {
        // needed
        char *address = malloc(256);
        int lineNo = 0, pos = 0;
        long long size = 0;
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
            else if (strEq(argKey, "-size"))
            {
                size = atoll(argVal);
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

        return copystr(parsePath(address), lineNo, pos, size, fOrB);
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
        return cutstr(parsePath(address), lineNo, pos, size, fOrB);
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
        return (pastestr(parsePath(address), lineNo, pos));
    }
    else if (strEq(baseCmd, "find"))
    {
        // needed
        char *address = malloc(128);
        char *str = malloc(128);
        int byword = 0, at = 0, count = 0, all = 0;

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
            // printf("-{%s}-\n", cmargs[i]);
            if (strEq(cmargs[i], "-count"))
            {
                count = 1;
            }
            if (strEq(cmargs[i], "-at"))
            {
                sscanf(cmargs[i + 1], "%d", &at);
                i++;
            }
            if (strEq(cmargs[i], "-all"))
            {
                all = 1;
            }
            if (strEq(cmargs[i], "-byword"))
            {
                byword = 1;
            }

            i++;
        }
        return find(parsePath(address), str, byword, at, count, all);
    }
    else if (strEq(baseCmd, "replace"))
    {
        // needed
        char *address = malloc(128);
        char *str1 = malloc(128);
        char *str2 = malloc(128);
        int at = 0, all = 0;
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
                sscanf(cmargs[i + 1], "%d", &at);
                i++;
            }
            else if (strEq(cmargs[i], "-all"))
            {
                all = 1;
            }

            i++;
        }
        return replace(parsePath(address), str1, str2, at, all);
    }
    else if (strEq(baseCmd, "grep"))
    {

        // needed
        char *str = malloc(128);
        int mode = 0;
        char **listOfFiles = malloc(256 * 16);
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
        autoIndent(parsePath(address));
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
        return compare(parsePath(address1), parsePath(address2));
    }
    else if (strEq(baseCmd, "tree"))
    {
        // needed
        int depth = 0;
        //
        sscanf(cmargs[1], "%d", &depth);
        // tree(rootFolder, 0, depth);
    }
}