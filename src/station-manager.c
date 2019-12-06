#include "station-manager.h"

int main(int argc, char** argv) {
    int segmentID = 0;

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
    return 0;
}
