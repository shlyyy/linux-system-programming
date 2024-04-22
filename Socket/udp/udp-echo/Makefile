.PHONY:all
all:udpserver udpclient

udpserver:UdpServer.cc
	g++ -o $@ $^ -std=c++11
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++11


.PHONY:clean
clean:
	rm -f udpserver udpclient