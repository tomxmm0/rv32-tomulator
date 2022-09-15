CC = gcc
CFLAGS = -g

default: main

main:	emulator.o dram.o cpu.o bus.o
		$(CC) $(CFLAGS) -o main main.c emulator.o dram.o cpu.o bus.o

emulator.o:	emulator.c emulator.h
		$(CC) $(CFLAGS) -c emulator.c

cpu.o: cpu.c cpu.h opcodes.h bus.h
		$(CC) $(CFLAGS) -c cpu.c

bus.o: bus.c bus.h dram.h
		$(CC) $(CFLAGS) -c bus.c

dram.o: dram.c dram.h
		$(CC) $(CFLAGS) -c dram.c

clean:
	$(RM) main *.o