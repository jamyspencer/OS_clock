# Written by Jamy Spencer 30 Jan 2017 
CC=gcc 
DEBUG_ARGS=-g -Wall
MAIN=master
SECONDARY=slave
OBJS1=main.o forkerlib.o slaveobj.o
OBJS2=slave.o slavelib.o
DEPS=forkerlib.h slavelib.h slaveobj.h

all: $(MAIN) $(SECONDARY)

%.o: %.c $(DEPS)
	$(CC) $(DEBUG_ARGS) -c $< -o $@

$(MAIN): $(OBJS1)
	$(CC) $(DEBUG_ARGS) -o $(MAIN) $(OBJS1)

$(SECONDARY): $(OBJS2) $(DEPS)
	$(CC) $(DEBUG_ARGS) -o $(SECONDARY) $(OBJS2)

clean :
	rm $(MAIN) $(SECONDARY) $(OBJS1) $(OBJS2) *.out
