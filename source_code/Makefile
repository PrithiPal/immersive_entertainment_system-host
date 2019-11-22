#compiler setup
# source-code

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

MOOD_SETTER_LIBRARIES = packetSender.cpp mode1.cpp mode2.cpp mode3.cpp sendAverageScreenshot.cpp sendDominantColorScreenshot.cpp rgbCombiner.cpp

all : menu

% : %.cpp
	cp ../../gaming-libraries/$(BEAGLEBONE_H_LIBRARIES) .
	$(COMPILE_TOOL) $(CFLAGS) $(MOOD_SETTER_LIBRARIES) -o $(OUTPUTDIR)/$@ $< $(OPENCV_LIBRARIES)  -lpthread
	rm $(BEAGLEBONE_H_LIBRARIES)


clean :
	rm -f $(OUTPUTDIR)/mode1_driver || true
	rm -f $(OUTPUTDIR)/mode2_driver || true
	rm -f $(OUTPUTDIR)/mode3_driver || true
	rm $(BEAGLEBONE_H_LIBRARIES) || true
