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

/*operands counter*/
static int operandsInOperator(Operator *op){ 
    int count;
    for(count = 0; count < 3; count++){
        if(op->opd_types[count] == OP_NONE){
            break;
        }
    }
    printf("Count: %d", count);
    return count;
}

/*Analysis of operands*/
static OperandType operandsAnalyser(const char *str, SymbolTable alias_table, 
                              Instruction **instr, unsigned int index){
    /*VERIFICATION 1: REGISTER*/
    if(str[0] == '$'){
        if(strlen(str) == 1){
            /*ERROR: $ EXPECTED NUMBER OF REGISTER*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        }
        else{
            int value = 0; //value is the register number
            int pot = 1;
            for(int i = strlen(str)-1; i > 0; i--){
                if(str[i] < '0' || str[i] > '9'){
                    /*ERROR: $ INVALID REGISTER*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                }
                else{
                    value += pot*(str[i] - 48);
                    pot *= 10;
                }
            }
            if(value > 255){
                /*ERROR: $ INVALID REGISTER*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            }
            else{
                /*RETURN REGISTER;*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            }
        }        
    }
    /*VERIFICATION 2: LABEL*/
    Instruction *current = *instr;
    while(current != NULL){
        if(current->label != NULL && strcmp(current->label, str) == 0){
            /*RETURN LABEL;*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        }
        current = current->next;
    }
    /*VERIFICATION 3: REGISTER ALIAS*/
    if(stable_find(alias_table, str) != NULL){
        /*RETURN REGISTER ALIAS*/
        /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
    }
    int value = 0; //value is the number
    int pot = 1;
    for(int i = strlen(str)-1; i >= 0; i--){
        if(str[i] < '0' || str[i] > '9'){
            /*ERROR: INVALID OPERAND*/ /*GERAL CASE*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
        }
        else{
            value += pot*(str[i] - 48);
            pot *= 10;
        }
    }
    /*RETURN NUMBER*/
    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
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
            /*number of operands validation*/
            if(operandsCounter != operandsInOperator(opFound)){
                if(operandsCounter < operandsInOperator(opFound)){
                    /*ERROR: TOO FEW OPERANDS*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/                
                    printf("ERROR: TOO FEW OPERANDS\n");
                }
                else if(operandsCounter > operandsInOperator(opFound)){
                    /*ERROR: TOO MANY OPERANDS*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/      
                    printf("ERROR: TOO MANY OPERANDS\n");    
                }
            }                   
            for(int i = 0; i <= 3; i++){  
                printf("%d\n", opFound->opd_types[i]);              
                if(i == 3){                        
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                    /*CREATE NEW INSTRUCTION*/
                    printf("CREATE NEW INSTRUCTION\n");
                    break;
                }                
                else{
                    /*CHECK IF THE OPERAND TYPES ARE OK*/
                    printf("CHECK IF THE OPERAND TYPES ARE OK\n");
                    operandsCounter++;
                }
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
    parse("loop     MUL   a, b, 0* Faz multiplicacao\n", NULL, NULL, NULL);
    return 0;    
}