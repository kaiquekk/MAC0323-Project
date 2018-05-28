#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "error.h"
#include "optable.c"

/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
int operandsCounter; /*the number of Operand in the line(the real number, and not (number-1))*/

static char** operandsAnalyser(const char *str, unsigned int index){
    
}

static char** operandsFinder(const char *str, unsigned int index){
    char **token = malloc(sizeof(char**));
    char *tmp = malloc(sizeof(char)*strlen(str));
    int tmpCounter = 0; //charCounter to tmp
    int space = 0;//flag to identifies the space
    strcpy(tmp, "");
    operandsCounter = 0; //initializing the counter variable
    for(unsigned int i = index; i < strlen(str); i++){
        if(isspace(str[i])){
            if(strcmp("", tmp) == 0){
                continue;
            }
            else{
                space = 1;                
            }
        }
        else if(str[i] == '*'){
            if(strcmp("", tmp) == 0){
                /*ERROR: OPERAND EXPECTED (Example: a,,b)*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                printf("ERROR: OPERAND EXPECTED");
            }
            token[operandsCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            strcpy(tmp, ""); //reset the tmp string
            tmpCounter = 0;
            space = 0;
            break;
        }
        else if(str[i] == ','){
            if(strcmp("", tmp) == 0){
                /*ERROR: OPERAND EXPECTED (Example: a,,b)*/
                printf("ERROR: OPERAND EXPECTED");
            }
            token[operandsCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            printf("%s\n", token[operandsCounter-1]);
            strcpy(tmp, ""); //reset the tmp string
            tmpCounter = 0;
            space = 0;
        }
        else{
            if(space == 1){
                /*ERROR: TWO OPERANDS AT THE SAME BLOCK OF OPERANDS*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                printf("ERROR: TWO OPERANDS AT THE SAME BLOCK OF OPERANDS\n");
            }
            else{
                tmp[tmpCounter++] = str[i];
            }
        }
    }
    return token;
}

int parse(const char *s, SymbolTable alias_table, Instruction **instr,
          const char **errptr){
    char *str = (char*)malloc(sizeof(char)*strlen(s));
    strcpy(str, s);
    char **token = malloc(sizeof(char**));
    char *firstStr = (char*)malloc(sizeof(char)*strlen(str));    
    int firstStrCounter = 0, endOfFirstStr = 0;
    //firstStr = first string of line
    //firstStrCounter = length of first string of line
    //endOfFirstString = index of next char that does not belong of firstString
    char *secondStr = (char*)malloc(sizeof(char)*strlen(str));    
    int secondStrCounter = 0, endOfSecondStr = 0;
    strcpy(firstStr, "");
    strcpy(secondStr, "");
    for(unsigned int i = 0; i < strlen(str); i++){
        /*Identifies the first string before a space char*/
        if(isspace(str[i]) && strcmp(firstStr, "") != 0){
            endOfFirstStr = i+1;
            break;
        }
        if(isspace(str[i]) && strcmp(firstStr, "") == 0){
            continue;
        }
        firstStr[firstStrCounter++] = str[i];
    }
    /*Now, the first string was found, and is necessary to know if it is a label or a operator*/
    if(optable_find(firstStr) == NULL){
        /*the first token is not a operator, so it will be assumed the first token is a LABEL*/
        /*so the second string needs to be a operator*/
        for(unsigned int i = endOfFirstStr; i < strlen(str); i++){
            /*Identifies the second string before a space char*/
            if(isspace(str[i]) && strcmp(secondStr, "") != 0){
                endOfSecondStr = i+1;
                break;
            }
            if(isspace(str[i]) && strcmp(secondStr, "") == 0){
                continue;
            }
            secondStr[secondStrCounter++] = str[i];            
        }
        if(optable_find(secondStr) == NULL){
            /*ERROR: EXPECTED OPERATOR*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
        }
        else{
            /*a operator was found, so the next block of strings needs to be the operands*/
            char **operands = operandsFinder(str, endOfSecondStr);
            Operator* opFound = optable_find(secondStr);            
            int opFoundNumberOperands = 0;
            for(int i = 0; i; i++){
                if(opFound->opd_types[i] == NULL){
                    /*now is necessary check if the operands number is correct*/
                    if(operandsCounter == i){
                        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                        /*CREATE NEW INSTRUCTION*/
                    }
                    else{
                        /*ERROR: TOO MANY ARGUMENTS*/
                        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                    }
                }
                else if(operandsCounter < i){
                    /*ERROR: TOO FEW ARGUMENTS*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                }
                else if()
            }
            printf("OperandsCounter = %d\n", operandsCounter);
            printf("%s|%s|%s|%s|%s|\n", firstStr, secondStr, operands[0], operands[1], operands[2]);
        }
    }
    else{
        /*the first token is a OPERATOR, so the second block of strings needs to be the OPERANDS*/
        char **operands = operandsFinder(str, endOfSecondStr);
        printf("%s|%s|%s|%s|%s|\n", firstStr, secondStr, operands[0], operands[1], operands[2]);
    }
    return 0;
}

int main(int argc, char *argv[]){
    parse("loop     MUL   a, b,  0  * Faz multiplicacao\n", NULL, NULL, NULL);
    return 0;    
}