TARGET=all.out
OBJS=Karaoke_rest.cpp
TOPDIR=${PWD}

CFLAGS=-I${TOPDIR}/inc

all:$(TARGET)

$(TARGET):$(OBJS)
	g++ $^ -o $@ -lpthread

%.o:src/%.c

test.o:test.c

clean:
	rm -rf ${OBJS} ${TARGETS}
