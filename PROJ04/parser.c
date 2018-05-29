#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "error.c"
#include "optable.c"
#include "stable.c"
#include "asmtypes.c"

/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
int operandsCounter; /*the number of Operand in the line(the real number, and not (number-1))*/

/*TO OPERANDS*/
int number;

/*operands counter*/
static int operandsInOperator(Operator *op){ 
    int count;
    for(count = 0; count < 3; count++){
        if(op->opd_types[count] == OP_NONE){
            break;
        }
    }
    return count;
}

/*Analysis of operands*/
static OperandType operandsAnalyser(const char *str, SymbolTable alias_table, 
                              Instruction **instr, unsigned int index){
    //the index variable exists for error message
    /*setting the initial value of variables*/
    number = 0;
    /*VERIFICATION 1: REGISTER*/
    if(str[0] == '$'){
        if(strlen(str) == 1){
            /*ERROR: $ EXPECTED REGISTER NUMBER*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            printf("ERROR: $ EXPECTED REGISTER NUMBER\n");
        }
        else{
            int value = 0; //value is the register number
            int pot = 1;
            for(int i = strlen(str)-1; i > 0; i--){
                if(str[i] < '0' || str[i] > '9'){
                    /*ERROR: $ INVALID REGISTER*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                    printf("ERROR: $ INVALID REGISTER\n");
                }
                else{
                    value += pot*(str[i] - 48);
                    pot *= 10;
                }
            }
            if(value > 255){
                /*ERROR: $ INVALID REGISTER*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                printf("ERROR: $ INVALID REGISTER\n");
            }
            else{
                printf("REGISTER: %s\n", str);
                number = value;
                return REGISTER;
            }
        }        
    }
    /*VERIFICATION 2: STRING*/
    if(str[0] == '\"'){
        if(str[strlen(str)-1] == '\"'){
            printf("STRING: %s\n", str);
            return STRING;
        }
        else{
            /*ERROR: $ INVALID STRING*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            printf("ERROR: INVALID STRING\n");
        }
    }
    /*VERIFICATION 3: LABEL*/
    Instruction *current = *instr;
    while(current != NULL){
        if(current->label != NULL && strcmp(current->label, str) == 0){        
            printf("LABEL: %s\n", str);
            return LABEL;
        }
        current = current->next;        
    }
    /*VERIFICATION 4: REGISTER ALIAS*/
    if(stable_find(alias_table, str) != NULL){        
        printf("REGISTER ALIAS: %s\n", str);
        return REGISTER;
    }
    /*VERIFICATION 5: NUMBER*/
    int value = 0; //value is the number
    int pot = 1;
    for(int i = strlen(str)-1; i >= 0; i--){
        if(str[i] < '0' || str[i] > '9'){
            /*ERROR: INVALID OPERAND*/ /*GERAL CASE*/
            /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
            printf("ERROR: INVALID OPERAND\n");
        }
        else{
            value += pot*(str[i] - 48);
            pot *= 10;
        }
    }
    printf("NUMBER: %s\n", str);
    number = value;
    return NUMBER_TYPE;
}

static char** operandsFinder(const char *str, unsigned int index){
    char **token = malloc(sizeof(char**));
    char *tmp = malloc(sizeof(char)*strlen(str));
    int tmpCounter = 0; //charCounter to tmp
    int space = 0;//flag to identifies the space
    int doubleQuotes = 0;//flag to identifies the first double-quote char
    strcpy(tmp, "");
    operandsCounter = 0; //initializing the counter variable
    for(unsigned int i = index; i < strlen(str); i++){
        if(i == strlen(str)-1){
            if(strcmp("", tmp) == 0){
                /*ERROR: OPERAND EXPECTED (Example: a,,b)*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                printf("ERROR: OPERAND EXPECTED");
            }
            token[operandsCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            free(tmp);
            char *tmp = malloc(sizeof(char)*strlen(str));
            strcpy(tmp, ""); //reset the tmp string
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
        }
        if(isspace(str[i])){
            if(strcmp("", tmp) == 0){
                continue;
            }
            else{
                if(doubleQuotes == 0){                    
                    space = 1; 
                }            
                else{
                    tmp[tmpCounter++] = str[i];
                }                   
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
            free(tmp);
            char *tmp = malloc(sizeof(char)*strlen(str));
            strcpy(tmp, ""); //reset the tmp string
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
            break;
        }
        else if(str[i] == ','){
            if(strcmp("", tmp) == 0){
                /*ERROR: OPERAND EXPECTED (Example: a,,b)*/
                printf("ERROR: OPERAND EXPECTED");
            }
            token[operandsCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            free(tmp);
            char *tmp = malloc(sizeof(char)*strlen(str));
            strcpy(tmp, ""); //reset the tmp string
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
        }
        else{
            if(space == 1){
                /*ERROR: TWO OPERANDS AT THE SAME BLOCK OF OPERANDS*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                printf("ERROR: TWO OPERANDS AT THE SAME BLOCK OF OPERANDS\n");
            }
            else{
                if(str[i] == '\"'){
                    doubleQuotes = 1;
                }
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
        /*so it's necessary to check if the label already exists*/
        Instruction *current = *instr;
        while(current != NULL){
            if(current->label != NULL && strcmp(current->label, firstStr) == 0){        
                /*ERROR: LABEL ALREADY EXISTS*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO */
                printf("ERROR: LABEL ALREADY EXISTS\n");
            }
            current = current->next;        
        }
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
            printf("ERROR: EXPECTED OPERATOR\n");
        }
        else{
            /*a operator was found, so the next block of strings needs to be the operands*/
            char **operands = operandsFinder(str, endOfSecondStr);
            Operator* opFound = optable_find(secondStr);  
            Operand *newOperands[3];        
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
            for(int i = 0; i < operandsInOperator(opFound); i++){             
               /*CHECK IF THE OPERAND TYPES ARE OK*/
                OperandType opType = operandsAnalyser(operands[i], alias_table, instr, 0);
                if (!(opType & opFound->opd_types[i])) { // bitwise and
                    /*ERROR: WRONG OPERAND TYPE*/
                    /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/      
                    printf("ERROR: WRONG OPERAND TYPE\n");
                }
                else{
                    printf("OPERAND OK\n");
                    if(opType == REGISTER){
                        if(operands[i][0] == '$'){
                            newOperands[i] = operand_create_register(number);
                        }
                        else{
                            newOperands[i] = operand_create_register(stable_find(alias_table, operands[i]));
                        }
                    }
                    else if(opType == LABEL){
                        newOperands[i] = operand_create_label(operands[i]);
                    }
                    else if(opType == STRING){
                        newOperands[i] = operand_create_string(operands[i]);
                    }
                    else{
                        newOperands[i] = operand_create_number(number);
                     }
                }
                operandsCounter++;                
            }
            Instruction *new = instr_create(firstStr, opFound, newOperands);
            printf("label: %s | operator: %s | operandType: %d | operandType: %d | operandType: %d\n", new->label, new->op->name, new->opds[0]->type,new->opds[1]->type,new->opds[2]->type);
            new->next = *instr;
            *instr = new;
            printf("NEW INSTRUCTION CREATED\n");
        }
    }
    else{
        /*the first token is a OPERATOR, so the second block of strings needs to be the OPERANDS*/
        /*a operator was found, so the next block of strings needs to be the operands*/
        char **operands = operandsFinder(str, endOfFirstStr);
        Operator* opFound = optable_find(firstStr);                      
        Operand *newOperands[3];
        /*number of operands validation*/
        if(operandsCounter != operandsInOperator(opFound)){
            if(operandsCounter < operandsInOperator(opFound)){
                /*ERROR: TOO FEW OPERANDS*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
                printf("OperandsCoutner %d", operandsCounter);               
                printf("ERROR: TOO FEW OPERANDS\n");
            }
            else if(operandsCounter > operandsInOperator(opFound)){
                /*ERROR: TOO MANY OPERANDS*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/      
                printf("ERROR: TOO MANY OPERANDS\n");    
            }
        }                   
        for(int i = 0; i < operandsInOperator(opFound); i++){             
            /*CHECK IF THE OPERAND TYPES ARE OK*/
            OperandType opType = operandsAnalyser(operands[i], alias_table, instr, 0);
            if (!(opType & opFound->opd_types[i])) { // bitwise and
                /*ERROR: WRONG OPERAND TYPE*/
                /*TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/      
                printf("ERROR: WRONG OPERAND TYPE\n");
            }
            else{
                printf("OPERAND OK\n");
                if(opType == REGISTER){
                    if(operands[i][0] == '$'){
                        newOperands[i] = operand_create_register(number);
                    }
                    else{
                        newOperands[i] = operand_create_register(stable_find(alias_table, operands[i]));
                    }
                }
                else if(opType == LABEL){
                    newOperands[i] = operand_create_label(operands[i]);
                }
                else if(opType == STRING){
                    newOperands[i] = operand_create_string(operands[i]);
                }
                else{
                    newOperands[i] = operand_create_number(number);
                }
                printf("OPTYPE: %d\n", newOperands[i]->type);
            }
            operandsCounter++;
        }
        Instruction *new = instr_create(NULL, opFound, newOperands);
        printf("label: %s | operator: %s | operandType: %d | operandType: %d | operandType: %d\n", new->label, new->op->name, new->opds[0]->type,new->opds[1]->type,new->opds[2]->type);
        new->next = *instr;
        *instr = new;
        printf("NEW INSTRUCTION CREATED\n");
    }
    return 0;
}

int main(int argc, char *argv[]){
    SymbolTable st = stable_create();   
    stable_insert(st, "a");
    Instruction **new = malloc(sizeof(Instruction**));
    parse(" loop MUL   a , a, $1* Faz multiplicacao\n", st, new, NULL);
    parse(" 1 MUL   a , loop, $1* Faz multiplicacao\n", st, new, NULL);
    parse(" 2 JMP   loop*\n", st, new, NULL);
    parse(" 4 JMP   1*\n", st, new, NULL);
    parse(" 5 JMP   2*\n", st, new, NULL);
    parse(" 5 JMP   5*\n", st, new, NULL);
    return 0;    
}