.PHONY:all
all:main contacts.pb.cc

contacts.pb.cc:contacts.proto
	protoc contacts.proto --cpp_out=.

main:main.cc contacts.pb.cc
	g++ main.cc contacts.pb.cc -o main -lprotobuf

.PHONY:clean
clean:
	rm -r main contacts.pb*