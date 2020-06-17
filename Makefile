CFLAGS = -Wall -Wextra -pedantic

all: client server

client: client.cpp
	g++ $(CFLAGS) client.cpp -o client
server: server.cpp
	g++ $(CFLAGS) server.cpp -o server
clean:
	rm client server

