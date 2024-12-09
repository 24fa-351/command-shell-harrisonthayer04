#ifndef ENVVARIABLES_H
#define ENVVARIABLES_H

extern struct envVariable *envVariableArray;
extern size_t numberOfEnvVariables;

void initializeEnvVariables();
void setEnvVariable(char *key, char *value);
char* getEnvVariable(char *key);

#endif