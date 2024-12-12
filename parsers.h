#ifndef PARSERS_H
#define PARSERS_H

bool checkForDollarSign(char* token);
bool checkForGreaterThanSign(char** tokenizedCommandOutput);
int findGreaterThanSignIndex(char** tokenizedCommandOutput);
bool checkForLessThanSign(char** tokenizedCommandOutput);
int findLessThanSignIndex(char** tokenizedCommandOutput);

#endif