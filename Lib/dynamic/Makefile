dy-lib=libmymethod.so
static-lib=libmymath.a

.PHONY:all
all: $(dy-lib) $(static-lib)

$(static-lib):mymath.o
	ar -rc $@ $^
mymath.o:mymath.c
	gcc -c $^

$(dy-lib):mylog.o myprint.o
	gcc -shared -o $@ $^
mylog.o:mylog.c
	gcc -fPIC -c $^
myprint.o:myprint.c
	gcc -fPIC -c $^

.PHONY:clean
clean:
	rm -rf *.o *.a *.so mylib

.PHONY:output
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib
	cp *.h mylib/include
	cp *.a mylib/lib
	cp *.so mylib/lib