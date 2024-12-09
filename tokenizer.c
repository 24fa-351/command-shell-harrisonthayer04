#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsers.h"
#include "envVariables.h"

char **tokenize(char *commandBuffer, char *delimiter) {
    char commandBufferCopy[2048];
    strcpy(commandBufferCopy, commandBuffer);
    char **tokenizedCommand = (char **)malloc(sizeof(char *) * 2048);
    if (tokenizedCommand == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    char *token = strtok(commandBufferCopy, " \t\n");
    int numberOfTokens = 0;

    while (token != NULL && numberOfTokens < 2047) {
        tokenizedCommand[numberOfTokens] = (char *)malloc(strlen(token) + 1);
        if (tokenizedCommand[numberOfTokens] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int indexOfToken = 0; indexOfToken < numberOfTokens;
                 indexOfToken++) {
                free(tokenizedCommand[indexOfToken]);
            }
            free(tokenizedCommand);
            return NULL;
        }
        if (checkForDollarSign(token)) {
            memmove(token, token + 1, strlen(token));
            // fprintf(stderr, "Token having removed the dollar sign: %s\n", token);
            tokenizedCommand[numberOfTokens] = getEnvVariable(token);
        } else {
            strcpy(tokenizedCommand[numberOfTokens], token);
        }
        numberOfTokens++;
        token = strtok(NULL, delimiter);
    }
    tokenizedCommand[numberOfTokens] = NULL;
    /*for (int i = 0; tokenizedCommand[i] != NULL; i++) {
        fprintf(stderr, "Token %d: %s\n", i, tokenizedCommand[i]);
    } */
    return tokenizedCommand;
}