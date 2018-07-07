.PHONY: all clean cleanup test
CC = gcc
CFLAGS+=-g -Wall -Werror -Wunused-parameter -Wunused-variable

all: main.x

main.x: main.o parser/parser.o angleDescriptor/angleDescriptor.o Distance/generateNGrams.o
	${CC} -o $@ $^ -lm

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $^
clean:
	${RM} *.o parser/*.csv

cleanup: clean
	${RM} *.x
test: main.x
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main.x test.test -f