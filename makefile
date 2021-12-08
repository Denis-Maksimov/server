CXX:=x86_64-w64-mingw32-g++
CPP:=x86_64-w64-mingw32-g++
CC:=x86_64-w64-mingw32-g++

CXX:=g++
CPP:=g++
CC:=g++

OBJ:=\
main.o \
uhtml.o \
uhttp.o \
usrv.o 
# LDFLAGS:=  -lwinmm -lws2_32
CPPFLAGS+= -g 
.PHONY: clean

main: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

main.o:
uhtml.o: 
uhttp.o:
usrv.o:

clean:
	rm -f $(OBJ) main