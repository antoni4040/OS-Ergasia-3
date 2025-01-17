#include "files.h"

/*
Check if given config filename exists.
*/
bool checkConfigFileExists(char* filename) {
    struct stat fileExists;
    return (stat (filename, &fileExists) == 0);
}

/*
Create or empty output file.
*/
int initializeOutputFile(char* filename) {
    FILE* output = fopen(filename, "w");
    if(output == NULL) {
        fprintf(stderr, "Problem creating output file.\n");
        return 1;
    }
    fprintf(output, "Output File:\n");
    fclose(output);
    return 0;
}

/*
Read config file and initialize the station's bay information accordingly.
*/
int readConfigFile(station* Station, char* configFile) {
    FILE* config = fopen(configFile, "r");
    if(config == NULL) {
        fprintf(stderr, "Problem opening config file\n");
        return 1;
    }

    char* line = NULL;
    size_t len = 0;
    char *bayCode, *bayCapacity;
    for(int i = 0; i < 3; i++) {        // Read bay capacities.
        getline(&line, &len, config);
        bayCode = strtok(line, " ");
        bayCapacity = strtok(NULL, " ");
        if(strcmp(bayCode, "ASK") == 0) {       // Initialize ASK bay area.
            Station->ASKsize = (int)strtoul(bayCapacity, NULL, 10);
            Station->ASKavailable = Station->ASKsize;
        }
        else if(strcmp(bayCode, "PEL") == 0) {      // Initialize PEL bay area.
            Station->PELsize = (int)strtoul(bayCapacity, NULL, 10);
            Station->PELavailable = Station->PELsize;
        }
        else if(strcmp(bayCode, "VOR") == 0) {      // Initialize VOR bay area.
            Station->VORsize = (int)strtoul(bayCapacity, NULL, 10);
            Station->VORavailable = Station->VORsize;
        }
        else {
            fprintf(stderr, "Unrecognised region code.\n");
            return 1;
        }
    }
    free(line);
    fclose(config);
    return 0;
}