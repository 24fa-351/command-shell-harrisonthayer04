#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "envVariables.h"
#include "parsers.h"

#define INPUT_BUFFER_SIZE 32768

char **insertArrayIntoAnotherArray(char **firstArray, char **secondArray,
                                   int indexOfInsertion) {
    int index = 0;
    while (secondArray[index] != NULL) {
        firstArray[indexOfInsertion] = secondArray[index];
        index++;
        indexOfInsertion++;
    }
    return firstArray;
}

char **tokenize(char *commandBuffer, char *delimiter) {
    char commandBufferCopy[INPUT_BUFFER_SIZE];
    strcpy(commandBufferCopy, commandBuffer);
    char **tokenizedCommand =
        (char **)malloc(sizeof(char *) * INPUT_BUFFER_SIZE);
    if (tokenizedCommand == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    char *token = strtok(commandBufferCopy, " \t\n");
    int numberOfTokens = 0;

    while (token != NULL && numberOfTokens < INPUT_BUFFER_SIZE - 1) {
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
            // fprintf(stderr, "Token having removed the dollar sign: %s\n",
            // token);
            tokenizedCommand[numberOfTokens] = getEnvVariable(token);
        } else {
            strcpy(tokenizedCommand[numberOfTokens], token);
        }
        numberOfTokens++;
        token = strtok(NULL, delimiter);
    }
    tokenizedCommand[numberOfTokens] = NULL;
    /*for (int index = 0; tokenizedCommand[index] != NULL; index++) {
        fprintf(stderr, "Token %d: %s\n", index, tokenizedCommand[index]);
    }*/
    if (!checkForLessThanSign(tokenizedCommand)) {
        fprintf(stderr, "Less than sign not found\n");
        return tokenizedCommand;
    } else {
        fprintf(stderr, "Less than sign found\n");
        int indexOfLessThanSign = findLessThanSignIndex(tokenizedCommand);
        char *nameOfFile = tokenizedCommand[indexOfLessThanSign + 1];
        FILE *file = fopen(nameOfFile, "r");
        char **tokenizedFileContents =
            (char **)malloc(sizeof(char *) * INPUT_BUFFER_SIZE);

        char buffer[INPUT_BUFFER_SIZE];
        int fileTokenCount = 0;
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            char *fileToken = strtok(buffer, " \t\n");
            while (fileToken != NULL && fileTokenCount < INPUT_BUFFER_SIZE) {
                tokenizedFileContents[fileTokenCount] =
                    (char *)malloc(strlen(fileToken) + 1);
                strcpy(tokenizedFileContents[fileTokenCount], fileToken);
                fileTokenCount++;
                fileToken = strtok(NULL, " \t\n");
            }
        }
        tokenizedFileContents[fileTokenCount] = NULL;
        fclose(file);
        tokenizedCommand = insertArrayIntoAnotherArray(
            tokenizedCommand, tokenizedFileContents, indexOfLessThanSign);
        return tokenizedCommand;

        for (int index = indexOfLessThanSign; tokenizedCommand[index] != NULL;
             index++) {
            tokenizedCommand[index] = tokenizedCommand[index + 1];
            if (tokenizedCommand[index + 1] == NULL) {
                tokenizedCommand[index] = NULL;
            }
        }
        return tokenizedCommand;
    }
}