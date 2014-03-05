##############################################################################
#	Makefile.inc
#	Zac Hester
#	2011-08-17
##############################################################################

# Basic compile environment settings
CC = /usr/bin/gcc
CFLAGS = -Wall
LD = $(CC)
LDFLAGS = $(CFLAGS)
AR = /usr/bin/ar
ARFLAGS = -cr
SHELL = /usr/bin/sh

# Add debug symbols for any debug target.
debug: CFLAGS += -ggdb
debug: LDFLAGS += -ggdb

# Detect build environment.
UNAME := $(shell uname)
