#compiler setup


BEAGLEBONE_H_LIBRARIES = multicast.h
ifdef jace
	OUTPUTDIR = $(HOME)/Git/gaming-host/source_code
else
	OUTPUTDIR = $(HOME)/cmpt433/public/myApp
endif
CROSS_TOOL = arm-linux-gnueabihf-
COMPILE_TOOL = g++
CC_C = $(CROSS_TOOL)$(COMPILE_TOOL)
CFLAGS = -Wall -g -D _POSIX_C_SOURCE=200809L -Werror
OPENCV_LIBRARIES =  -lX11 `pkg-config --cflags opencv` `pkg-config --libs opencv`


all : moodSetter

% : %.cpp
	cp ../../gaming-libraries/$(BEAGLEBONE_H_LIBRARIES) .
	$(COMPILE_TOOL) $(CFLAGS) -o $(OUTPUTDIR)/$@ $< $(OPENCV_LIBRARIES)
	rm $(BEAGLEBONE_H_LIBRARIES)


clean :
	rm -f $(OUTPUTDIR)/moodSetter || true
	rm $(BEAGLEBONE_H_LIBRARIES) || true
