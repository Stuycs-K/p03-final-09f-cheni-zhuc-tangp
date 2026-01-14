./PHONY: compile clean server client

compile: server client

server: server.o networking.o upload.o
	gcc -o server server.o networking.o upload.o

client: client.o networking.o
	gcc -o client client.o networking.o -lncurses

server.o: server.c networking.h upload.h 
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c -lncurses

upload.o: upload.c upload.h networking.h
	gcc -c upload.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm -f *.o server client
