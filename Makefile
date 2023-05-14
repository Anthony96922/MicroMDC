#
# Makefile for main program
#
CC = gcc
DEBUG ?= 0
CFLAGS = -Wall -Wextra -pedantic
LIBS =
WAVEOUT = 1

MICROMDC_SRC = micromdc.c micromdc_extra.c libmdc.a c_inc.c
MICROMDC_INC = micromdc_common.h micromdc_extra.h

ifeq ($(DEBUG), 0)
	CFLAGS += -s
endif

ifeq ($(WAVEOUT), 1)
	CFLAGS += -DWAVEOUT
	LIBS += -lsndfile
endif

all: clean micromdc

libmdc.a:
	$(MAKE) -C mdc-encode-decode -f ../Makefile.mdclib

micromdc: $(MICROMDC_SRC) $(MICROMDC_INC)
	$(CC) -o micromdc $(MICROMDC_SRC) $(CFLAGS) $(LIBS)

clean:
	$(MAKE) -C mdc-encode-decode -f ../Makefile.mdclib clean
	rm -f micromdc
