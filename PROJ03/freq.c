#include "stable.c"
#include <ctype.h>
#include <string.h>

const char **keys; //'array' of strings
int *vals; //'array' of vals
int index; //global counter for print function
unsigned int maxLength = 0; //maximum length of all the keys
//the Bubblesort recursive function
static void bubblesort(const char *keys[], int* vals, int n){
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
/*the get function to put all the keys and values into the respective arrays*/
static int get(const char *key, EntryData *data){
    if(strlen(key) > maxLength){
        maxLength = strlen(key);
    }
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
static void printSpace(int qtd){
    if(qtd < 0) return;
    printf(" ");
    printSpace(qtd-1);
}
int main(int argc, char *argv[]){
    FILE *file;
    SymbolTable st;
    InsertionResult ir;
    char *aux;
    int index, flag, textIndex;
    flag = 0;
    textIndex = 0;
    aux = (char*)malloc(sizeof(char));//auxiliary pointer
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
            aux[index++] = ch;//store in auxiliary char array   
            ch = (char)fgetc(file);
            flag = 1;//setup flag to evade multiple blank spaces counting as a word
        }
        if(flag == 1){
            aux[index++] = 0; //make last character of string null
            char *tmp =  malloc(sizeof(char)*index);//alloc memory for the new word read
            strcpy(tmp, aux);//copy the content of the auxiliary pointer to the new allocated memory
            ir = stable_insert(st, tmp);//insert the new word in the symbol table
            if(ir.new == 0){//if it is not a new entry on the stable
                ir.data->i++;//increment the value of the given key by 1
            }
            else{//if it is a new entry on the stable
                ir.data->i = 1;//set the value of the given key to 1            
            }
            flag = 0;//reset the flag
        }
        if(feof(file)){//check again for EOF
            break;
        }
        textIndex++;
    }    
    free(aux);//free the auxiliary char pointer
    keys = malloc(sizeof(char**) * (st->n+1));
    vals = malloc(sizeof(int) * (st->n+1));
    index = 0;
    index = stable_visit(st, get); 
    bubblesort(keys, vals, st->n);
    for (int i = 0; i < st->n; i++){
        printf("%s", keys[i]);
        printSpace(maxLength - strlen(keys[i]));
        printf("%d\n", vals[i]);
    }
    fclose(file);
    stable_destroy(st);
}