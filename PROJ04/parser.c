#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "error.h"
#include "optable.c"

/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
int tokenCounter = 0; /*the number of Operand in the line(the real number, and not (number-1))*/

static char** split(const char *str){
    char **words = malloc(sizeof(char**));
    int len = strlen(str);
    char *tmp = (char*)malloc(sizeof(char)*len);
    for(int i = 0, j = 0, wordCounter = 0; i < len; i++){
        if(i == len-1 && !isspace(str[i])){
            tmp[j++] = str[i];
        }
        if(isspace(str[i]) || i == len-1){
            if(j > 0){
                words[wordCounter] = (char*)malloc(sizeof(char)*(j+1));                
                strcpy(words[wordCounter], tmp);
                wordCounter++;
                tmp = (char*)malloc(sizeof(char)*len);
                j = 0;
            }
        }
        else{
            tmp[j++] = str[i];
        }
    }
    return words;
}
static char** operandsFinder(const char *str, unsigned int index){
    char **token = malloc(sizeof(char**));
    char *tmp = malloc(sizeof(char)*strlen(str));
    int tmpCounter = 0; //charCounter to tmp
    int tokenCounter = 0; //token Counter to token
    int space = 0;//flag to identifies the space
    strcpy(tmp, "");
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
                printf("ERROR: OPERAND EXPECTED");
            }
            token[tokenCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[tokenCounter++], tmp); //put the tmp in the token array
            printf("%s\n", token[tokenCounter-1]);
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
            printf("ERROR: OPERAND EXPECTED");
            token[tokenCounter] = (char*)malloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[tokenCounter++], tmp); //put the tmp in the token array
            printf("%s\n", token[tokenCounter-1]);
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
/*Casos:
    2 palavras:
        - label NOP
            - checar se a label é válida
                - label começa com uma letra ou underscore, contendo apenas letras, numeros e underscores e não é um nome de operador
                - label não é o "nome" de um Registrador atribuido pelo operador IS 
        - OPERADOR Operandos
            - checar se o operador existe na optable
            - checar se os operandos sao validos de acordo com o operador
    3 palvras:
        - label OPERADOR Operandos
            - checar se a label é valida
            - checar se o operador existe na optable
            - checar se os operandos sao validos de acordo com o operador
        - nome IS Registrador    
            - checar se nome ja existe na stable
            - checar se nome é um nome valido
            - adicionar na stable nome com o devido registrador
*/

int parse(const char *s, SymbolTable alias_table, Instruction **instr,
          const char **errptr){
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
            printf("%s|%s|%s|%s|%s|\n", firstStr, secondStr, operands[0], operands[1], operands[2]);
        }
    }
    else{
        /*the first token is a OPERATOR, so the second block of strings needs to be the OPERANDS*/
        char **operands = operandsFinder(str, endOfSecondStr);
        printf("%s|%s|%s|%s|%s|\n", firstStr, secondStr, operands[0], operands[1], operands[2]);
    }
}

int main(int argc, char *argv[]){
    return 0;    
}