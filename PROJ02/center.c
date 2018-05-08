#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "buffer.c"

int main(int argc, char *argv[])
{
    int argument, con, countL, first, flag;
    Buffer *buffer;
    FILE *file;
    FILE *output;
    if(argc==1)
    {
        perror("Missing arguments in command line.");
        return (-1);
    }      
    file = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    argument = atoi(argv[3]);
    buffer = buffer_create(argument);   
    con = 1;  
    countL = 0;  
    flag = 0;  /*flag for extra line control*/
    first = 1;
    if(file ==  NULL || output == NULL)
    {
        perror("Error in opening file.");
        return (-1);
    }   
    while(1)
    {
        int pointer1, pointer2, diff, i;
        char* line;
        char *d; /* strdup() pointer aux */
        con = read_line(file, buffer); /*reading the next line*/
        if(con == 0){
            if(flag != 1) fprintf(output, "\n");
            break;
        }
        countL++; /*incrementing the line counter*/
        pointer1 = 0; /*pointer to first position that is not a space */
        pointer2 = con; /*pointer to last position that is not a space */
        d = malloc(strlen (buffer->data) + 1); /*                                            */
        strcpy(d, buffer->data);               /*  strdup() implementation to avoid warnings */  
        line = d;                              /*                                            */        
        for (i = 0; i < con; i++)
        {
            if(isspace(line[i])) pointer1++;
            else break;
        }
        for (i = con-1; i >= 0; i--)
        {
            if(isspace(line[i])) pointer2--;
            else break;
        }  
        if(pointer2==0)
        {
           if(flag==0 && first!=1)
           {
               fprintf(output, "\n");
               flag=1;
           }
           continue;
        }
        else if(flag==0)
        {
            if(first!=1) fprintf(output, "\n");                        
        }
        else{
            fprintf(output, "\n");
            flag = 0;
        }
        first = 0;
        strncpy(line, line+pointer1, pointer2);
        diff = argument-(pointer2-pointer1);               
        /*if diff>0 so the output needs more spaces*/
        /*if diff<0 so the output has more spaces than the permitted*/
        if(line[strlen(line)-1] == '\n') 
        {                       
            line[strlen(line)-1] = 0;
        }
        if(diff == 0)
        {
            fprintf(output, "%s", line);
        }        
        else if(diff > 0)
        {            
            if(diff % 2 == 0)
            {
                for (i = 0; i < (diff/2); i++)
                {
                    fprintf(output, " ");
                }
            }
            else
            {
                for (i = 0; i < (diff/2) + 1; i++)
                {
                    fprintf(output, " ");
                }
            }
            fprintf(output, "%s", line);                     
        }
        else
        {
            fprintf(output, "%s", line);           
            fprintf(stderr, "center: %s: line %d: line too long.\n",  argv[1], countL);
        }                       
    }           
    buffer_destroy(buffer);
    fclose(file);
    fclose(output);
    return 0;
}