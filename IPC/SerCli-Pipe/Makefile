.PHONY:all
all:server client

server:server.cc
	g++ -o $@ $^ -g -std=c++11
client:client.cc
	g++ -o $@ $^ -g -std=c++11

.PHONY:clean
clean:
	rm -f server client