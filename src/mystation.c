#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char* configFile;

    if(argc != 3) {
        fprintf(stderr, "Something went wrong with the command line parameters.\n");
        return 1;
    }

    if(strcmp(argv[1], "-l") != 0) {
        fprintf(stderr, "Unrecognised command line parameter.\n");
        return 1;
    } else {
        configFile = malloc((strlen(argv[2]) + 1) * sizeof(char));
        strcpy(configFile, argv[2]);
    }

    printf("Config file given: %s.\n", configFile);

    free(configFile);
    return 0;
}
