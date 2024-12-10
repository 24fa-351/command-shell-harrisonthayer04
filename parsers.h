#ifndef PARSERS_H
#define PARSERS_H

bool checkForDollarSign(char* token);
bool checkForGreaterThanSign(char** tokenizedCommandOutput);
int findGreaterThanSignIndex(char** tokenizedCommandOutput);

#endif