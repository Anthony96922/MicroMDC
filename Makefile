#
# Makefile for main program
#
CC = gcc
DEBUG ?= 0

MICROMDC_SRC = micromdc.c micromdc_extra.c libmdc.a

ifeq ($(DEBUG), 0)
	CFLAGS += -s
endif

all: micromdc

libmdc.a:
	$(MAKE) -C mdc-encode-decode -f ../Makefile.mdclib

micromdc: $(MICROMDC_SRC)
	$(CC) -o micromdc $(MICROMDC_SRC) $(CFLAGS)

clean:
	$(MAKE) -C mdc-encode-decode -f ../Makefile.mdclib clean
	rm -f micromdc
