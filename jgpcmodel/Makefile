HOST = $(shell hostname)

ifeq ($(HOST), gray-ubnetbook)

CC = g++
OUT_EXE = robot
OUT_DIR = out
DEFINES = -DdDOUBLE
CFLAGS = -Wall -c
ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

else 
ifeq ($(shell hostname), e6500.lgltex.lgl) 
CC = g++
OUT_EXE = car
OUT_DIR = out
CFLAGS = -Wall -c
ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g
endif


else
$(error Unknown host: $(HOST))
endif
endif


BUILD_TYPE = release  # debug or release

INC = 
LIBS = -lode
ifeq ($(HOST), gray-ubnetbook)
LIBS += -ldstuff -lX11
else 
ifeq ($(shell hostname), e6500.lgltex.lgl) 
LIBS += -ldstuff 
endif
endif
LIBS += -lGL -lGLU -lglut
LIBS += -lpthread
# LIBS += -lboost_date_time

OBJS = onecar.o 
OBJS += solidobstacle.o 
OBJS += car.o 
OBJS += jPoly.o 
OBJS += OdeInsSensor.o 
OBJS += OdePathIns.o 
OBJS += OdePath.o 
OBJS += OdePathRadar.o 
OBJS += OdeRadarSensor.o 
OBJS += radar.o
OBJ_LIST = $(addprefix $(OUT_DIR)/,$(notdir $(OBJS)))

#####  TARGETS  ###########
all : $(OUT_EXE)

ck_dir:
	if !(test -d $(OUT_DIR)); then mkdir $(OUT_DIR); fi

.cpp.o: 
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -o $(OUT_DIR)/$(notdir $@) $<

$(OUT_EXE) : ck_dir $(OBJS)
	$(CC) -o $(OUT_DIR)/$@ $(OBJ_LIST) $(LIBS) $(DEFINES)

clean:
	rm -f *~ $(OUT_DIR)/*.o $(OUT_DIR)/$(OUT_EXE)


