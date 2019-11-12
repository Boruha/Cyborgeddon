#deperecate, see new video with implementation with patsubst
SOURCES := $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))
LIBS 	:= -lIrrlicht
INCLUDE := -I/usr/include/irrlicht/

#TODO LIST:
#
#	1.-FOLDER NAVIGATION (mkdir -p and change wildcard to patsubst)
#	2.-MAKE CLEAN COMMAND (rm * obj)

#CREATE AN EXECUTABLE NAMED CYBORGEDDON LINKING ALL .O AND LIBS
#	CONOSOLE COMMAND: make
cyborgeddon: $(OBJECTS)
	g++ -o $@ $^ $(LIBS)

#TAKE ALL .CPP AND DO ALL .O IF DON'T EXIST 
#OR IF .CPP IS CHANGED
#	CONOSOLE COMMAND: make <filename>.o
%.o: %.cpp
	g++ -o $@ -c $^ $(INCLUDE)

#SHOW WHICH ELEMENTS ARE BEING INCLUDED IN THIS VARIABLES
#	CONOSOLE COMMAND: make info
info:
	$(info $(SOURCES))
	$(info $(OBJECTS))