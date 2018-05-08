HEADERS = stable.h
OBJECTS = freq.c stable.c
TARGET = freq
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -O2 -Wextra

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) freq.c -o freq

clean:
	-rm -f $(TARGET)