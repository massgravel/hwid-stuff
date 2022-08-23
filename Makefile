GCCOPT=-O0 -Wall -pedantic
GCC=gcc

all: hwid_dec

hwid_dec: hwid_dec.c
	$(GCC) $(GCCOPT) $< -o $@