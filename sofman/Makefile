CC = g++
OUT_EXE = _out_

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

HOST = $(shell hostname)
CFLAGS = -Wall -c
ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
endif
DEFINES = 
INC = -I../
LIB_INC = -L../colelib/
LIBS = -lcole
LIBS += -lboost_date_time
LIBS += -lX11
LIBS += -lGL -lGLU -lglut
LIBS += -lm
LIBS += -lode

ifeq ($(HOST), netbian)
	CFLAGS += -O2
	DEFINES += -DdDOUBLE
	LIBS += -lboost_thread
else ifeq ($(shell hostname), e6500.lgltex.lgl) 
	CFLAGS += -v
	CFLAGS += -std=c++0x
	LIBS += -lboost_thread-mt
else ifeq ($(shell hostname), cole-XPS-linux) 
	CFLAGS += -v
	DEFINES += -DdDOUBLE
	CFLAGS += -std=c++0x
	LIBS += -lboost_thread
else
$(error Unknown host: $(HOST))
endif

out : $(OUT_EXE)

%.o: %.cpp $(H_FILES)
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(OUT_EXE) : $(O_FILES)
	$(CC) -o $@ $(O_FILES) $(LIB_INC) $(LIBS)

clean:
	rm -f *.o $(OUT_EXE)


