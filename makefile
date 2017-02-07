# Written by Jamy Spencer 30 Jan 2017 
CC=gcc 
DEBUG_ARGS=-g -Wall
MAIN=master
SECONDARY=slave
OBJS1=main.o forkerlib.o
OBJS2=slave.o slavelib.o
DEPS=forkerlib.h slavelib.h

all: $(MAIN) $(SECONDARY)

%.o: %.c 
	$(CC) -c $< -o $@

$(MAIN): $(OBJS1)
	$(CC) -o $(MAIN) $(OBJS1)

$(SECONDARY): $(OBJS2)
	$(CC) -o $(SECONDARY) $(OBJS2)

clean :
	rm $(MAIN) $(SECONDARY) $(OBJS1) $(OBJS2) *.out
