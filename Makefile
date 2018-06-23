.PHONY: all clean cleanup

CFLAGS+=-g -Wall -Werror -Wunused-parameter -Wunused-variable

all: main.x

main.x: main.o
	${CC} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x
