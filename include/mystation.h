#ifndef OS_PROJECT_3_MYSTATION_H
#define OS_PROJECT_3_MYSTATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "files.h"

#define REPORT_TIME "1.0"
#define STATS_TIME "10.0"

typedef struct {
    pid_t id;
    int passengerCount;
    int maxToBoard;
    float parkPeriod;
}bus;

typedef struct station_{
    int ASKsize;
    int PELsize;
    int VORsize;
    int VORwaiting;
    int ASKwaiting;
    int busesMoving;
    sem_t access;
    sem_t busMove;
} station;

int createComptroller(int segmentID);
int createStationManager(int segmentID);

#endif //OS_PROJECT_3_MYSTATION_H
