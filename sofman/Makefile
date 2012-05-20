CC = g++
CFLAGS = -Wall -O2 -c
OUT_EXE = sofman

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

INC = 
LIBS = -lcole
LIBS += -lboost_date_time
LIBS += -lboost_thread

all : $(OUT_EXE)

.cpp.o:
	$(CC) $(CFLAGS) $(INC) -o $@ $<

$(OUT_EXE) : $(O_FILES)
	$(CC) -o $@ $(O_FILES) $(LIBS)

clean:
	rm -f *.o $(OUT_EXE)


