#ifndef OS_PROJECT_3_MYSTATION_H
#define OS_PROJECT_3_MYSTATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "files.h"

#define STATS_TIME "10.0"

#define IN 0
#define OUT 1
#define OVER 2

#define ALLOW 0
#define DECLINE 1
#define WAIT_IF_FULL 1

#define NUM_OF_BUSES 20     //Buses to create for this simulation.
#define NUM_OF_TTY_BUSES 0  //Buses that we want to start manually from a terminal.

#define MIN_PARKING_TIME 10
#define RANDOM_PARKING_TIME 5
#define MIN_MAN_TIME 4
#define RANDOM_MAN_TIME 2
#define MIN_PASSENGERS_COME 20
#define RANDOM_PASSENGERS_COME 40
#define MIN_CAPACITY 80
#define RANDOM_CAPACITY 20

enum region {ASK, PEL, VOR};

typedef struct {
    pid_t busID;
    enum region destination;
}bus;

typedef struct station_{
    int ASKsize;                //Parking spots in ASK bay.
    int PELsize;                //Parking spots in PEL bay.
    int VORsize;                //Parking spots in VOR bay.

    int ASKavailable;           //How many parking spots are available in ASK bay.
    int PELavailable;           //How many parking spots are available in PEL bay.
    int VORavailable;           //How many parking spots are available in VOR bay.

    int requestType;            //IN or OUT, depending on what the current bus request is.
    int allowRequest;           //ALLOW or DECLINE.
    enum region requestRegion;  //Region of the bus that is currently requesting.

    enum region goToBay;        //Bay for the bus to go if the IN request is successful.
    int goToSpot;               //Parking spot for the bus to go if the IN request is successful.

    int movingIn;               //How many buses are currently moving in(max 1).
    int movingOut;              //How many buses are currently moving out(max 1).

    sem_t mutex;                //Mutex(sort of) for the shared memory(for buses).
    sem_t request;              //Wake up station manager to serve request.
    sem_t awaitAnswer;          //Await answer from station manager for bus request.
    sem_t waitForIn;            //Wait for previous moving in procedure to finish.
    sem_t waitForOut;           //Wait for previous moving out procedure to finish.
    sem_t ASKcome;              //There's room for ASK buses, let them come.
    sem_t PELcome;              //There's room for PEL buses, let them come.
    sem_t VORcome;              //There's room for VOR buses, let them come.

    int busesLeft;              //Count the buses that have left, to stop the simulation at some point.

    // Statistics here:
    int busesArrived;
    int totalStayAtStation;
    int passengersCome;
    int passengersLeft;
} station;

int     createComptroller(int segmentID);
int     createStationManager(int segmentID);
int     createRandomBus(int segmentID);
void    initializeBays(void* segmentStart);

#endif //OS_PROJECT_3_MYSTATION_H
