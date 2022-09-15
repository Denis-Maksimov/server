CXX:=g++
CPP:=g++
CC:=g++

OBJ:= \
uhtml.o \
uhttp.o \
usrv.o \
./ctrlPage/serverCtrlPage.o \
ucli.o \
uservice.o 

SRV:=$(OBJ) \
test_srv.o 
CLI:=$(OBJ) \
test_cli.o 


UNAME:= $(shell uname)
ifeq ($(UNAME),MINGW64_NT-10.0-19042)
LDFLAGS:=  -lwinmm -lws2_32
endif
CPPFLAGS+= -g -std=c++20
.PHONY: clean all
all: test_srv test_cli

test_srv: $(SRV)
	$(CC) $(SRV) $(LDFLAGS) -o $@

test_cli: $(CLI)
	$(CC) $(CLI) $(LDFLAGS) -o $@

test_cli.o:
test_srv.o:
uhtml.o: 
uhttp.o:
usrv.o:
./ctrlPage/serverCtrlPage.o:
ucli.o:
uservice.o:

ifeq ($(UNAME),MINGW64_NT-10.0-19042)
clean:
	rm -f $(OBJ) main.exe
else
clean:
	rm -f $(SRV) $(CLI)
endif
