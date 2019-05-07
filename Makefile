CC = gcc
CFLAGS = -Wall -std=c99

all:test

control_panel.o: control_panel.c control_panel.h
	$(CC) $(CFLAGS) -c control_panel.c

open_addressing.o: open_addressing.c control_panel.h
	$(CC) $(CFLAGS) -c open_addressing.c

dijkstra_alg.o: dijkstra_alg.c control_panel.h
	$(CC) $(CFLAGS) -c dijkstra_alg.c

bellman_ford_alg.o: bellman_ford_alg.c control_panel.h
	$(CC) $(CFLAGS) -c bellman_ford_alg.c

test: control_panel.o open_addressing.o dijkstra_alg.o bellman_ford_alg.o 
	$(CC) $(CFLAGS) -o test control_panel.o open_addressing.o dijkstra_alg.o bellman_ford_alg.o 

clean:
	rm *.o test
