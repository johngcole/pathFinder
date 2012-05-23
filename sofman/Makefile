CC = g++
CFLAGS = -Wall -O2 -c
OUT_EXE = _out_

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

INC = 
LIBS = -lcole
LIBS += -lboost_date_time
LIBS += -lboost_thread

out : $(OUT_EXE)

%.o: %.cpp $(H_FILES)
	$(CC) $(CFLAGS) $(INC) -o $@ $<

$(OUT_EXE) : $(O_FILES)
	$(CC) -o $@ $(O_FILES) $(LIBS)

clean:
	rm -f *.o $(OUT_EXE)


