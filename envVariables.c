#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ENV_VARIABLES 2048

struct envVariable{
    char *key;
    char *value;
};

struct envVariable *envVariableArray;
size_t numberOfEnvVariables = 0;

void initializeEnvVariables() {
    envVariableArray = (struct envVariable *)malloc(sizeof(struct envVariable) * MAX_ENV_VARIABLES);
    if (envVariableArray == NULL) {
        fprintf(stderr, "Enviornment variables memory allocation failed\n");
        exit(1);
    }
}

void setEnvVariable(char *key, char *value) {
    for (int envVariableIndex = 0; envVariableIndex < numberOfEnvVariables; envVariableIndex++) {
        if (strcmp(envVariableArray[envVariableIndex].key, key) == 0) {
            envVariableArray[envVariableIndex].value = value;
            return;
        }
    }
    envVariableArray[numberOfEnvVariables].key = key;
    envVariableArray[numberOfEnvVariables].value = value;
    numberOfEnvVariables++;
    fprintf(stderr, "Print all env variables\n");
    /*for (int envVariableIndex = 0; envVariableIndex < numberOfEnvVariables; envVariableIndex++) {
        fprintf(stderr, "Key: %s, Value: %s\n", envVariableArray[envVariableIndex].key, envVariableArray[envVariableIndex].value);
    } */
}

char* getEnvVariable(char *key){
    for (int envVariableIndex = 0; envVariableIndex < numberOfEnvVariables; envVariableIndex++) {
        if (strcmp(envVariableArray[envVariableIndex].key, key) == 0) {
            return envVariableArray[envVariableIndex].value;
        }
    }
    fprintf(stderr, "Key not found\n");
    return NULL;
}