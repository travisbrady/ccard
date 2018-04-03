ifndef CC
	CC = gcc
endif

CFLAGS = -Wall -Wextra -march=native -O3 -I.

all: ccard

metrohash.o: metrohash.h metrohash64.c
	$(CC) $(CFLAGS) -c metrohash64.c -o metrohash.o

ccard: ccard.c metrohash.o
	$(CC) $(CFLAGS) -Wc++-compat metrohash.o -o ccard ccard.c

test: ccard
	./test.sh

clean:
	rm -rf ccard  *.o *.dSYM *.greg

.PHONY: all clean test
