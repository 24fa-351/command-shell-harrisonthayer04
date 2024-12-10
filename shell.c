#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "envVariables.h"
#include "parsers.h"
#include "tokenizer.h"

#define INPUT_BUFFER_SIZE 2048

void outputToFile(char *filename, char *outputText) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    FILE *fileStream = fdopen(fd, "w");
    fprintf(fileStream, "%s\n", outputText);
    fclose(fileStream);
}

char *concatenateArrayOfStringsIntoOneString(char *inputStrings[],
                                             int numberOfStrings) {
    size_t totalLength = 0;
    for (int index = 0; index < numberOfStrings; index++) {
        totalLength += strlen(inputStrings[index]);
        totalLength += strlen(" ");
    }
    totalLength += 1;

    char *output = (char *)malloc(totalLength);
    output[0] = '\0';
    for (int index = 0; index < numberOfStrings; index++) {
        strcat(output, inputStrings[index]);
        strcat(output, " ");
    }
    return output;
}

int main(int argc, char *argv[]) {
    char *path_env = getenv("PATH");
    initializeEnvVariables();
    char inputBuffer[INPUT_BUFFER_SIZE];
    char **tokenizedCommandOutput;
    while (1) {
        printf("xsh# ");
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            // fprintf(stdout, "You entered: %s\n", inputBuffer);

        } else {
            fprintf(stdout, "Try again");
        }
        if (strcmp(inputBuffer, "quit\n") == 0) {
            break;
        } else if (strcmp(inputBuffer, "exit\n") == 0) {
            break;
        }
        tokenizedCommandOutput = tokenize(inputBuffer, " \t\n");
        if (checkForGreaterThanSign(tokenizedCommandOutput)) {
            fprintf(stderr, "Greater than sign found in input\n");
        }

        /* for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
            fprintf(stderr, "Token %d: %s\n", i, tokenizedCommandOutput[i]);
        } */

        if (strcmp(tokenizedCommandOutput[0], "cd") == 0) {
            // fprintf(stdout, "Change directory\n");
            char *newPath = tokenizedCommandOutput[1];
            chdir(newPath);
        } else if (strcmp(tokenizedCommandOutput[0], "pwd") == 0) {
            // fprintf(stdout, "Print working directory\n");
            char *currentDirectory = malloc(sizeof(char) * 2048);
            getcwd(currentDirectory, 2048);
            if (checkForGreaterThanSign(tokenizedCommandOutput)) {
                outputToFile(
                    tokenizedCommandOutput
                        [findGreaterThanSignIndex(tokenizedCommandOutput) + 1],
                    currentDirectory);
            } else {
                fprintf(stdout, "%s\n", currentDirectory);
            }

            free(currentDirectory);
        } else if (strcmp(tokenizedCommandOutput[0], "ls") == 0) {
            char *currentDirectory = malloc(sizeof(char) * 2048);
            getcwd(currentDirectory, 2048);
            DIR *dir = opendir(currentDirectory);

            struct dirent *entry;
            if (checkForGreaterThanSign(tokenizedCommandOutput)) {
                int numberOfEntriesInArray = 0;
                char **entriesArray = malloc(sizeof(char *) * 1024);
                while ((entry = readdir(dir)) != NULL) {
                    entriesArray[numberOfEntriesInArray] =
                        strdup(entry->d_name);
                    numberOfEntriesInArray++;
                }
                char *lsOutput = concatenateArrayOfStringsIntoOneString(
                    entriesArray, numberOfEntriesInArray);
                outputToFile(
                    tokenizedCommandOutput
                        [findGreaterThanSignIndex(tokenizedCommandOutput) + 1],
                    lsOutput);
                free(lsOutput);
                for (int index = 0; index < numberOfEntriesInArray; index++) {
                    free(entriesArray[index]);
                }
                free(entriesArray);
            } else {
                while ((entry = readdir(dir)) != NULL) {
                    fprintf(stdout, "%s ", entry->d_name);
                }
                printf("\n");
            }

            closedir(dir);
            free(currentDirectory);
        } else if (strcmp(tokenizedCommandOutput[0], "echo") == 0) {
            for (int i = 1; tokenizedCommandOutput[i] != NULL; i++) {
                fprintf(stdout, "%s ", tokenizedCommandOutput[i]);
            }
            fprintf(stdout, "\n");
        } else if (strcmp(tokenizedCommandOutput[0], "set") == 0) {
            char *key = tokenizedCommandOutput[1];
            // memmove(key, key + 1, strlen(key));
            setEnvVariable(tokenizedCommandOutput[1],
                           tokenizedCommandOutput[2]);
        } else if (strcmp(tokenizedCommandOutput[0], "unset") == 0) {
            deleteEnvVariable(tokenizedCommandOutput[1]);

        } else {
            char *path_copy = strdup(path_env);
            char *dir = strtok(path_copy, ":");
            int commandFound = 0;

            while (dir != NULL) {
                // fprintf(stderr, "at: %i\n", __LINE__);
                char full_path[2048];
                snprintf(full_path, sizeof(full_path), "%s/%s", dir,
                         tokenizedCommandOutput[0]);

                if (access(full_path, X_OK) == 0) {
                    // fprintf(stderr, "Command found: %s at %s\n",
                    // tokenizedCommandOutput[0], full_path);
                    commandFound = 1;
                    pid_t pid = fork();
                    if (pid == 0) {
                        execve(full_path, tokenizedCommandOutput, NULL);
                        perror("execve");
                        exit(EXIT_FAILURE);
                    } else {
                        int status;
                        waitpid(pid, &status, 0);
                    }
                    break;
                }
                dir = strtok(NULL, ":");
            }
            if (!commandFound) {
                fprintf(stderr, "Command not found: %s\n",
                        tokenizedCommandOutput[0]);
            }

            free(path_copy);
        }
        memset(inputBuffer, 0, sizeof(inputBuffer));
    }
    free(envVariableArray);
    return 0;
}