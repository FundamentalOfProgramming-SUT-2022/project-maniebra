
#include <stdio.h>
#include <windows.h>
#include "libs/argParser.h"
#include "libs/ezio.h"
#include "libs/lineprc.h"
#include "libs/stringbutcooler.h"
#include <conio.h>

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}
void textcolor(int color)
{
    static int __BACKGROUND;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            color + (__BACKGROUND << 4));
}

char *load_file(char const *path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char *)malloc((length + 1) * sizeof(char));
        if (buffer)
        {
            fread(buffer, sizeof(char), length, f);
        }
        fclose(f);
    }
    buffer[length] = '\0';

    return buffer;
}
void printStuff(char *path)
{
    system("cls");

    char *toBePrinted = load_file(path);

    char *token = strtok(toBePrinted, "\n");
    int lineCntr = 1;
    while (token != NULL)
    {
        printf("\e[2;47m  %.4i  \e[0m %s\n", lineCntr++, token);
        token = strtok(NULL, "\n");
    }
    for (int i = 0; i < 4; i++)
    {
        printf("\e[1;34m ~ \e\n");
    }
    printf("\e[1;34m ~ \e");
    printf("\r");
    printf("\e[1;47m\n");
    printf("\e[0;44m NORMAL ");
    printf("\e[2;47m  %s   \e[1;47m", path);
    printf("\e[0m\n");
}
char address[256] = "";
void getInput()
{

    // break;
    int c = getch();
    if (c == 58)
    {
        printf("\e[1;37m:\e[0m");
        string command = input();
        if (command[strlen(command) - 1] == '\n')
        {
            command = substr(command, 0, strlen(command) - 1);
        }
        string *guiargs = getArg(command);
        if (strEq(guiargs[0], "open"))
        {
            string path = malloc(1024);
            strcpy(path, guiargs[1]);
            if (path[strlen(path) - 1] == '\n')
            {
                path = substr(path, 0, strlen(path) - 1);
                // puts(path);
            }
            strcpy(address, path);
            printStuff(path);
            getInput();
        }
        else
        {

            if (strEq(command, "exit"))
            {
                exit(1);
                return 0;
            }
            processLine(guiargs);
            printStuff(address);
        }
    }

    if (c == 224)
    {
        c = getch();
        switch (c)
        {
        case 80:
            printf("DOWN\n");
            getInput();
            break;
        case 77:
            printf("RIGHT\n");
            getInput();
            break;
        case 75:
            printf("LEFT\n");
            getInput();

            break;
        case 72:
            printf("UP\n");
            getInput();
            break;

        default:
            break;
        }
    }
    else if (c == 3)
    {
        return 0;
    }
    else if (c == 27)
    {
        printf("\r");
        getInput();
    }
    else
    {
        printf("%i", c);
        getInput();
    }
}

int main()
{

    // printStuff("./main.c");

    getInput();
    return 0;
}