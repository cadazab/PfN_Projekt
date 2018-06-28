.PHONY: all clean cleanup test

CFLAGS+=-g -Wall -Werror -Wunused-parameter -Wunused-variable

all: main.x

main.x: main.o parser/parser.o
	${CC} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x
test: 
	valgrind --leak-check=full --track-origins=yes  ./main.x test.test test.test -f