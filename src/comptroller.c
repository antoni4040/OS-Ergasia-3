#include "comptroller.h"

int main(int argc, char** argv) {
    unsigned int reportInterval = 0;
    int segmentID = 0;
    void* segmentStart;

    if(argc != 5) {
        fprintf(stderr, "Wrong number of command line arguments given.\n");
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-t") == 0) {   //Time interval to print statistics.
            i++;
            reportInterval = (unsigned int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-s") == 0) {     //Shared memory segment id.
            i++;
            segmentID = (int)strtol(argv[i], NULL, 10);
        }
        else {
            fprintf(stderr, "Unrecognised type of argument %s.\n", argv[i]);
            return 1;
        }
    }

    printf("Comptroller created. Given shared memory segment ID:%d\n", segmentID);

    // Attach shared memory segment.
    segmentStart = (void*) shmat(segmentID, NULL, 0);
    if(*(int*)segmentStart == -1)
        perror("Attachment of shared memory segment failed.\n");

    // Get station at beginning of segment.
    station* Station = (station*) segmentStart;

    int cycles = CYCLES_UNTIL_STATS;
    while(Station->busesLeft != NUM_OF_BUSES + NUM_OF_TTY_BUSES) {
        sleep(reportInterval);
        if(cycles == 0) {
            cycles = CYCLES_UNTIL_STATS;
            // Print station statistics.
            float medianStay = (float)Station->totalStayAtStation / (float)Station->busesLeft;
            printf("Comptroller Statistics: Median stay at station %.2f\n"
                   "Passengers arrived: %d\nPassengers left: %d\nBusses serviced: %d\n",
                   medianStay, Station->passengersCome, Station->passengersLeft, Station->busesLeft);
        }
        else {
            cycles--;
            // Print current status of station.
            int busesInStation = (Station->ASKsize - Station->ASKavailable) + \
                (Station->PELsize - Station->PELavailable) + (Station->VORsize - Station->VORavailable);
            printf("Comptroller Status: Number of buses: in station: %d\n"
                   "ASK free spots: %d\tPEL free spots: %d\tVOR free spots: %d\n", busesInStation, Station->ASKavailable,
                   Station->PELavailable, Station->VORavailable);
        }
    }

    // Detach shared memory segment.
    int err = shmdt((void*)segmentStart);
    if(err == -1)
        perror("Detachment of shared memory segment failed.\n");

    return 0;
}