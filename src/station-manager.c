#include "station-manager.h"

int main(int argc, char** argv) {
    int segmentID = 0;
    void* segmentStart;

    if(argc != 3) {
        fprintf(stderr, "Wrong number of command line arguments given.\n");
        return 1;
    }
    if(strcmp(argv[1], "-s") == 0) {    //Get shared memory segment id.
        segmentID = (int)strtol(argv[2], NULL, 10);
    }
    else {
        fprintf(stderr, "Unrecognised type of argument %s.\n", argv[1]);
        return 1;
    }

    printf("Station manager created. Given shared memory segment ID:%d\n", segmentID);

    // Attach shared memory segment.
    segmentStart = (void*) shmat(segmentID, NULL, 0);
    if(*(int*)segmentStart == -1)
        perror("Attachment of shared memory segment failed.\n");

    // Get station at beginning of segment and all 3 bays with pointer arithmetic from start.
    station* Station = (station*) segmentStart;
    bus* ASKBay = (bus*)(segmentStart + sizeof(station));
    bus* PELBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * Station->ASKsize);
    bus* VORBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * (Station->ASKsize + Station->PELsize));


    // Keep getting requests until last bus tells us to stop.
    while(1) {
        if(Station->ASKavailable > 0 || Station->PELavailable > 0)
            sem_post(&Station->ASKcome);
        if(Station->PELavailable > 0)
            sem_post(&Station->PELcome);
        if(Station->VORavailable > 0 || Station->PELavailable > 0)
            sem_post(&Station->VORcome);

        sem_wait(&Station->request);
        if(Station->requestType == OVER)
            break;
        else if(Station->requestType == IN) {
            enum region bayToGo;
            int spotToGo;
            if(entrancePossible(Station, ASKBay, PELBay, VORBay, &bayToGo, &spotToGo) == 0) {
                Station->allowRequest = ALLOW;
                Station->goToSpot = spotToGo;
                Station->goToBay = bayToGo;
                Station->movingIn = 1;
                if(bayToGo == ASK)
                    Station->ASKavailable -= 1;
                if(bayToGo == PEL)
                    Station->PELavailable -= 1;
                if(bayToGo == VOR)
                    Station->VORavailable -= 1;
            }
            else {
                Station->allowRequest = DECLINE;
            }
        }
        else if(Station->requestType == OUT) {
            if(Station->movingOut == 0) {
                Station->allowRequest = ALLOW;
                Station->movingOut = 1;
            }
            else {
                Station->allowRequest = DECLINE;
            }
        }
        sem_post(&Station->awaitAnswer);
    }

    printf("Ran out of buses, time for station manager to go home.\n");

    // Remove shared memory segment.
    int err = shmctl(segmentID, IPC_RMID, 0);
    if(err == -1)
        perror("Removal of shared memory segment failed in station manager.\n");

    return 0;
}

// Check if entrance is possible and set up the bay and spot for bus to go.
// I don't like having 6 parameters, but having them in a struct or something is a bit too much at this point...
int entrancePossible(station* Station, bus* ASKBay, bus* PELBay, bus* VORBay, enum region* bay, int* spot) {
    //TODO: buses can go to different bays if their bay is full.
    if(Station->movingIn == 1)
        return 1;

    if(Station->requestRegion == ASK) {
        for(int i = 0; i < Station->ASKsize; i++) {
            if(ASKBay[i].busID == 0) {
                *bay = ASK;
                *spot = i;
                return 0;
            }
        }
        for(int i = 0; i < Station->PELsize; i++) {
            if(PELBay[i].busID == 0) {
                *bay = PEL;
                *spot = i;
                return 0;
            }
        }
        return 1;
    }
    else if(Station->requestRegion == PEL) {
        for(int i = 0; i < Station->PELsize; i++) {
            if(PELBay[i].busID == 0) {
                *bay = PEL;
                *spot = i;
                return 0;
            }
        }
        return 1;
    }
    else if(Station->requestRegion == VOR) {
        for(int i = 0; i < Station->VORsize; i++) {
            if(VORBay[i].busID == 0) {
                *bay = VOR;
                *spot = i;
                return 0;
            }
        }
        for(int i = 0; i < Station->PELsize; i++) {
            if(PELBay[i].busID == 0) {
                *bay = PEL;
                *spot = i;
                return 0;
            }
        }
        return 1;
    }
    else {
        fprintf(stderr, "Unrecognised destination region during lookup.\n");
        return 1;
    }
}
