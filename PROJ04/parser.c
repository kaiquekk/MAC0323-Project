#include <stdio.h>
/*This function split the line, creating a bidimensional array with the strings
that do not have blank spaces*/
static **char split(){
    char **words = malloc(sizeof(char**));
    char *str = "Teste     com    split.";
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
}
int parse(const char *s, SymbolTable alias_table, Instruction **instr,
          const char **errptr){
    
}
int main(){
    
}