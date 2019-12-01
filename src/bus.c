#include "bus.h"

int main(int argc, char** argv) {
    enum region destination = ASK;
    int passengerCount = 0;
    int maxToBoard = 0;
    float parkPeriod = 0.0f;
    float manTime = 0.0f;
    int shmid = 0;

    if(argc != 13) {    // Check if right number of command line arguments are given.
        fprintf(stderr, "Wrong number of command line arguments given.\n");
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-t") == 0) {    //Type of destination.
            i++;
            if(strcmp(argv[i], "ASK") == 0) {
                destination = ASK;
            }
            else if(strcmp(argv[i], "PEL") == 0) {
                destination = PEL;
            }
            else if(strcmp(argv[i], "VOR") == 0) {
                destination = VOR;
            }
            else {
                fprintf(stderr, "Region code not recognised.\n");
                return 1;
            }
        }
        else if(strcmp(argv[i], "-n") == 0) {     //Number of passengers.
            i++;
            passengerCount = (int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-c") == 0) {     //Max number of passengers to board.
            i++;
            maxToBoard = (int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-p") == 0) {     //Park period.
            i++;
            parkPeriod = strtof(argv[i], NULL);
        }
        else if(strcmp(argv[i], "-m") == 0) {     //Maneuver time.
            i++;
            manTime = strtof(argv[i], NULL);
        }
        else if(strcmp(argv[i], "-s") == 0) {     //Shared memory segment id.
            i++;
            shmid = (int)strtol(argv[i], NULL, 10);
        }
    }

    printf("%d %d %d %f %f %d\n", destination, passengerCount, maxToBoard, parkPeriod, manTime, shmid);

    return 0;
}
