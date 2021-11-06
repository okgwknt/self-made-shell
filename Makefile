PROGRAM = oksh
OBJS = commands/fg.o commands/jobs.o get_path.o read_cmd.o signal.o child_process.o create_full_path.o execute.o
CC = gcc
CFLAGS = -O2

$(PROGRAM): $(PROGRAM).o $(OBJS)
	$(CC) -o $(PROGRAM) $(PROGRAM).o $(OBJS) -lm

$(OBJS) $(PROGRAM).o: header.h

clean:
	rm -f *.o commands/*.o