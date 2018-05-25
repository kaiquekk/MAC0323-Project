HEADERS = parser.h stable.h buffer.h optable.h asmtypes.h opcodes.h mactypes.h error.h
OBJECTS = parse_test.c parser.c stable.c buffer.c optable.c asmtypes.c error.c
TARGET = parse_test
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -O2 -Wextra

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) parse_text.c -o parse_text

clean:
	-rm -f $(TARGET)