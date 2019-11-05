#compiler setup

BEAGLEBONE_EXECUTABLE = gaming-pc

BEAGLEBONE_ENTRY_FILE = displayImage.cpp
BEAGLEBONE_H_LIBRARIES = multicast.h
OUTPUTDIR = $(HOME)/Git/gaming-host
CROSS_TOOL = arm-linux-gnueabihf-
COMPILE_TOOL = g++
CC_C = $(CROSS_TOOL)$(COMPILE_TOOL)
CFLAGS = -Wall -g -D _POSIX_C_SOURCE=200809L # -Werror
OPENCV_LIBRARIES = `pkg-config opencv --cflags --libs`


all :
	cp ../../gaming-libraries/$(BEAGLEBONE_H_LIBRARIES) .
	$(COMPILE_TOOL) $(CFLAGS) $(BEAGLEBONE_ENTRY_FILE) -o $(OUTPUTDIR)/$(BEAGLEBONE_EXECUTABLE) $(OPENCV_LIBRARIES)
	rm $(BEAGLEBONE_H_LIBRARIES)



clean :
	rm -f $(OUTPUTDIR)/$(BEAGLEBONE_EXECUTABLE)
