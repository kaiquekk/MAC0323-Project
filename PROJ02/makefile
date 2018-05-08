HEADERS = buffer.h
OBJECTS = center.c buffer.c
TARGET = center
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -O2 -Wextra

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) center.c -o center

clean:
	-rm -f $(TARGET)