HOST = $(shell hostname)

ifeq ($(HOST), gray-ubnetbook)

DEPLOY_LIB_DIR = /usr/lib
DEPLOY_HDR_DIR = /usr/include/drawstuff
CC = g++
OUT_DIR = .
OUT_LIB = libdstuff.a

else 
#ifeq ($(shell hostname), INSERT_HOSTNAME) 


#	else
$(error Unknown host: $(HOST))
#	endif
endif


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
 
OBJS = drawstuff.o 
OBJS += x11.o 
OBJ_LIST = $(addprefix $(OUT_DIR)/,$(notdir $(OBJS)))

#####  TARGETS  ###########
all : $(OUT_LIB)

.cpp.o: 
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $(OUT_DIR)/$(notdir $@) $<

$(OUT_LIB) : $(OBJS)
	ar -cq $(OUT_DIR)/$@ $(OBJ_LIST)

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/$(OUT_LIB)

deploy:
	if !(test -d $(DEPLOY_HDR_DIR)); then mkdir $(DEPLOY_HDR_DIR); fi
	cp $(OUT_DIR)/$(OUT_LIB) $(DEPLOY_LIB_DIR)
	cp *.h $(DEPLOY_HDR_DIR)
	cp -r textures/ $(DEPLOY_HDR_DIR)

