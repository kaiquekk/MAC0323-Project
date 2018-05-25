#include <stdio.h>
#include "buffer.h"
#define size 256    /*standard initial size for data*/

Buffer *buffer_create(size_t member_size)
{
    Buffer *b = (Buffer*)malloc(sizeof(Buffer));
    if(!b) fprintf(stderr, "Memória Insuficiente.\n");
    b->buffer_size = size;
    b->member_size = member_size;    
    b->data = malloc(size * member_size + 1);
    b->p = 0;
    return b;
}
void buffer_destroy(Buffer *B)
{
    memset(B, 0, sizeof(*B));
    free(B); 
}
void buffer_reset(Buffer *B)
{            
    free(B->data);
    B->data = malloc(size * B->member_size + 1);
    B->p = 0;
}
void *buffer_push_back(Buffer *B)
{
    B->buffer_size *= 2;
    B->data = malloc(B->member_size * B->buffer_size + 1);
    return (size_t *) B->p; 
}
int read_line(FILE *input, Buffer *B)
{
    size_t count = 0;  /*char counter*/
    int ch;          /*char that will be read*/
    char *line;     /*line that will be read*/
    char *d;
    line = malloc(B->buffer_size);
    buffer_reset(B);
    while((ch = fgetc(input)) != EOF)
    {                
        if(count == B->buffer_size)
        {
            B->p = (size_t)buffer_push_back(B);
            memcpy(line, line, B->member_size * B->buffer_size + 1);
        }  
        if(ch == 10) 
        {      
            line[B->p++] = '\n';
            if(count == B->buffer_size)
            {
                B->p = (size_t)buffer_push_back(B);
                memcpy(line, line, B->member_size * B->buffer_size + 1);
            }  
            line[B->p++] = '\0';
            count++; 
            break;          
        } 
        line[B->p++] = ch;
        count++;
    }
    if(ch == EOF)
    {
        if(count == B->buffer_size)
        {
            B->p = (size_t)buffer_push_back(B);
            memcpy(line, line, B->member_size * B->buffer_size + 1);
        }   
        line[B->p++] = '\0';
    }    
    d = malloc(strlen (line) + 1);
    strcpy(d, line);
    B->data = d;
    memset(line, 0, B->buffer_size);  
    free(line);
    return (int) count;
}