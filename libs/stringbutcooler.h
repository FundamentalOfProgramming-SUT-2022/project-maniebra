#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define string char *
#define strEq(a, b) (!strcmp(a, b))

int strstrIdx(string haystack, string needle)
{
    return strstr(haystack, needle) - haystack;
}
string substr(const string src, int begin, int end)
{
    int len = end - begin;
    string dest = (string)malloc(sizeof(char) * (len + 1));
    for (int i = begin; i < end /* && (*(src + i) != '\0')*/; i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}