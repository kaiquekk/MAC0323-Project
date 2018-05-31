#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "error.c"
#include "optable.c"
#include "stable.c"
#include "asmtypes.c"
#include <math.h>

/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
int operandsCounter, ind, nop, number; /*the number of Operand in the line(the real number, and not (number-1))*/

void add_default_reg(SymbolTable alias_table){
    InsertionResult ir = stable_insert(alias_table, "rA");
    ir.data->opd = operand_create_register(255);
    ir = stable_insert(alias_table, "rR");
    ir.data->opd = operand_create_register(254);
    ir = stable_insert(alias_table, "rSP");
    ir.data->opd = operand_create_register(253);
    ir = stable_insert(alias_table, "rX");
    ir.data->opd = operand_create_register(252);
    ir = stable_insert(alias_table, "rY");
    ir.data->opd = operand_create_register(251);
    ir = stable_insert(alias_table, "rZ");
    ir.data->opd = operand_create_register(250);
}

/*operands counter*/
static int operandsInOperator(const Operator *op){ 
    int count = 0;    
    for(int i = 0; i < 3; i++){
        if(op->opd_types[i] != OP_NONE) count++;
        else break;
    }
    return count;
}

/*Analysis of operands*/
static OperandType operandsAnalyser(const char *str, SymbolTable alias_table, 
                              Instruction **instr, const char **errptr, unsigned int index, const char *s){
    //the index variable exists for error message
    /*setting the initial value of variables*/
    number = 0;
    /*VERIFICATION: REGISTER*/
    if(str[0] == '$'){
        if(strlen(str) == 1){
            set_error_msg("invalid register");
            for(unsigned int y = index; y < strlen(s); y++){
                if(s[y] == '$'){
                    if(!isdigit(s[y+1])){
                        ind = y;
                        break;
                    }
                }
            }
            char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
            memset(errorAux, ' ', ind);
            errorAux[ind] = '^';
            *errptr = errorAux;
            return 0;
        }
        else{
            int value = 0; //value is the register number
            int pot = 1;
            for(int i = strlen(str)-1; i > 0; i--){
                if(!isdigit(str[i])){    
                    set_error_msg("invalid register");
                    for(unsigned int y = index; y < strlen(s); y++){
                        if(s[y] == '$'){
                            while(isdigit(s[y+1]) || isspace(s[y+1]) || s[y+1] == ','){
                                y++;
                            }
                            if(s[y+1] != '$'){
                                ind = y;
                                break;
                            }                          
                        }
                    }
                    char *errorAux = (char*)emalloc(sizeof(char) * ind+2);
                    memset(errorAux, ' ', ind+1);
                    errorAux[ind+1] = '^';
                    *errptr = errorAux;
                    return 0;
                }
                else{
                    value += pot*(str[i] - 48);
                    pot *= 10;
                }
            }
            if(value > 255){
                set_error_msg("invalid register");
                for(unsigned int y = index; y < strlen(s); y++){
                    char *valueC = emalloc(sizeof(char)*3);
                    int counterC = 0;
                    if(s[y] == '$'){
                        while(isdigit(s[y+1])){
                            valueC[counterC++] = s[y+1];
                            if(counterC > 3){
                                ind = y;
                                break;
                            }
                            if(counterC == 3){
                                int valueI = atoi(valueC);
                                if(valueI > 255){
                                    ind = y;
                                    break;
                                }
                            }
                            y++;
                        }
                    }
                    if(counterC >= 3) break;
                }
                char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                memset(errorAux, ' ', ind);
                errorAux[ind] = '^';
                *errptr = errorAux;
                return 0;
            }
            else{
                number = value;
                return REGISTER;
            }
        }        
    }
    /*VERIFICATION: STRING*/
    if(str[0] == '\"'){
        for(unsigned int y = 1; y < strlen(str); y++){
            if(str[y] == '\"'){
                return STRING;
            }
        }
        set_error_msg("invalid string");
        for(unsigned int y = index; y < strlen(s); y++){
            if(s[y] == '\"'){
                if(s[y+strlen(str)-1] != '\"'){
                    ind = y;
                    break;
                }
            }
        }
        char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
        memset(errorAux, ' ', ind);
        errorAux[ind] = '^';
        *errptr = errorAux;
        return 0;
    }
    Instruction *current = *instr;
    while(current != NULL){
        if(current->label != NULL && !strcmp(current->label, str)){  
            return LABEL;
        }
        current = current->next;        
    }

    //VERIFICATION: NUMBER
    if(isdigit(str[0])){
        number = atoi(str);
        return NUMBER_TYPE; 
    }
    /*VERIFICATION: REGISTER ALIAS*/
    if(stable_find(alias_table, str) != NULL){  
        return REGISTER;
    }
    else return LABEL;
    return 0;      
}

