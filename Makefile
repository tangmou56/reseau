all : server.o client.o
	gcc -o client client.o
	gcc -o server server.o
server.o : server.c
	gcc -c server.c

client.o : client.c
	gcc -c client.c

clean:
	rm client
	rm server
	rm *.o

