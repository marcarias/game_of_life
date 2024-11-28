CC = gcc
CFLAGS = -Wall -I/usr/include/ -I.
LDFLAGS = -lncurses
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< -lm -pg

conway: $(OBJ)
	gcc -o $@ $^  $(LDFLAGS) $(CFLAGS) -lm -pg

clean:
	rm -f $(OBJ) conway
