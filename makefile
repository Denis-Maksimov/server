CXX:=g++
CPP:=g++
CC:=g++

OBJ:=\
main.o \
uhtml.o \
uhttp.o \
usrv.o \
./ctrlPage/serverCtrlPage.o 
UNAME:= $(shell uname)
ifeq ($(UNAME),MINGW64_NT-10.0-19042)
LDFLAGS:=  -lwinmm -lws2_32
endif
CPPFLAGS+= -g 
.PHONY: clean

main: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

main.o:
uhtml.o: 
uhttp.o:
usrv.o:
./ctrlPage/serverCtrlPage.o:



ifeq ($(UNAME),MINGW64_NT-10.0-19042)
clean:
	rm -f $(OBJ) main.exe
else
clean:
	rm -f $(OBJ) main
endif
