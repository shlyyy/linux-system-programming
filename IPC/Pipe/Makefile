
.PHONY:all
all:testPipe1 testPipe

testPipe1:testPipe1.cc
	g++ -o $@ $^ -std=c++11
testPipe:testPipe.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f testPipe testPipe1