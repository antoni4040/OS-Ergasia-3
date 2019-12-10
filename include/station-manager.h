#ifndef OS_PROJECT_3_STATION_MANAGER_H
#define OS_PROJECT_3_STATION_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mystation.h"
#include "bus.h"

int entrancePossible(station* Station, bus* ASKBay, bus* PELBay, bus* VORBay, enum region* bay, int* spot);
#endif //OS_PROJECT_3_STATION_MANAGER_H
