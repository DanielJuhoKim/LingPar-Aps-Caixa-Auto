CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = caixa_auto

all: $(TARGET)

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

$(TARGET): lex.yy.c parser.tab.c
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lm

clean:
	rm -f $(TARGET) lex.yy.c parser.tab.c parser.tab.h parser.output

test: $(TARGET)
	./$(TARGET) exemplo.jota

.PHONY: all clean test