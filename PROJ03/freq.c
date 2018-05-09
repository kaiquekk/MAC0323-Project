#include "stable.c"
#include <ctype.h>

char words[1000][100];
static int print(const char *key, EntryData *data){
    if(key != NULL && data != NULL){
        printf("%s\t\t%d\n", key, data->i);
        return 1;
    }
    else{
        return 0;
    }
}
int main(int argc, char *argv[]){
    FILE *file, *file2;
    SymbolTable st;
    InsertionResult ir;
    int index, flag, textIndex, max_length, max_words;
    max_length = 0;
    max_words = 0;
    flag = 0;
    textIndex = 0;
    if(argc==1)
    {
        perror("Missing arguments in command line.");
        return (-1);
    }   
    if(argc>2){
        perror("Too much arguments. Only one argument is needed.");
        return(-1);
    }
    file = fopen(argv[1], "r");//file for inserting on stable
    file2 = fopen(argv[1], "r");//file for counting words array indexes   
    if(file2 == NULL)
    {
        perror("Error in opening file.");
        return (-1);
    }  
    st = stable_create();

    /*while(1){//words array indexes counter block
        int i = 0;//auxiliary counter
        char ch = (char)fgetc(file2);
        while(!isblank(ch) && !feof(file2) && ch != '\n' && ch != '\t'){
            i++;     
            flag = 1;//setup flag to evade multiple blank spaces counting as a word
            ch = (char)fgetc(file2);
        }
        if(flag == 1){
            max_words++;
            if(i>max_length) max_length = i;  
            flag = 0;//reset the flag
        }
        if(feof(file2)){          
            break;
        }            
    }
    fclose(file2);  
    char words[max_words][max_length+1];*/
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
    index = stable_visit(st, print);   
    fclose(file);
    stable_destroy(st);
}