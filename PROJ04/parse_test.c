#include <math.h>
#include "parser.c"
#include "buffer.c"

// void printInstr(Instruction **instr){
//     if((*instr)->label != NULL) printf("label    = %s\n", (*instr)->label);
//     else printf("label    = n/a\n");
//     printf("operator = %s\n", (*instr)->op->name);
//     int i = 0;
//     while((*instr)->opds[i] != NULL){
//         i++;
//         if(i == 3) break;
//     }
//     switch(i){
//         case 0:
//             printf("operands = n/a\n");
//             break;
//         case 1:
//             switch((*instr)->opds[0]->type)
//     }
    
    
// }

int getLength(int x){
    int count = 1;
    while(x/10 > 0){
        x /= 10;
        count++;
    }
    return count;
}

int main(int argc, char *argv[]){
    set_prog_name(argv[0]);
    if(argc == 1){
        die("missing arguments");
    }
    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        die("cannot open file");
    }
    Buffer *buffer = buffer_create(sizeof(char));
    SymbolTable stable = stable_create();
    const char *errptr = malloc(sizeof(char*));
    Instruction *instr = (Instruction*)malloc(sizeof(Instruction));

    int con = 1, i = 0;
    while(1){      
        con = read_line(file, buffer);
        // printf("%s\n", (char*)buffer->data);     
        
        if(con == 0){
            break;
        }
        int j = parse((char*)buffer->data, stable, &instr, &errptr); 
        
        if(j != 0){
            Instruction *new = instr;
            while(new->next != NULL) new = new->next;
            new->lineno = i;
            new->pos = buffer->p-1;
            printf("line     = %s\n", (char*)buffer->data);
            // printInst(&new);
        }
        else{            
            printf("line %d: %s\n", i, (char*)buffer->data);
            int sz = 7 + getLength(i);
            printf("%*s%s\n", sz, " ", errptr);
            buffer_destroy(buffer);
            stable_destroy(stable);
            fclose(file);
            die(NULL);
        }
        i++;
    }
    buffer_destroy(buffer);
    stable_destroy(stable);
    fclose(file);
    return 0;
}