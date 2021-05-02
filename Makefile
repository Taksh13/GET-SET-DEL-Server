CFLAGS = -Wall -g -std=gnu99 -O0
OBJS = hash.o table.o server.o

main: $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c -o server -lm

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *~ *.o main
