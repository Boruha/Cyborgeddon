#
#	MACROS
#

#COMPILER
CC          := g++
CCFLAGS     := -Wall -pedantic
RELEASEFLAG := -O3
DEBUGFLAG   := -g

#DIRS
APPDIR      :=
RELEASEDIR  := Release/
DEBUGDIR    := Debug/

#DEBUG OR RELEASE
ifeq (${debug}, true)
    CCFLAGS += $(DEBUGFLAG)
    APPDIR  := $(DEBUGDIR)
else
    CCFLAGS += $(RELEASEFLAG)
    APPDIR  := $(RELEASEDIR)
endif

#FOLDERS
MKDIR 		:= mkdir -p
SRC 		:= src
OBJ 		:= obj
OBJDIR      := $(APPDIR)$(OBJ)
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJDIR)%,$(SUBDIRS))

#FILES
APP 		:= cyborgeddon
ALLCCPS    	:= $(shell find $(SRC)/ -type f -iname *.cpp)
ALLCCPOBJS 	:= $(patsubst $(SRC)%,$(OBJDIR)%,$(patsubst %.cpp,%.o,$(ALLCCPS)))

#HEADERS AND LIBRARIES
LIBS 		:= -lIrrlicht
INCLUDE 	:= -I/usr/include/irrlicht/ -I.

.PHONY: info clean 

#
#	DEFINED FUNCTIONS
#
define COMPILECFILE 
$(CC) -o $(2) -c $(1) $(INCLUDE) $(CCFLAGS)
endef

define PRINT_SHELL
@printf "$(1)\n"
$(shell $(1))
endef

#========================================================================
#	LINKER

#CREATE AN EXECUTABLE NAMED CYBORGEDDON LINKING ALL .O AND LIBS
#	CONOSOLE COMMAND: make
$(APPDIR)$(APP) : $(OBJSUBDIRS) $(ALLCCPOBJS)
#$(CC) -o $(APP) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCCPOBJS)) $(LIBS)
	$(CC) -o $(APPDIR)$(APP) $(ALLCCPOBJS) $(LIBS)

#========================================================================
#	COMPILER C++

#TAKE ALL .CPP AND DO ALL .O IF DON'T EXIST OR IF .CPP IS CHANGED
$(ALLCCPOBJS) : $(ALLCCPS)	
	$(foreach CCP_FILE,$^,$(call COMPILECFILE,$(CCP_FILE),$(subst $(SRC),$(OBJDIR),$(subst .cpp,.o,$(CCP_FILE)))))

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

#DELETE ALL COMPILED FILES
#	CONOSOLE COMMAND: make clean

RM  := rm -f

DEL_RELEASE := $(RM) $(RELEASEDIR)$(APP) -r $(RELEASEDIR)$(OBJ)
DEL_DEBUG   := $(RM) $(DEBUGDIR)$(APP) -r $(DEBUGDIR)$(OBJ)

clean :
	$(call PRINT_SHELL,$(DEL_RELEASE))
	$(call PRINT_SHELL,$(DEL_DEBUG))
