#ifndef OS_PROJECT_3_FILES_H
#define OS_PROJECT_3_FILES_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "mystation.h"

typedef struct station_ station;    //Two header files need each other, this solves any issues.

bool    checkConfigFileExists(char* filename);
int     initializeOutputFile(char* filename);
int     readConfigFile(station* Station, char* configFile);

#endif //OS_PROJECT_3_FILES_H
