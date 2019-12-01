#include "comptroller.h"

int main(int argc, char** argv) {
    float reportInterval;
    float statsTime;
    int shmid;

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
    }

    return 0;
}