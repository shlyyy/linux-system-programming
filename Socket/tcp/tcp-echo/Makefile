.PHONY:all
all:tcpserver tcpclient

tcpserver:TcpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcpclient:TcpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread


.PHONY:clean
clean:
	rm -f tcpserver tcpclient