static char** operandsFinder(const char *str, unsigned int index, const char **errptr){
    ind = index;
    char **token = emalloc(sizeof(char**));
    char *tmp = emalloc(strlen(str));
    int tmpCounter = 0; //charCounter to tmp
    int space = 0;//flag to identifies the space
    int doubleQuotes = 0;//flag to identifies the first double-quote char
    memset(tmp, '\0', strlen(str));
    operandsCounter = 0; //initializing the counter variable
    for(unsigned int i = index; i <= strlen(str); i++){
        if(i == strlen(str)){
            if(!strcmp("", tmp)){
                if(nop != 0) return token;
                set_error_msg("expected operand");
                char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                memset(errorAux, '\0', ind);
                errorAux[ind] = '^';
                *errptr = errorAux;
                return 0;
            }
            token[operandsCounter] = (char*)emalloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            memset(tmp, '\0', strlen(str));
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
        }
        if(isspace(str[i])){
            if(!strcmp("", tmp)){
                continue;
            }
            else{
                if(doubleQuotes == 0){                    
                    space = 1; 
                }            
                else{
                    tmp[tmpCounter++] = str[i];
                    ind = i;
                }                   
            }
        }
        else if(str[i] == '*'){
            if(!strcmp("", tmp)){
                set_error_msg("expected operand");
                char *errorAux = (char*)emalloc(sizeof(char) * i);
                memset(errorAux, '\0', i-1);
                errorAux[i-1] = '^';
                *errptr = errorAux;
                return 0;
            }
            token[operandsCounter] = (char*)emalloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            memset(tmp, '\0', strlen(str));
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
            break;
        }
        else if(str[i] == ','){
            if(!strcmp("", tmp)){
                set_error_msg("expected operand");
                char *errorAux = (char*)emalloc(sizeof(char) * i);
                memset(errorAux, '\0', i-1);
                errorAux[i-1] = '^';
                *errptr = errorAux;
                return 0;
            }
            token[operandsCounter] = (char*)emalloc(sizeof(char)*(tmpCounter+1)); 
            strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
            memset(tmp, '\0', strlen(str));
            tmpCounter = 0;
            space = 0;
            doubleQuotes = 0;
            ind = i;
        }
        else{
            if(space == 1){
                set_error_msg("invalid operands positioning");
                char *errorAux = (char*)emalloc(sizeof(char) * i);
                memset(errorAux, '\0', i-1);
                errorAux[i-1] = '^';
                *errptr = errorAux;
                return 0;
            }
            else{
                tmp[tmpCounter++] = str[i];
                ind = i;
                if(str[i] == '\"'){
                    if(doubleQuotes == 0) doubleQuotes = 1;
                    else{
                        token[operandsCounter] = (char*)emalloc(sizeof(char)*(tmpCounter+1)); 
                        strcpy(token[operandsCounter++], tmp); //put the tmp in the token array
                        memset(tmp, '\0', strlen(str));
                        tmpCounter = 0;
                        space = 0;
                        doubleQuotes = 0;
                        return token;
                    }
                }
            }
        }
    } 
    return token;
}

int check_label(char *str){
    for(unsigned int index = 0; index < strlen(str); index++){
        if(index==0){
            if(!isalpha(str[0]) && str[0] != '_'){//first letter is not a letter nor an underscore
                set_error_msg("invalid label name");
                return 0;
            }
        }
        else if(!isalpha(str[index]) && !isdigit(str[index]) && str[index] != '_'){//letter on index is not a letter nor a number nor an underscore
            set_error_msg("invalid label name");
            return 0;
        }
    }
    return 1;
}

