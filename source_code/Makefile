#compiler setup

BEAGLEBONE_EXECUTABLE = sendAverageScreenshot

BEAGLEBONE_H_LIBRARIES = multicast.h
ifdef jace
	OUTPUTDIR = $(HOME)/Git/gaming-host
else
	OUTPUTDIR = $(HOME)/cmpt433/public/myApp
endif
CROSS_TOOL = arm-linux-gnueabihf-
COMPILE_TOOL = g++
CC_C = $(CROSS_TOOL)$(COMPILE_TOOL)
CFLAGS = -Wall -g -D _POSIX_C_SOURCE=200809L -Werror
OPENCV_LIBRARIES = `pkg-config opencv --cflags --libs`


avg = sendAverageScreenshot

dominant = sendDominantColorScreenshot


all : $(dominant) $(avg)

% : %.cpp
	cp ../../gaming-libraries/$(BEAGLEBONE_H_LIBRARIES) .
	$(COMPILE_TOOL) $(CFLAGS) -o $(OUTPUTDIR)/$@ $< $(OPENCV_LIBRARIES)
	rm $(BEAGLEBONE_H_LIBRARIES)



clean :
	rm -f $(OUTPUTDIR)/$(avg) || true
	rm -f $(OUTPUTDIR)/$(dominant) || true
	rm $(BEAGLEBONE_H_LIBRARIES) || true
