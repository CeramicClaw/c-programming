CC=gcc
CFLAGS=-Wall -Werror -ggdb3 -std=gnu99 -pedantic $(OTHERFLAGS)
DEPS=cards.o  deck.o  eval.o  future.o  input.o  main.o

poker: $(DEPS)
	$(CC) -o poker $(DEPS)
cards.o:
	$(CC) $(CFLAGS) -c cards.c cards.h
deck.o:
	$(CC) $(CFLAGS) -c deck.c deck.h
eval.o:
	$(CC) $(CFLAGS) -c eval.c eval.h
future.o:
	$(CC) $(CFLAGS) -c future.c future.h
input.o:
	$(CC) $(CFLAGS) -c input.c input.h
main.o:
	$(CC) $(CFLAGS) -c main.c future.c input.c deck.c eval.c cards.c
clean:
	rm -rf *.o *~ *.gch
