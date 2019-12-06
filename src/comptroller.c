#include "comptroller.h"

int main(int argc, char** argv) {
    float reportInterval = 0.0f;
    float statsTime = 0.0f;
    int segmentID = 0;
    void* segmentStart;

    if(argc != 7) {
        fprintf(stderr, "Wrong number of command line arguments given.\n");
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-d") == 0) {    //Time interval to report.
            i++;
            reportInterval = strtof(argv[i], NULL);
        }
        else if(strcmp(argv[i], "-t") == 0) {   //Time interval to print statistics.
            i++;
            statsTime = strtof(argv[i], NULL);
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
    printf("Test: station ASK size %d\n", Station->ASKsize);

    // Remove shared memory segment.
    int err = shmctl(segmentID, IPC_RMID, 0);
    if(err == -1)
        perror("Removal of shared memory segment failed.\n");

    return 0;
}