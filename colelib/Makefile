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
OUT_DIR = out
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
 
OBJS = Angle.o
OBJS += Attitude.o
OBJS += CircularMessageQueue.o
OBJS += Config.o
OBJS += Constants.o
OBJS += Length.o
OBJS += Logger.o
OBJS += NorthBearingAngle.o
OBJS += ParseUtility.o
OBJS += Stoppable.o
OBJS += Velocity.o
OBJS += Velocity3D.o
OBJ_LIST = $(addprefix $(OUT_DIR)/,$(notdir $(OBJS)))

#####  TARGETS  ###########
all : chk_dir $(OUT_LIB)

chk_dir :
	if !(test -d $(OUT_DIR)); then mkdir $(OUT_DIR); fi

.cpp.o: 
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $(OUT_DIR)/$(notdir $@) $<

$(OUT_LIB) : $(OBJS)
	ar -cq $(OUT_DIR)/$@ $(OBJ_LIST)

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/$(OUT_LIB)

deploy:
	if !(test -d $(DEPLOY_HDR_DIR)); then mkdir $(DEPLOY_HDR_DIR); fi
	rm $(DEPLOY_HDR_DIR)/*.h
	cp $(OUT_DIR)/$(OUT_LIB) $(DEPLOY_LIB_DIR)
	cp *.h $(DEPLOY_HDR_DIR)

