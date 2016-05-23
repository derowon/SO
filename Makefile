all: client server

CC = gcc
CFLAGS = -lrt -pthread -l sqlite3
COBJECTS = fifos.o parser.o client.o
SOBJECTS = server.o fifos.o dataBaseConnection.o sqlite.o

client: $(COBJECTS)
	$(CC) $(COBJECTS) -o client.exe

server: $(SOBJECTS)
	$(CC) $(SOBJECTS) $(CFLAGS) -o server.exe

%.o: %.c
	$(CC) -c $<

clean:
	-rm -f *.o
	-rm -f *.exe