CC=gcc
CFLAGS=-Wall -Wextra -fPIC -std=c2x
WRAP_FLAGS=-Wl,--wrap=fred -Wl,--wrap=john
OBJS=fred.o john.o bill.o sam.o

all: list_interpose

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

list_interpose: solution.c $(OBJS)
	$(CC) $(CFLAGS) $(WRAP_FLAGS) -o $@ solution.c $(OBJS) -lm

clean:
	rm -f *.o list_interpose