HOST = $(shell hostname)

ifeq ($(HOST), gray-ubnetbook)
	DEPLOY_LIB_DIR = /usr/lib
	DEPLOY_HDR_DIR = /usr/include/drawstuff
else ifeq ($(HOST), e6500.lgltex.lgl) 
	DEPLOY_LIB_DIR = /usr/lib64
	DEPLOY_HDR_DIR = /usr/include/drawstuff
else
	$(error Unknown host: $(HOST))
endif


CC = g++
OUT_LIB = libdstuff.a
BUILD_TYPE = release  # debug or release

CFLAGS = -Wall -c
ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
else
	CFLAGS += -O2
endif
DEFINES = -DPATH_TO_TEXTURES=$(DEPLOY_HDR_DIR)/textures/ 
LIBS =
INC =

H_FILES := $(wildcard *.h)
CPP_FILES := $(wildcard *.cpp)
O_FILES := $(CPP_FILES:.cpp=.o) 

#####  TARGETS  ###########
all : $(OUT_LIB)

%.o: %.cpp $(H_FILES)
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(OUT_LIB) : $(O_FILES)
	rm -f $(OUT_LIB)
	ar -cq $@ $(O_FILES)

clean:
	rm -rf *.o $(OUT_LIB)

deploy:
	@if !(test -d $(DEPLOY_HDR_DIR)); then mkdir $(DEPLOY_HDR_DIR); fi
	rm -rf $(DEPLOY_HDR_DIR)/* $(DEPLOY_LIB_DIR)/$(OUT_LIB)
	cp $(OUT_LIB) $(DEPLOY_LIB_DIR)
	cp *.h $(DEPLOY_HDR_DIR)
	cp -r textures/ $(DEPLOY_HDR_DIR)

