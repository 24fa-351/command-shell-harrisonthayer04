#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "envVariables.h"
#include "tokenizer.h"

#define INPUT_BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    initializeEnvVariables();
    char inputBuffer[INPUT_BUFFER_SIZE];
    char **tokenizedCommandOutput;
    while (1) {
        printf("xsh# ");
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            fprintf(stdout, "You entered: %s\n", inputBuffer);

        } else {
            fprintf(stdout, "Try again");
        }
        if (strcmp(inputBuffer, "quit\n") == 0) {
            break;
        } else if (strcmp(inputBuffer, "exit\n") == 0) {
            break;
        }
        tokenizedCommandOutput = tokenize(inputBuffer, " \t\n");
        /* for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
            fprintf(stderr, "Token %d: %s\n", i, tokenizedCommandOutput[i]);
        } */

        if (strcmp(tokenizedCommandOutput[0], "cd") == 0) {
            // fprintf(stdout, "Change directory\n");
            char *newPath = tokenizedCommandOutput[1];
            chdir(newPath);
        }

        if (strcmp(tokenizedCommandOutput[0], "pwd") == 0) {
            // fprintf(stdout, "Print working directory\n");
            char *currentDirectory = malloc(sizeof(char) * 2048);
            getcwd(currentDirectory, 2048);
            fprintf(stdout, "%s\n", currentDirectory);
            free(currentDirectory);
        }

        if (strcmp(tokenizedCommandOutput[0], "ls") == 0) {
            char *currentDirectory = malloc(sizeof(char) * 2048);
            getcwd(currentDirectory, 2048);
            DIR *dir = opendir(currentDirectory);

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                printf("%s\n", entry->d_name);
            }

            closedir(dir);
            free(currentDirectory);
        }

        if (strcmp(tokenizedCommandOutput[0], "echo") == 0) {
            for (int i = 1; tokenizedCommandOutput[i] != NULL; i++) {
                fprintf(stdout, "%s ", tokenizedCommandOutput[i]);
            }
            fprintf(stdout, "\n");
        }
        if (strcmp(tokenizedCommandOutput[0], "set") == 0) {
            setEnvVariable(tokenizedCommandOutput[1],
                           tokenizedCommandOutput[2]);
        }

        memset(inputBuffer, 0, sizeof(inputBuffer));
    }
    free(envVariableArray);
    return 0;
}