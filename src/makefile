PROJECT := simulator
CC = gcc
CFLAGS = -Wall
DEBUG = -ggdb3 -D DEBUG
LDFLAGS = -pthread

HEADERS := $(wildcard *.h)
CFILES := $(wildcard *.c)
OBJECTS := $(wildcard *.o)


all: library schedulerNotPreemptive schedulerPreemptive $(PROJECT)

library: librerie_dati_funz.c librerie_dati_funz.h
	$(CC) -c $(CFLAGS) $< 

schedulerNotPreemptive: not_preemptive.c not_preemptive.h
	$(CC) -c $(CFLAGS) $<
	
schedulerPreemptive: preemptive.c preemptive.h
	$(CC) -c $(CFLAGS) $<

$(PROJECT): MAIN.c
	$(CC) -c $(CFLAGS) $<
	$(CC) $(CFLAGS) $(LDFLAGS) $(wildcard *.o) -o $(PROJECT)
	
clean:
	@rm $(PROJECT)
	@rm $(OBJECTS)
