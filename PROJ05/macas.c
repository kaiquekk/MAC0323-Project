#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv){
    const char *posDot = strchr(argv[1], 46);
    if(argc > 2){
        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        printf("ERROR: too many arguments\n");
    }
    else if(strcmp(posDot, ".as") != 0){
        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        printf("ERROR: wrong file extension.\n");
    }
    else if(access(argv[1], R_OK) == -1){
        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        printf("ERROR: no such file.\n");
    }
    else{
        /*CALL assemble*/
    }        
}