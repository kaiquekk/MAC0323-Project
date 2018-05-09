#include "stable.c"
#include <ctype.h>
#include <string.h>
#define MAX_WORDS 2000
#define MAX_LENGTH 200
const char **keys;
int *vals;
int index;
// A function to implement bubble sort
void bubblesort(const char *keys[], int* vals, int n){
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n-1; j++) {
            if(strcmp(keys[j], keys[j+1]) > 0){
                int aux = vals[j];
                vals[j] = vals[j+1];
                vals[j+1] = aux;
                const char *tmp = keys[j];
                keys[j] = keys[j+1];
                keys[j+1] = tmp;
            }
        }
    }
}
static int print(const char *key, EntryData *data){
    if(key != NULL && data != NULL){
        keys[index] = key;
        vals[index] = data->i;
        index++;
        return 1;
    }
    else{
        return 0;
    }
}

int main(int argc, char *argv[]){
    FILE *file;
    SymbolTable st;
    InsertionResult ir;
    char words[MAX_WORDS][MAX_LENGTH];
    int index, flag, textIndex;
    flag = 0;
    textIndex = 0;
    if(argc==1){
        perror("Missing arguments in command line.");
        return (-1);
    }   
    if(argc>2){
        perror("Too much arguments. Only one argument is needed.");
        return(-1);
    }
    file = fopen(argv[1], "r");//file for inserting on stable  
    if(file == NULL){
        perror("Error in opening file.");
        return (-1);
    }  
    st = stable_create();
    while(1){
        char ch = (char)fgetc(file);
        index = 0;
        while(!isblank(ch) && !feof(file) && ch != '\n' && ch != '\t'){
            //read until blank space, new line, tab or EOF
            words[textIndex][index++] = ch;//store in array            
            ch = (char)fgetc(file);
            flag = 1;//setup flag to evade multiple blank spaces counting as a word
        }
        if(flag == 1){
            words[textIndex][index]=0; //make last character of string null 
            ir = stable_insert(st, words[textIndex]);
            if(ir.new == 0){//if it is not a new entry on the stable
                ir.data->i++;
            }
            else{//if it is a new entry on the stable
                ir.data->i = 1;                
            }
            flag = 0;//reset the flag
        }
        if(feof(file)){//check again for EOF
            break;
        }
        textIndex++;
    }    
    keys = malloc(sizeof(char**) * (st->n+1));
    vals = malloc(sizeof(int) * (st->n+1));
    index = 0;
    index = stable_visit(st, print); 
    bubblesort(keys, vals, st->n);
    for (int i = 0; i < st->n; i++){
        printf("%-30s %4d\n", keys[i], vals[i]);
    }
    fclose(file);
    stable_destroy(st);
}