#
#	MACROS
#

#FILES
APP 		:= cyborgeddon
ALLCCPS    	:= $(shell find src/ -type f -iname *.cpp)
ALLCCPOBJS 	:= $(patsubst %.cpp,%.o,$(ALLCCPS))

#FOLDERS
MKDIR 		:= mkdir -p
SRC 		:= src
OBJ 		:= obj
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

#HEADERS AND LIBRARIES
LIBS 		:= -lIrrlicht
INCLUDE 	:= -I/usr/include/irrlicht/

#COMPILER
CC 			:= g++
CCFLAGS 	:= -Wall -pedantic


.PHONY: info

#TODO LIST:
#
#	1.-FOLDER NAVIGATION (mkdir -p and change wildcard to patsubst)


#
#	LINKER
#

#CREATE AN EXECUTABLE NAMED CYBORGEDDON LINKING ALL .O AND LIBS
#	CONOSOLE COMMAND: make
$(APP) : $(OBJSUBDIRS) $(ALLCCPOBJS)
	$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCCPOBJS)) #$(LIBS)

#
#	COMPILER C++
#

#TAKE ALL .CPP AND DO ALL .O IF DON'T EXIST 
#OR IF .CPP IS CHANGED
#	CONOSOLE COMMAND: make <filename>.o
%.o : %.cpp
	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ #$(INCLUDE) $(CCFLAGS)

#
#	FOLDER STRUCTURE
#
$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)

#SHOW WHICH ELEMENTS ARE BEING INCLUDED IN THIS VARIABLES
#	CONOSOLE COMMAND: make info
info :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCCPS))
	$(info $(ALLCCPOBJS))

clean : 
	$(shell rm -r $(OBJ))
	$(shell rm $(APP))