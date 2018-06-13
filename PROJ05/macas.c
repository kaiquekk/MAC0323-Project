#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv){
    if(argc > 2){
        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        printf("ERROR: too many arguments\n");
    }
    else{
        const char *posDot = strchr(argv[1], 46);
        if(strcmp(posDot, ".as") != 0){
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            printf("ERROR: wrong file extension.\n");
        }
        else{
            /*CALL assemble*/
        }
    }
}