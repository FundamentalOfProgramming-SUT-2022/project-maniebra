#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **getArg(char *inputLine)
{
    char **listOfArgs;
    listOfArgs = (char **)malloc(16 * sizeof(char *));
    int inputSize = strlen(inputLine);
    char *arg = (char *)malloc(inputSize);

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
            if (currNumOfArgs % 2)
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
                if (currChar == '-' && !(openQs % 2))
                {
                    *(arg + currIdx) = '\0';
                    *(listOfArgs + currNumOfArgs) = arg;
                    arg = (char *)malloc(inputSize);
                    currNumOfArgs++;
                    currIdx = 0;
                    i--;
                    openQs = 0;
                }
                else
                {
                    if (currChar == '"')
                    {
                        openQs++;
                    }
                    *(arg + currIdx) = currChar;
                    currIdx++;
                }
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
    free(arg);
    return listOfArgs;
}