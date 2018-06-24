.PHONY: all clean cleanup test

CFLAGS+=-g -Wall -Werror -Wunused-parameter -Wunused-variable

all: main.x

main.x: main.o
	${CC} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x
test: 
	valgrind --leak-check=full --track-origins=yes  ./main.x test.out test.out -f