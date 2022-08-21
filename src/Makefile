#!/usr/bin/make -f

SHELL = /bin/sh

topdir = ..
srcdir = $(topdir)/src

CC = clang
INSTALL = /usr/local/bin/install -c
INSTALLDATA = /usr/local/bin/install -c -m 644

LANG = c
STD = c17
CDEBUG = -g -DDEBUG
CFLAGS = $(CDEBUG) -Wall -pedantic -O2
DEFS = 
CPPFLAGS = -I. -I$(srcdir) -I$(topdir)/include $(DEFS)
LDFLAGS = 

LIBS = 

prefix = /usr/local
binprefix = 

bindir = $(prefix)/bin

SRCS_C = main.c
SRCS = $(SRCS_C)
OBJS = $(SRCS_C:.c=.o)

## add README to this
AUX = COPYING

.ONESHELL:

.EXPORT_ALL_VARIABLES:

.SUFFIXES:
.SUFFIXES: .c .o .h

.c.o:
	$(CC) -c -x$(LANG) -std=$(STD) $(CPPFLAGS) $(CFLAGS) $< -o $@

.PHONY: all
all: templ

.PHONY: install
install: all
	$(INSTALL) templ $(bindir)/$(binprefix)templ

.PHONY: TAGS tags
TAGS: $(SRCS)
	ctags $^

tags: TAGS

.PHONY: clean realclean distclean
clean:
	rm -f *.o

realclean: clean
	rm -f templ TAGS tags

distclean: realclean

templ: $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

## main.o: templ.o
