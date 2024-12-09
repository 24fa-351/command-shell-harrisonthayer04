#include <stdbool.h>

bool checkForDollarSign(char* token) {
    bool dollarSignPresent = false;
    if (token[0] == '$') {
        return true;
    }
    return dollarSignPresent;
}