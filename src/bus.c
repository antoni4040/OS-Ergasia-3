#include "bus.h"

int main(int argc, char** argv) {
    enum region destination = ASK;
    int passengerCount = 0;
    int maxToBoard = 0;
    unsigned int parkPeriod = 0;
    unsigned int manTime = 0;
    int segmentID = 0;
    void* segmentStart;
    enum region parkingBay;
    int parkingSpot;
    char* outputFile = NULL;
    time_t rawtime;
    struct tm* timeinfo;

    if(argc != 15) {    // Check if right number of command line arguments are given.
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
            parkPeriod = (unsigned int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-m") == 0) {     //Maneuver time.
            i++;
            manTime = (unsigned int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-s") == 0) {     //Shared memory segment id.
            i++;
            segmentID = (int)strtol(argv[i], NULL, 10);
        }
        else if(strcmp(argv[i], "-o") == 0) {     //Output file.
            i++;
            outputFile = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(outputFile, argv[i]);
        }
        else {
            fprintf(stderr, "Unrecognised type of argument %s.\n", argv[i]);
            return 1;
        }
    }

    FILE* output = fopen(outputFile, "a");
    if(output == NULL) {
        fprintf(stderr, "Bus: could not open output file.\n");
        return 1;
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("BUS %d: dest:%d  passengers:%d maxToBoard:%d Park:%d Man:%d\n",
            getpid(), destination, passengerCount, maxToBoard, parkPeriod, manTime);
    fprintf(output, "Time: %s BUS %d: dest:%d  passengers:%d maxToBoard:%d Park:%d Man:%d \n", asctime(timeinfo),
            getpid(), destination, passengerCount, maxToBoard, parkPeriod, manTime);
    fclose(output);

    // Attach shared memory segment.
    segmentStart = (void*) shmat(segmentID, NULL, 0);
    if(*(int*)segmentStart == -1)
        perror("Attachment of shared memory segment failed.\n");

    // Get station at beginning of segment and all 3 bays with pointer arithmetic from start.
    station* Station = (station*) segmentStart;
    bus* ASKBay = (bus*)(segmentStart + sizeof(station));
    bus* PELBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * Station->ASKsize);
    bus* VORBay = (bus*)(segmentStart + sizeof(station) + sizeof(bus) * (Station->ASKsize + Station->PELsize));

    // Ask to go in.
    int notAllowed = 1;
    while(notAllowed == 1) {
        // Wait for station manager to signal available spots.
        if(destination == ASK)
            sem_wait(&Station->ASKcome);
        if(destination == PEL)
            sem_wait(&Station->PELcome);
        if(destination == VOR)
            sem_wait(&Station->VORcome);

        // Only one bus entering.
        sem_wait(&Station->waitForIn);
        // Mutex for talking to manager.
        sem_wait(&Station->mutex);
        Station->requestType = IN;
        Station->requestRegion = destination;
        // Wakeup manager and await his answer.
        sem_post(&Station->request);
        sem_wait(&Station->awaitAnswer);
        if(Station->allowRequest == ALLOW) {
            //Go to spot.
            sem_post(&Station->mutex);
            notAllowed = 0;
        }
        else {
            sem_post(&Station->mutex);
            sem_post(&Station->waitForIn);
            printf("Station full, spending time here.\n");
//            sleep(WAIT_IF_FULL);
        }
    }

    // Maneuver in the station and reach parking spot.
    output = fopen(outputFile, "a");
    if(output == NULL) {
        fprintf(stderr, "Bus: could not open output file.\n");
        return 1;
    }
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Bus %d moving in station.\n", getpid());
    fprintf(output,"Time: %s BUS: %d moving in station.\n", asctime(timeinfo), getpid());
    fclose(output);

    sleep(manTime);
    if(Station->goToBay == ASK) {
        parkingBay = ASK;
        parkingSpot = Station->goToSpot;
        ASKBay[Station->goToSpot].busID = getpid();
        ASKBay[Station->goToSpot].destination = destination;
    }
    else if(Station->goToBay == PEL) {
        parkingBay = PEL;
        parkingSpot = Station->goToSpot;
        PELBay[Station->goToSpot].busID = getpid();
        PELBay[Station->goToSpot].destination = destination;
    }
    else {
        parkingBay = VOR;
        parkingSpot = Station->goToSpot;
        VORBay[Station->goToSpot].busID = getpid();
        VORBay[Station->goToSpot].destination = destination;
    }
    srand(time(0));
    Station->busesArrived += 1;
    int passengersComingDown =  rand() % passengerCount + MIN_PASSENGERS_COME;
    Station->passengersCome += passengersComingDown;
    Station->movingIn = 0;
    sem_post(&Station->waitForIn);

    // Passengers go, passengers come, randomly for the needs of this exercise.
    output = fopen(outputFile, "a");
    if(output == NULL) {
        fprintf(stderr, "Bus: could not open output file.\n");
        return 1;
    }
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Bus %d parked.\n", getpid());
    fprintf(output,"Time: %s BUS: %d parked. %d passengers coming down.\n", asctime(timeinfo),
            getpid(), passengersComingDown);
    fclose(output);

    sleep(parkPeriod);

    // Ask to leave station.
    int passengersLeaving = 0;
    notAllowed = 1;
    while(notAllowed == 1) {
        sem_wait(&Station->waitForOut);
        sem_wait(&Station->mutex);
        Station->requestType = OUT;
        sem_post(&Station->request);
        sem_wait(&Station->awaitAnswer);
        if(Station->allowRequest == ALLOW) {
            //Leave.
            passengersLeaving = rand() % (maxToBoard - passengersComingDown);
            Station->passengersLeft += passengersLeaving;
            Station->totalStayAtStation += (int)parkPeriod;
            sem_post(&Station->mutex);
            notAllowed = 0;
        }
        // Never gonna reach this part, but just for good measure.
        else {
            sem_post(&Station->mutex);
            sem_post(&Station->waitForOut);
        }
    }

    // Maneuver out of station.
    output = fopen(outputFile, "a");
    if(output == NULL) {
        fprintf(stderr, "Bus: could not open output file.\n");
        return 1;
    }
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Bus %d leaving station.\n", getpid());
    fprintf(output,"Time: %s BUS: %d leaving station. %d passengers boarded.\n", asctime(timeinfo),
            getpid(), passengersLeaving);
    fclose(output);

    if(parkingBay == ASK) {
        Station->ASKavailable += 1;
        ASKBay[parkingSpot].busID = 0;
    }
    else if(parkingBay == PEL) {
        Station->PELavailable += 1;
        PELBay[parkingSpot].busID = 0;
    }
    else {
        Station->VORavailable += 1;
        VORBay[parkingSpot].busID = 0;
    }
    sleep(manTime);
    Station->movingOut = 0;
    Station->busesLeft += 1;
    sem_post(&Station->waitForOut);

    output = fopen(outputFile, "a");
    if(output == NULL) {
        fprintf(stderr, "Bus: could not open output file.\n");
        return 1;
    }
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Bus %d left station.\n", getpid());
    fprintf(output,"Time: %s BUS: %d left the station.\n", asctime(timeinfo),
            getpid());
    fclose(output);

    //Last bus tells the station manager to fuck off.
    if(Station->busesLeft == NUM_OF_BUSES + NUM_OF_TTY_BUSES) {
        Station->requestType = OVER;
        sem_post(&Station->request);
    }

    // Detach shared memory segment.
    int err = shmdt((void*)segmentStart);
    if(err == -1)
        perror("Detachment of shared memory segment failed in bus.\n");

    free(outputFile);
    // Leave station and go West to the land of the Elves or whatever.
    return 0;
}
