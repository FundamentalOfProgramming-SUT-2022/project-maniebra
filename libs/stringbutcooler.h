#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define string char *
#define strEq(a, b) (!strcmp(a, b))

int strstrIdx(string haystack, string needle)
{
    return strstr(haystack, needle) - haystack;
}
