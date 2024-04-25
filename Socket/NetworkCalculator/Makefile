.PHONY:all
all:calserver calclient

# Flag=#-DMySelf=1
# Lib=-ljsoncpp

calserver:CalServer.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp
calclient:CalClient.cc
	g++ -o $@ $^ -std=c++11 -g -ljsoncpp

# $(Lib) $(Flag)

.PHONY:clean
clean:
	rm -f calclient calserver