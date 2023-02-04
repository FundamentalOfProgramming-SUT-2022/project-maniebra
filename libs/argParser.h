#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **getArg(char *inputLine)
{
    char **listOfArgs;
    listOfArgs = (char **)malloc(1024 * sizeof(char *));
    int inputSize = strlen(inputLine);
    char *arg = (char *)malloc(inputSize * 32);

    int currIdx = 0;
    int currNumOfArgs = 0;
    int openQs = 0;
    for (int i = 0; i < inputSize; i++)
    {
        char currChar = inputLine[i];
        if (currNumOfArgs == 0)
        {
            if (currChar == ' ')
            {
                *(arg + currIdx) = '\0';
                *(listOfArgs + currNumOfArgs) = arg;
                arg = (char *)malloc(inputSize);
                currNumOfArgs++;
                currIdx = 0;
            }
            else
            {
                *(arg + currIdx) = currChar;
                currIdx++;
            }
        }
        else
        {

            if (currChar == ' ' && (!(openQs % 2)))
            {
                *(arg + currIdx) = '\0';
                *(listOfArgs + currNumOfArgs) = arg;
                arg = (char *)malloc(inputSize);
                currNumOfArgs++;
                openQs = 0;
                currIdx = 0;
            }
            else
            {
                if (currChar == '"' && inputLine[i - 1] != '\\')
                {
                    *(arg + currIdx) = '\"';
                    openQs++;
                }
                else
                {
                    *(arg + currIdx) = currChar;
                }

                currIdx++;
            }
        }
    }
    if (strlen(arg) > 0)
    {
        *(arg + currIdx) = '\0';
        *(listOfArgs + currNumOfArgs) = arg;
        arg = (char *)malloc(inputSize);
        currNumOfArgs++;
        currIdx = 0;
    }
    *(listOfArgs + currNumOfArgs) = NULL;
    free(arg);
    return listOfArgs;
}