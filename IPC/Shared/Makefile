.PHONY:all
all:processa processb

processa:processa.cc
	g++ -o $@ $^ -g -std=c++11
processb:processb.cc
	g++ -o $@ $^ -g -std=c++11

.PHONY:clean
clean:
	rm -f processa processb