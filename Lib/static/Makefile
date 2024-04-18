lib=libmymath.a

$(lib):mymath.o
	ar -rc $@ $^
mymath.o:mymath.c
	gcc -c $^

.PHONY:clean
clean:
	rm -rf *.o *.a lib

.PHONY:output
output:
	mkdir -p lib/include
	mkdir -p lib/mymathlib
	cp *.h lib/include
	cp *.a lib/mymathlib