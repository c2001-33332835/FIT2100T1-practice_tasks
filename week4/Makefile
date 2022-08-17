OBJS = sourcefile1.o sourcefile2.o sourcefile3.o
CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

myprogram: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o myprogram

sourcefile1.o: sourcefile1.c headerfile1.h
	$(CC) $(CFLAGS) sourcefile1.c

sourcefile2.o: sourcefile2.c headerfile2.h
	$(CC) $(CFLAGS) sourcefile2.c

sourcefile3.o: sourcefile3.c headerfile3.h
	$(CC) $(CFLAGS) sourcefile3.c
