CXX = g++
NVCC = nvcc

OPTS=-lstdc++ -std=c++11
CPPFLAGS=$(shell pkg-config --cflags opencv)
LDFLAGS=$(shell pkg-config --libs opencv)
SRCS=minarea.cpp murmurhash3.cpp xxhash.c

ifeq ($(DEBUG), 1) 
OPTS+=-O0 -g
else
OPTS+=-O3
endif

%.o: %.cpp
	$(NVCC) $(OPTS) $(CPPFLAGS) -c $<

all: minarea 

minarea: minarea.o murmurhash3.o xxhash.o
	$(NVCC) minarea.o murmurhash3.o xxhash.o  -o sample_minarea $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf sample_minarea
