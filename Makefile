CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -Werror
OBJS = dijkstra.o node.o List.o

.PHONY: all clean

all: dijkstra

dijkstra: $(OBJS)
	$(CC) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o dijkstra
