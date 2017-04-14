#
# Makefile
# Jerry Jia, 2017-04-10 14:57
#

PROTOC=protoc
GRPC_CPP_PLUGIN=`which grpc_cpp_plugin`
OUT_DIR=.
CXX=clang++
CXXFLAGS += -std=c++11
GOPATH:=${GOPATH}
#PROTOC_INCLUDE=-I. -I$(GOPATH)/src -I$(GOPATH)/src/github.com/grpc-ecosystem/grpc-gateway/third_party/googleapis

LDFLAGS += `pkg-config --libs grpc++` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -lprotobuf -lpthread -ldl
_PM_LIBS=../../build/src/libpbook.libs.a ../../build/src/libpbook.db.a -lsqlite3 -lboost_system
LDFLAGS += $(_PM_LIBS)

all: libpbook.libs.a password_service

libpbook.libs.a:
	cd ../../ && mkdir -p build && cd build && cmake -DBUILD_WEB_SERVICE=OFF .. && $(MAKE)

password_service: password_service.pb.o password_service.grpc.pb.o password_service.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) $(PROTOC_INCLUDE) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN) $<

%.pb.cc: %.proto
	$(PROTOC) $(PROTOC_INCLUDE) --cpp_out=. $<

clean:
	rm -v -f *.pb.cc *.pb.h
	rm -v -f *.o *.out
	rm -v -f password_service

# vim:ft=make
#
