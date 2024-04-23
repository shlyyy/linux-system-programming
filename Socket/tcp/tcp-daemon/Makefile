.PHONY:all
all:tcpserverd tcpclient

tcpserverd:TcpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcpclient:TcpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread


.PHONY:clean
clean:
	rm -f tcpserverd tcpclient