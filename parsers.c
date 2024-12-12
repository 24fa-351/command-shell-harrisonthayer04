#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool checkForDollarSign(char* token) {
    bool dollarSignPresent = false;
    if (token[0] == '$') {
        return true;
    }
    return dollarSignPresent;
}

bool checkForGreaterThanSign(char** tokenizedCommandOutput) {
    bool greaterThanSignPresent = false;
    for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
        if (strcmp(tokenizedCommandOutput[i], ">") == 0) {
            return true;
        }
    }
    return greaterThanSignPresent;
}
int findGreaterThanSignIndex(char** tokenizedCommandOutput) {
    int greaterThanSignIndex = -1;
    for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
        if (strcmp(tokenizedCommandOutput[i], ">") == 0) {
            return i;
        }
    }
    return greaterThanSignIndex;
}

bool checkForLessThanSign(char** tokenizedCommandOutput) {
    bool lessThanSignPresent = false;
    for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
        if (strcmp(tokenizedCommandOutput[i], "<") == 0) {
            return true;
        }
    }
    return lessThanSignPresent;
}

int findLessThanSignIndex(char** tokenizedCommandOutput) {
    int lessThanSignIndex = -1;
    for (int i = 0; tokenizedCommandOutput[i] != NULL; i++) {
        if (strcmp(tokenizedCommandOutput[i], "<") == 0) {
            return i;
        }
    }
    return lessThanSignIndex;
}