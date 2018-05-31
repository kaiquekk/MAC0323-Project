#include "parser.c"
#include "buffer.c"

void printInstr(Instruction **instr){//print the instruction
    if((*instr)->label != NULL) printf("label    = %s\n", (*instr)->label);
    else printf("label    = n/a\n");
    printf("operator = %s\n", (*instr)->op->name);
    int i = 0;
    while((*instr)->opds[i] != NULL){
        i++;
        if(i == 3) break;
    }
    // printf("%d  %d\n", (*instr)->opds[0]->type, (*instr)->opds[1]->type);
    switch(i){
        case 0:          
            printf("operands = n/a\n\n");
            break;
        case 1:
            printf("operands = ");
            switch((*instr)->opds[0]->type){
                case REGISTER:
                    printf("Register(%d)\n\n", (*instr)->opds[0]->value.reg);
                    break;
                case LABEL:
                    printf("Label(\"%s\")\n\n", (*instr)->opds[0]->value.label);
                    break;
                case NUMBER_TYPE:
                    printf("Number(%lld)\n\n", (*instr)->opds[0]->value.num);
                    break;
                case STRING:
                    printf("String(\"%s\")\n\n", (*instr)->opds[0]->value.str);
                    break;
            }
            break;
        case 2:
            printf("operands = ");
            for(int j = 0; j < 2; j++){
                switch((*instr)->opds[j]->type){
                case REGISTER:
                    printf("Register(%d)", (*instr)->opds[j]->value.reg);
                    if(j==1) printf("\n\n");
                    else printf(", ");
                    break;
                case LABEL:
                    printf("Label(\"%s\")", (*instr)->opds[j]->value.label);
                    if(j==1) printf("\n\n");
                    else printf(", ");
                    break;
                case NUMBER_TYPE:
                    printf("Number(%lld)", (*instr)->opds[j]->value.num);
                    if(j==1) printf("\n\n");
                    else printf(", ");
                    break;
                case STRING:
                    printf("String(\"%s\")", (*instr)->opds[j]->value.str);
                    if(j==1) printf("\n");
                    else printf(", ");
                    break;
                }
            }
            break;
        case 3:
            printf("operands = ");
            for(int j = 0; j < 3; j++){
                switch((*instr)->opds[j]->type){
                case REGISTER:
                    printf("Register(%d)", (*instr)->opds[j]->value.reg);
                    if(j==2) printf("\n\n");
                    else printf(", ");
                    break;
                case LABEL:
                    printf("Label(\"%s\")", (*instr)->opds[j]->value.label);
                    if(j==2) printf("\n\n");
                    else printf(", ");
                    break;
                case NUMBER_TYPE:
                    printf("Number(%lld)", (*instr)->opds[j]->value.num);
                    if(j==2) printf("\n\n");
                    else printf(", ");
                    break;
                case STRING:
                    printf("String(\"%s\")", (*instr)->opds[j]->value.str);
                    if(j==2) printf("\n\n");
                    else printf(", ");
                    break;
                }
            }
            break;
    }    
}

int getLength(int x){
    int count = 1;
    while(x/10 > 0){
        x /= 10;
        count++;
    }
    return count;
}

//errors to be fixed:
//NUMBER operands losing one digit eg. 10 -> 1, 100 -> 10, etc  
//a IS $3 --- invalid register with errptr to 3 and lots of thrash after the ^
//a IS 3 --- expected operand no errptr
//STR teste --- unknown register with errptr to S and lots of thrash after the ^
//STR "teste" --- invalid string with an strange symbol on the first quote
//STR 'teste' --- unknown register with errptr to S and lots of thrash after the ^
//calls to labels not working. Most likely errors on the stable manipulation.
////JZ  $1, loop --- unknown register with errptr to J 
//NOP --- SegFault  ---- opds[]->type = random number when should be 0
////maybe problems with operators without 3 operands
//////loop PUSH $4 --- invalid register with errptr to 4
//////PUSH $4 --- invalid register with errptr to 4 and lots of trash after the ^
//probably code errors on lines without a label

//observations:
//INT doesn't work properly. Need to add a special checking too.
//client also prints the comments on the "line   =", unlike the ONLY avaliable example.
////most likely this happens because the full string analysis is being done on the parser, not with a part in the client
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
        if(con == 0){
            break;
        }
        int j = parse((char*)buffer->data, stable, &instr, &errptr); 
        
        if(j != 0){
            Instruction *new = instr;
            while(new->next != NULL) new = new->next;
            new->lineno = i;
            new->pos = buffer->p-1;
            char *line = (char*)buffer->data;
            printf("line     = %s", line);
            int tam = strlen(line);
            if(line[tam-1] != '\n') printf("\n");
            printInstr(&new);
        }
        else{            
            printf("line %d: %s\n", i+1, (char*)buffer->data);
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