HOST = $(shell hostname)

ifeq ($(HOST), gray-ubnetbook)

DEPLOY_LIB_DIR = /usr/lib
DEPLOY_HDR_DIR = /usr/include/colelib

else 
ifeq ($(shell hostname), e6500.lgltex.lgl) 

DEPLOY_LIB_DIR = /usr/lib64
DEPLOY_HDR_DIR = /usr/include/colelib

else
$(error Unknown host: $(HOST))
endif
endif

CC = g++
OUT_LIB = libcole.a

BUILD_TYPE = release  # debug or release

CFLAGS = -Wall -c
ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
else
	CFLAGS += -O2
endif
DEFINES =
LIBS =
INC =

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

#####  TARGETS  ###########
out : $(OUT_LIB)

%.o: %.cpp $(H_FILES)
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(OUT_LIB) : $(O_FILES)
	rm -f $(OUT_LIB)
	ar -cq $@ $(O_FILES)

clean:
	rm -rf *.o $(OUT_LIB)

deploy:
	@if !(test -d $(DEPLOY_HDR_DIR)); then mkdir $(DEPLOY_HDR_DIR); fi
	rm -f $(DEPLOY_HDR_DIR)/*.h $(DEPLOY_LIB_DIR)/$(OUT_LIB)
	cp $(OUT_LIB) $(DEPLOY_LIB_DIR)
	cp $(H_FILES) $(DEPLOY_HDR_DIR)

