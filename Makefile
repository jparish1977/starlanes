#
# Makefile for Starlanes
#
CC=/usr/bin/gcc
CCOPTS=-Wall
CURSESLIB=ncurses
INSTALLDIR=/usr/local

all: starlanes

ai.o: ai.c common.h
	$(CC) -g $(CCOPTS) -c ai.c

starlanes.o: starlanes.c ai.c common.h
	$(CC) -g $(CCOPTS) -c starlanes.c

starlanes: starlanes.o ai.o
	$(CC) -g starlanes.o ai.o -o starlanes -l$(CURSESLIB)

install:
	cp starlanes $(INSTALLDIR)/bin
	cp starlanes.6 $(INSTALLDIR)/man/man6/
	chown bin:bin $(INSTALLDIR)/bin/starlanes
	chown root:root $(INSTALLDIR)/man/man6/starlanes.6
	chmod 755 $(INSTALLDIR)/bin/starlanes
	chmod 444 $(INSTALLDIR)/man/man6/starlanes.6

# fakeinstall just echos the install commands:
fakeinstall:
	@echo cp starlanes $(INSTALLDIR)/bin
	@echo cp starlanes.6 $(INSTALLDIR)/man/man6
	@echo chown bin:bin $(INSTALLDIR)/bin/starlanes
	@echo chown root:root $(INSTALLDIR)/man/man6/starlanes.6
	@echo chmod 755 $(INSTALLDIR)/bin/starlanes
	@echo chmod 444 $(INSTALLDIR)/man/man6/starlanes.6