int parse(const char *s, SymbolTable alias_table, Instruction **instr,
          const char **errptr){    
    nop = 0;
    char *firstStr = (char*)emalloc(strlen(s)); 
    int firstStrCounter = 0, endOfFirstStr = 0;
    //firstStr = first string of line
    //firstStrCounter = length of first string of line
    //endOfFirstString = index of next char that does not belong of firstString  
    int secondStrCounter = 0, endOfSecondStr = 0;
    memset(firstStr, '\0', strlen(s));
    for(unsigned int i = 0; i < strlen(s); i++){
        /*Identifies the first string before a space char*/
        if(isspace(s[i]) && strcmp(firstStr, "") != 0){
            endOfFirstStr = i+1;
            break;
        }
        if(isspace(s[i]) && !strcmp(firstStr, "")){
            continue;
        }
        firstStr[firstStrCounter++] = s[i];
    }
    /*Now, the first string was found, and is necessary to know if it is a label or a operator*/
    const Operator *opF = optable_find(firstStr);
    if(opF == NULL){
        /*the first token is not a operator, so it will be assumed the first token is a LABEL or the operator IS*/ 
        //check if the label is valid
        if(!check_label(firstStr)){            
            int errorAuxCounter = endOfFirstStr - strlen(firstStr);
            char *errorAux = (char*)emalloc(sizeof(char) * errorAuxCounter);
            memset(errorAux, ' ', errorAuxCounter-1);
            errorAux[errorAuxCounter-1] = '^';
            *errptr = errorAux;
            return 0;
        }      
        /*so it's necessary to check if the label already exists*/
        Instruction *current = *instr;
        while(current != NULL){
            if(current->label != NULL && !strcmp(current->label, firstStr)){//label name already on the linked list   
                set_error_msg("label name already in use");
                int errorAuxCounter = endOfFirstStr - strlen(firstStr);
                char *errorAux = (char*)emalloc(sizeof(char) * errorAuxCounter);
                memset(errorAux, ' ', errorAuxCounter-1);
                errorAux[errorAuxCounter-1] = '^';
                *errptr = errorAux;
                return 0;                
            }
            current = current->next;        
        }
        /*so the second string needs to be a operator*/
        char *secondStr = emalloc(strlen(s)-strlen(firstStr));
        memset(secondStr, '\0', strlen(s)-strlen(firstStr));
        for(unsigned int i = endOfFirstStr; i < strlen(s); i++){
            /*Identifies the second string before a space char*/
            if(isspace(s[i]) && strcmp(secondStr, "") != 0){
                endOfSecondStr = i+1;
                break;
            }
            if(isspace(s[i]) && !strcmp(secondStr, "")){
                continue;
            }
            secondStr[secondStrCounter++] = s[i];            
        }        
        if(endOfSecondStr == 0) endOfSecondStr = strlen(s);
        opF = optable_find(secondStr);
        if(opF == NULL){//second string found is not an operator
            set_error_msg("expected operator");
            int errorAuxCounter = endOfSecondStr - strlen(secondStr);
            char *errorAux = (char*)emalloc(sizeof(char) * errorAuxCounter);
            memset(errorAux, ' ', errorAuxCounter-1);
            errorAux[errorAuxCounter-1] = '^';
            *errptr = errorAux;
            return 0;  
        }
        else{
            if(!strcmp(secondStr, "NOP")){
                nop = 1;
            }
            /*CHECKING "IS"*/
            if(!strcmp(secondStr, "IS")){
                Operand **newOperands = (Operand**)emalloc(sizeof(Operand*)*3);
                char **operands = operandsFinder(s, endOfSecondStr, errptr);
                if(operands == 0) return 0;
                if(operandsCounter != operandsInOperator(opF)){
                    if(operandsCounter < operandsInOperator(opF)) set_error_msg("expected operand");
                    else set_error_msg("too many operands");
                    char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                    memset(errorAux, ' ', ind);
                    errorAux[ind] = '^';
                    *errptr = errorAux;  
                    return 0; 
                }                
                OperandType opType = operandsAnalyser(operands[0], alias_table, instr, errptr, 0, s);                
                if(opType == 0) return 0;
                if (!(opType & opF->opd_types[0])) { // bitwise and                    
                    set_error_msg("wrong operand type");
                    for(unsigned int y = endOfSecondStr; y < strlen(s); y++){
                        if(!isspace(s[y])){
                            ind = y;
                            break;                             
                        }
                    }
                    char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                    memset(errorAux, ' ', ind);
                    errorAux[ind] = '^';
                    *errptr = errorAux;  
                    return 0; 
                }
                else{                   
                    if(stable_find(alias_table, firstStr) != NULL){
                        set_error_msg("alias already exists");
                        for(unsigned int y = endOfSecondStr; y < strlen(s); y++){
                            if(!isspace(s[y])){
                                ind = y;
                                break;
                            }
                        }
                        char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                        memset(errorAux, ' ', ind);
                        errorAux[ind] = '^';
                        *errptr = errorAux;  
                        return 0;  
                    }
                    else{
                        InsertionResult ir = stable_insert(alias_table, firstStr);   
                        if(opType == REGISTER){
                            if(operands[0][0] == '$'){                                
                                ir.data->opd = operand_create_register(number);
                                newOperands[0] = ir.data->opd;
                            }
                        }
                        else{
                            ir.data->opd = operand_create_number(number);
                            newOperands[0] = ir.data->opd;
                        }                          
                        if((*instr)->op == NULL){
                            *instr = instr_create(firstStr, opF, newOperands);
                        }
                        else{
                            Instruction *new = *instr;
                            while(new->next != NULL) new = new->next;                            
                            new->next = instr_create(firstStr, opF, newOperands);
                        }                  
                    }                    
                }
                return 1;
            }
            /*a operator was found, so the next block of strings needs to be the operands*/
            char **operands = operandsFinder(s, endOfSecondStr, errptr);
            if(operands == 0) return 0; //error found in the operandsFinder function
            Operand **newOperands = (Operand**)emalloc(sizeof(Operand*)*3); 
            /*number of operands validation*/            
            if(operandsCounter != operandsInOperator(opF)){
                if(operandsCounter < operandsInOperator(opF)) set_error_msg("expected operand");
                else set_error_msg("too many operands");
                char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                memset(errorAux, ' ', ind);
                errorAux[ind] = '^';
                *errptr = errorAux;  
                return 0; 
            }                
            for(int i = 0; i < operandsInOperator(opF); i++){             
                /*CHECK IF THE OPERAND TYPES ARE OK*/
                OperandType opType = operandsAnalyser(operands[i], alias_table, instr, errptr, endOfSecondStr, s);
                //error found in the operandsAnalyser function
                if(opType == 0) return 0;
                if (!(opType & opF->opd_types[i])) { // bitwise and
                    EntryData *ed = stable_find(alias_table, operands[i]);
                    if(opType == LABEL && ed != NULL){
                        if(!(ed->opd->type & opF->opd_types[i])){
                            set_error_msg("wrong operand type");
                            int comCount = 0;
                            for(unsigned int y = endOfSecondStr; y < strlen(s); y++){
                                if(!isspace(s[y]) && s[y] != ','){
                                    if(comCount == i){
                                        ind = y;
                                        break;
                                    }
                                    else continue;
                                }
                                else if(s[y] == ',') comCount++;
                            }
                            char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                            memset(errorAux, ' ', ind);
                            errorAux[ind] = '^';
                            *errptr = errorAux;
                            return 0;
                        }
                        else{
                            if(ed->opd->type == REGISTER){
                                newOperands[i] = operand_create_register(ed->opd->value.reg);
                            }
                            else{
                                newOperands[i] = operand_create_number(ed->opd->value.num);
                            }
                        }
                    }
                    else if(opType == LABEL && ed == NULL && (opF->opd_types[i] & LABEL)){
                        newOperands[i] = operand_create_label(operands[i]);
                    }
                    else{
                        set_error_msg("wrong operand type");
                        int comCount = 0;
                        for(unsigned int y = endOfSecondStr; y < strlen(s); y++){
                            if(!isspace(s[y]) && s[y] != ','){
                                if(comCount == i){
                                    ind = y;
                                    break;
                                }
                                else continue;
                            }
                            else if(s[y] == ',') comCount++;
                        }
                        char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                        memset(errorAux, ' ', ind);
                        errorAux[ind] = '^';
                        *errptr = errorAux;  
                        return 0; 
                    } 
                }
                else{
                    if(opType == REGISTER){
                        if(operands[i][0] == '$'){
                            newOperands[i] = operand_create_register(number);
                        }
                        else{
                            EntryData *ed = stable_find(alias_table, operands[i]);
                            newOperands[i] = operand_create_register(ed->opd->value.reg);
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
            }
            if((*instr)->op == NULL){
                *instr = instr_create(firstStr, opF, newOperands);
            }
            else{
                Instruction *new = *instr;
                while(new->next != NULL){
                    new = new->next;
                }
                new->next = instr_create(firstStr, opF, newOperands);
            }              
        }
    }
    else{
        if(!strcmp(firstStr, "NOP")){
            nop = 1;
        }
        /*the first token is a OPERATOR, so the second block of strings needs to be the OPERANDS*/
        /*a operator was found, so the next block of strings needs to be the operands*/
        char **operands = operandsFinder(s, endOfFirstStr, errptr);
        if(operands == 0) return 0;    
        Operand **newOperands = (Operand**)emalloc(sizeof(Operand*)*3);
        /*number of operands validation*/
        if(operandsCounter != operandsInOperator(opF)){
            if(operandsCounter < operandsInOperator(opF)) set_error_msg("expected operand");
            else set_error_msg("too many operands");
            char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
            memset(errorAux, ' ', ind);
            errorAux[ind] = '^';
            *errptr = errorAux;  
            return 0; 
        }                  
        for(int i = 0; i < operandsInOperator(opF); i++){             
            /*CHECK IF THE OPERAND TYPES ARE OK*/
            OperandType opType = operandsAnalyser(operands[i], alias_table, instr, errptr, 0, s);
            if(opType == 0) return 0;
            if (!(opType & opF->opd_types[i])) { // bitwise and
                EntryData *ed = stable_find(alias_table, operands[i]);
                if(opType == LABEL && ed != NULL){
                    if(!(ed->opd->type & opF->opd_types[i])){
                        set_error_msg("wrong operand type");
                        int comCount = 0;
                        for(unsigned int y = endOfFirstStr; y < strlen(s); y++){
                            if(!isspace(s[y]) && s[y] != ','){
                                if(comCount == i){
                                    ind = y;
                                    break;
                                }
                                else continue;
                            }
                            else if(s[y] == ',') comCount++;
                        }
                        char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                        memset(errorAux, ' ', ind);
                        errorAux[ind] = '^';
                        *errptr = errorAux;
                        return 0;
                    }
                    else{
                        if(ed->opd->type == REGISTER){
                            newOperands[i] = operand_create_register(ed->opd->value.reg);
                        }
                        else{
                            newOperands[i] = operand_create_number(ed->opd->value.num);
                        }
                    }
                }
                else if(opType == LABEL && ed == NULL && (opF->opd_types[i] & LABEL)){
                    newOperands[i] = operand_create_label(operands[i]);
                }
                else{
                    set_error_msg("wrong operand type");
                    int comCount = 0;
                    for(unsigned int y = endOfFirstStr; y < strlen(s); y++){
                        if(!isspace(s[y]) && s[y] != ','){
                            if(comCount == i){
                                ind = y;
                                break;
                            }
                            else continue;
                        }
                        else if(s[y] == ',') comCount++;
                    }
                    char *errorAux = (char*)emalloc(sizeof(char) * ind+1);
                    memset(errorAux, ' ', ind);
                    errorAux[ind] = '^';
                    *errptr = errorAux;  
                    return 0; 
                } 
            }
            else{
                if(opType == REGISTER){
                    if(operands[i][0] == '$'){
                        newOperands[i] = operand_create_register(number);
                    }
                    else{
                        EntryData *ed = stable_find(alias_table, operands[i]);
                        newOperands[i] = operand_create_register(ed->opd->value.reg);
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
        }
        if((*instr)->op == NULL){
            *instr = instr_create(NULL, opF, newOperands);
        }
        else{
            Instruction *new = *instr;
            while(new->next != NULL) new = new->next;
            new->next = instr_create(NULL, opF, newOperands);
        }
    }
    return 1;
}

// int main(){
//     SymbolTable st = stable_create();  
//     Instruction *new = (Instruction*)malloc(sizeof(Instruction));
//     const char *errptr = malloc(sizeof(char*));
//     parse("startf  MUL  $2, $3, 1 \n", st, &new, &errptr);
//     printf("startf  MUL  $2, $3, 1 \n");
//     printf("%s\n", errptr);
//     parse("    DIV $4, $2   \n", st, &new, &errptr);
//     printf("    DIV $4, $2   \n");
//     printf("%s\n", errptr);
//     // parse("   abc JNZ $4, loop   \n", st, &new, &errptr);
//     // printf("   abc JNZ $4, loop   \n");
//     // printf("%s\n", errptr);
//     parse("   c IS $85   \n", st, &new, &errptr);
//     printf("   c IS $85   \n");
//     printf("%s\n", errptr);
//     print_error_msg(NULL);
//     return 0;    
// }