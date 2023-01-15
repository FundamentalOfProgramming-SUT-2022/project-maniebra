//////////////////////
//                  //
//  <-- EZ IO -->   //
//                  //
//////////////////////

#include <stdio.h>
#include <stdlib.h>

#define getInt(_var) scanf("%i", &_var)
#define getFloat(_var) scanf("%f", &_var)
#define getLongLong(_var) scanf("%lli", &_var)

char *input()
{
    char *buffer;
    size_t bufsize = 64;
    size_t characters;
    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL)
    {
        perror("Unable to allocate buffer");
        return "\0";
    }
    characters = getline(&buffer, &bufsize, stdin);
    return buffer;
}
void printLn(char *__line)
{
    printf(__line);
    printf("\n");
    fflush(stdout);
}