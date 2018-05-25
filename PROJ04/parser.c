#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "error.h"
#include "optable.c"

/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
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
                words[wordCounter++] = tmp;
                free(tmp);
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

int label_check(char *label){//check if the label is valid

}

int parse(const char *s, SymbolTable alias_table, Instruction **instr,
          const char **errptr){
    Instruction *newInstr = emalloc(sizeof(Instruction*));   
    char *tokAux;
    char *tokens[3];
    char *s_copy = emalloc(sizeof(s));
    strcpy(s_copy, s);
    tokAux = strtok(s_copy, " ");
    int i = 0;
    while(tokAux!=NULL && i<3){//maximum strings to consider on a line: label, operand and value
        tokens[i++] = tokAux;
        tokAux = strtok(NULL, " ");
    }
    if(i<3){//line has no label or can be a "label NOP" line
        if(strcmp(tokens[1], "NOP") == 0){//line is "label NOP"
            if(label_check(tokens[0]) == 0){//label is invalid
                set_error_msg("invalid label name");
                errptr = "0";//coloquei apontando pro inicio da linha pois n sei como fazer o controle corretamente com a implementação do strtok
                return 0;
            }
            else{
                //fill instruction newInstr 
                newInstr->label = emmalloc(sizeof(tokens[0]));
                strcpy(newInstr->label, tokens[0]);
                newInstr->op = optable_find(tokens[1]);
                //newInstr->lineno ?
                //newInstr->pos ?
                Instruction *iptr = instr;
                while(iptr->next != NULL){
                    iptr = iptr->next;                    
                }
                iptr->next = newInstr;
                return 1;
            }
        }
        const Operator *op = optable_find(tokens[0]);
        if(op==NULL){
            set_error_msg("invalid operator");
            errptr = "0";
            return 0;
        }
        else{
            
        }
    }    
}

int main(int argc, char *argv[]){
    
    return 0;
    
}