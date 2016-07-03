INC=-I.

LIBS=-L. -lnw_evtloop

VPATH=.

SRC=nw_evtloop.c main.c

OBJS=$(SRC:.c=.o)

OUTPUT=main libnw_evtloop.so libnw_evtloop.a

all:$(OUTPUT)
		
.SUFFIXES:.o .c .so .a

.c.o:
	gcc -fPIC $(INC) -g -c $^

main:main.o libnw_evtloop.a
	gcc -g $^ -o $@ -L.  -lnw_evtloop 

libnw_evtloop.so:nw_evtloop.o 
	gcc -fPIC -shared -g $^ -o $@

libnw_evtloop.a:nw_evtloop.o
		ar -rcs $@  $^

test:main
		./main
			
clean:
		rm -f *.o *.a *.so  $(OUTPUT) 
