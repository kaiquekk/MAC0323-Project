#include "stable.c"
#include <ctype.h>
#define MAX_WORDS_ON_TEXT 1000
#define MAX_WORD_LENGTH 100

int main(int argc, char *argv[]){
    FILE *file;
    SymbolTable st;
    InsertionResult ir;
    int index, flag, textIndex;
    char words[MAX_WORDS_ON_TEXT][MAX_WORD_LENGTH];
    flag = 0;
    textIndex = 0;
    if(argc==1)
    {
        perror("Missing arguments in command line.");
        return (-1);
    }      
    file = fopen(argv[1], "r");
    if(file ==  NULL)
    {
        perror("Error in opening file.");
        return (-1);
    }  
    st = stable_create();

    while(1){
        char ch = (char)fgetc(file);
        index = 0;
        while(!isblank(ch) && !feof(file) && ch != '\n'){	//read till  or EOF
            words[textIndex][index++] = ch;			//store in array            
            ch = (char)fgetc(file);
            flag = 1;
        }
        if(flag == 1){
            words[textIndex][index]=0;		//make last character of string null 
            ir = stable_insert(st, words[textIndex]);
            if(ir.new == 0){
                ir.data->i++;
            }
            else{
                ir.data->i = 1;                
            }
            flag = 0;
        }
        if(feof(file)){		//check again for EOF
            break;
        }
        textIndex++;
    }

    index = stable_visit(st, 0);   

    fclose(file);
    stable_destroy(st);
}