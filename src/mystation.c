#include "mystation.h"

int main(int argc, char** argv) {
    char* configFile;
    int segmentID = 0;
    station *Station, *StationSeg;
    void* segmentStart;

    // Check and get command line parameters.
    if(argc != 3) {
        fprintf(stderr, "Something went wrong with the command line parameters.\n");
        return 1;
    }

    if(strcmp(argv[1], "-l") != 0) {
        fprintf(stderr, "Unrecognised type of argument %s.\n", argv[1]);
        return 1;
    } else {
        configFile = malloc((strlen(argv[2]) + 1) * sizeof(char));
        strcpy(configFile, argv[2]);
    }

    // Check if config file exists.
    if(!checkConfigFileExists(configFile)) {
        fprintf(stderr, "Config file doesn't exist.\n");
        return 1;
    }

    // Create local station struct(using malloc).
    Station = malloc(sizeof(station));
    if(readConfigFile(Station, configFile) != 0)
        return 1;

    // Initialize semaphores.
    sem_init(&(Station->mutex), 1, 1);
    sem_init(&(Station->request), 1, 0);
    sem_init(&(Station->awaitAnswer), 1, 0);
    sem_init(&(Station->waitForIn), 1, 1);
    sem_init(&(Station->waitForOut), 1, 1);
    Station->goToSpot = -1;

    // Compute proper size for shared memory segment.
    size_t segmentSize = sizeof(station) + (
            Station->ASKsize + Station->PELsize + Station->VORsize
            ) * sizeof(bus);

    // Create shared memory segment.
    segmentID = shmget(IPC_PRIVATE, segmentSize, 0666);
    if(segmentID == -1)
        perror("Creation of shared memory segment failed.\n");

    // Attach shared memory segment.
    segmentStart = (void*) shmat(segmentID, NULL, 0);
    if(*(int*)segmentStart == -1)
        perror("Attachment of shared memory segment failed.\n");

    // Create station at beginning of segment.
    StationSeg = (station*) segmentStart;
    memcpy(StationSeg, Station, sizeof(station));

    // Initialize empty bays.
    initializeBays(segmentStart);

    // Create comptroller and quit if it fails.
    int errC = createComptroller(segmentID);
    if(errC == -1)
        return 1;

    // Create station manager and quit if it fails.
    int errS = createStationManager(segmentID);
    if(errS == -1)
        return 1;

    int pid, status;
    while((pid=wait(&status)) > 0);

    // Remove shared memory segment.
    int err = shmctl(segmentID, IPC_RMID, 0);
    if(err == -1)
        perror("Removal of shared memory segment failed.\n");

    free(configFile);
    return 0;
}

// Fork and execlp the comptroller process, passing the shared memory segment ID.
int createComptroller(int segmentID) {
    char segmentIDtoStr[20];
    sprintf(segmentIDtoStr, "%d", segmentID);
    int pid = fork();

    // New child process, create comptroller.
    if(pid == 0) {
        execlp("./comptroller", "./comptroller", "-d", ""REPORT_TIME,
                "-t", ""STATS_TIME, "-s", segmentIDtoStr, (char*)NULL);
    }
    // Error during fork.
    else if(pid < 0) {
        fprintf(stderr, "Error forking for comptroller.\n");
        return -1;
    }
    return 0;
}

// Fork and execlp the station manager process, passing the shared memory segment ID.
int createStationManager(int segmentID) {
    char segmentIDtoStr[20];
    sprintf(segmentIDtoStr, "%d", segmentID);
    int pid = fork();

    // New child process, create comptroller.
    if(pid == 0) {
        execlp("./station-manager", "./station-manager", "-s", segmentIDtoStr, (char*)NULL);
    }
    // Error during fork.
    else if(pid < 0) {
        fprintf(stderr, "Error forking for station manager.\n");
        return -1;
    }
    return 0;
}

// Instead of getting the buses from a file, we'll create them randomly.
int createRandomBus(int segmentID) {
    
}

// Initialize empty parking spots in ASK, PEL and VOR bays.
void initializeBays(void* segmentStart) {
    station* Station = (station*)segmentStart;

    // Initialize ASK bay.
    bus* ASKBay = (bus*)(segmentStart + sizeof(station));
    for(int i = 0; i < Station->ASKsize; i++) {
        ASKBay[i].busID = 0;
    }

    // Initialize PEL bay.
    bus* PELBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * Station->ASKsize);
    for(int i = 0; i < Station->PELsize; i++) {
        PELBay[i].busID = 0;
    }

    // Initialize VOR bay.
    bus* VORBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * (Station->ASKsize + Station->PELsize));
    for(int i = 0; i < Station->VORsize; i++) {
        VORBay[i].busID = 0;
    }
}