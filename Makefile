ifndef CC
	CC = gcc
endif

CFLAGS = -Wall -Wextra -O3 -I.. -L..

all: ccard

ccard: ccard.c
	$(CC) $(CFLAGS) -Wc++-compat -o ccard ccard.c


clean:
	rm -rf ccard  *.o *.dSYM *.greg

.PHONY: all clean test
