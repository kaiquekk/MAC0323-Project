#include "buffer.h"

Buffer *buffer_create(size_t member_size)
{
    Buffer *b = (Buffer*)malloc(sizeof(Buffer));
    if(!b) fprintf(stderr, "Memória Insuficiente.\n");
    b->buffer_size = 0;
    b->member_size = member_size;
    b->p = 5;
    b->data = NULL;
    return b;
}

void buffer_destroy(Buffer *B)
{
    free(B); /*Free mas nao tao nulo.*/
}

void buffer_reset(Buffer *B)
{
    buffer_destroy(B);
    B = buffer_create(1);
}

/*
  Return a valid pointer to the first free position of the
  buffer. This means that, if the space allocated is not enough, then
  the buffer size is increased and the contents are copied.
*/
void *buffer_push_back(Buffer *B)
{
       
}

/*
  Read a line (i.e., reads up to a newline '\n' character or the
  end-of-file) from the input file and places it into the given
  buffer, including the newline character if it is present. The buffer
  is resetted before the line is read.
  Returns the number of characters read; in particular, returns ZERO
  if end-of-file is reached before any characters are read.
*/
int read_line(FILE *input, Buffer *B)
{
    buffer_reset(B);
    int count;
    char c;
    while(c != EOF)
    {
        
    }

    return count;
}

int main()
{
    Buffer *a2 = buffer_create(10);
    printf("P: %d\n", a2->p);
    buffer_reset(a2);
    printf("P2: %d\n", a2->p);
    a2->p = 12;
    a2->member_size = 15;
    a2->buffer_size = 15;
    int *x = buffer_push_back(a2);
    printf("x = %d\n", x);
    buffer_destroy(a2);

}