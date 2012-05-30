CC = g++
OUT_EXE = _out_

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

HOST = $(shell hostname)

ifeq ($(HOST), gray-ubnetbook)
CFLAGS = -Wall -O2 -c
DEFINES = -DdDOUBLE
ifeq ($(BUILD_TYPE), debug)
        CFLAGS += -g
endif

else 
ifeq ($(shell hostname), e6500.lgltex.lgl) 
CFLAGS = -Wall -c -v
CFLAGS += -std=c++0x
ifeq ($(BUILD_TYPE), debug)
        CFLAGS += -g
endif

else
$(error Unknown host: $(HOST))
endif
endif

INC = 
LIBS = -lcole
LIBS += -lboost_date_time
ifeq ($(HOST), gray-ubnetbook)
LIBS += -lboost_thread

else 
ifeq ($(shell hostname), e6500.lgltex.lgl) 
LIBS += -lboost_thread-mt
endif
endif
LIBS += -lX11
LIBS += -lGL -lGLU -lglut
LIBS += -lm
LIBS += -lode

out : $(OUT_EXE)

%.o: %.cpp $(H_FILES)
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(OUT_EXE) : $(O_FILES)
	$(CC) -o $@ $(O_FILES) $(LIBS)

clean:
	rm -f *.o $(OUT_EXE)


