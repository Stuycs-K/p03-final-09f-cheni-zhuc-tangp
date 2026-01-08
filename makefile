./PHONY: compile clean server client

compile: server client

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o -lncurses

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c -lncurses

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm -f *.o server client
