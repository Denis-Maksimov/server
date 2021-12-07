

CC:=g++

OBJ:=\
main.o \
uhtml.o \
uhttp.o \
usrv.o 

.PHONY: clean

main: $(OBJ)

main.o:
uhtml.o: 
uhttp.o:
usrv.o:

clean:
	rm -f $(OBJ) main