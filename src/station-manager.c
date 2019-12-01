#include "station-manager.h"

int main(int argc, char** argv) {
    int shmid = 0;

    if(argc != 3) {
        fprintf(stderr, "Wrong number of command line arguments given.\n");
        return 1;
    }
    if(strcmp(argv[1], "-s") == 0) {    //Get shared memory segment id.
        shmid = (int)strtol(argv[2], NULL, 10);
    }
    else {
        fprintf(stderr, "Unrecognised type of argument.\n");
        return 1;
    }

    printf("%d\n", shmid);
    return 0;
}
