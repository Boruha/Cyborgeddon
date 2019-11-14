#
#	MACROS
#

#COMPILER
CC 			:= g++
CCFLAGS 	:= -Wall -pedantic

#FOLDERS
MKDIR 		:= mkdir -p
SRC 		:= src
OBJ 		:= obj
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

#FILES
APP 		:= cyborgeddon
ALLCCPS    	:= $(shell find $(SRC)/ -type f -iname *.cpp)
ALLCCPOBJS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(patsubst %.cpp,%.o,$(ALLCCPS)))

#HEADERS AND LIBRARIES
LIBS 		:= -lIrrlicht
INCLUDE 	:= -I/usr/include/irrlicht/

.PHONY: info clean 


#
#	DEFINED FUNCTIONS
#
define COMPILECFILE 
	$(CC) -o $(2) -c $(1) $(CCFLAGS)
endef

#========================================================================
#	LINKER

#CREATE AN EXECUTABLE NAMED CYBORGEDDON LINKING ALL .O AND LIBS
#	CONOSOLE COMMAND: make
$(APP) : $(ALLCCPOBJS)
	$(CC) -o $(APP) $(ALLCCPOBJS)

#========================================================================
#	COMPILER C++

#TAKE ALL .CPP AND DO ALL .O IF DON'T EXIST OR IF .CPP IS CHANGED
$(ALLCCPOBJS) : $(ALLCCPS) $(OBJSUBDIRS)
	$(foreach CCP_FILE,$<,$(call COMPILECFILE,$(CCP_FILE), $(subst $(SRC),$(OBJ),$(subst .cpp,.o,$(CCP_FILE)))))

dir : $(OBJSUBDIRS) $(ALLCCPS)
	$(info $*)

#========================================================================
#	FOLDER STRUCTURE

$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)

#========================================================================
#	OTHER COMMANDS

#SHOW WHICH ELEMENTS ARE BEING INCLUDED IN THIS VARIABLES
#	CONOSOLE COMMAND: make info
info :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCCPS))
	$(info $(ALLCCPOBJS))
	$(info $(foreach CCP_FILE,$(ALLCCPS),$(call COMPILECFILE,$(CCP_FILE),$(subst $(SRC),$(OBJ),$(subst .cpp,.o,$(CCP_FILE))))))

#DELETE ALL COMPILED FILES
#	CONOSOLE COMMAND: make clean
clean : 
	$(shell rm -r $(OBJ))
	$(shell rm $(APP))