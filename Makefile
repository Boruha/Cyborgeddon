###############################################
#       MACROS
###############################################

#   $(1) = orden (ejemplo "rm -f -r obj/")
define PRINT_SHELL
@printf "$(1)\n"
$(shell $(1))
endef

#   $(1) = compiler
#   $(2) = xxxxx.o
#   $(3) = xxxxx.cpp
#   $(4) = xxxxx.hpp
#   $(5) = flags
define COMPILE_CPP
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)
endef

#	$(1) = xxxxx.cpp
define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJDIR)%,$(1))))
endef

#	$(1) = xxxxx.cpp
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

# COMPILER
CC          := g++

# FLAGS
CCFLAGS     := -Wall -pedantic
RELEASEFLAG := -O3
DEBUGFLAG   := -g

# MAIN FOLDERS
APPDIR      :=
RELEASEDIR  := Release/
DEBUGDIR    := Debug/

# DEBUG OR RELEASE
ifeq (${debug}, true)
    CCFLAGS += $(DEBUGFLAG)
    APPDIR  := $(DEBUGDIR)
else
    CCFLAGS += $(RELEASEFLAG)
    APPDIR  := $(RELEASEDIR)
endif

# STRUCTURE FOLDERS
MKDIR 		:= mkdir -p
SRC 		:= src
OBJ 		:= obj
OBJDIR		:= $(APPDIR)$(OBJ)
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJDIR)%,$(SUBDIRS))

# FILES
APP 		:= cyborgeddon
ALLCPP    	:= $(shell find $(SRC)/ -type f -iname *.cpp)
ALLOBJ 	    := $(patsubst $(SRC)%,$(OBJDIR)%,$(patsubst %.cpp,%.o,$(ALLCPP)))

# HEADERS AND LIBRARIES
LIBS 		:= -lIrrlicht
INCLUDE 	:= -I/usr/include/irrlicht/ -I.

# CLEAN

RM  := rm -f

DEL_RELEASE := $(RM) $(RELEASEDIR)$(APP) -r $(RELEASEDIR)$(OBJ)
DEL_DEBUG   := $(RM) $(DEBUGDIR)$(APP) -r $(DEBUGDIR)$(OBJ)

.PHONY: info clean

#========================================================================
#	LINKER

# CREATES AN EXECUTABLE NAMED CYBORGEDDON LINKING ALLOBJ AND LIBS
#   CONSOLE COMMANDS:
#                       make
#                       make debug=true

$(APPDIR)$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $(APPDIR)$(APP) $(ALLOBJ) $(LIBS)

#========================================================================
#	COMPILER C++

# COMPILES EVERY .CPP IF IT HAS NOT CHANGED SINCE THE LAST MAKE

$(foreach F,$(ALLCPP),$(eval $(call COMPILE_CPP,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS))))

#========================================================================
#	FOLDER STRUCTURE

$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)

#========================================================================
#	OTHER COMMANDS

# SHOW WHICH ELEMENTS ARE BEING INCLUDED IN THIS VARIABLES
#	CONOSOLE COMMAND:
#                       make info

info :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLOBJ))

# DELETE EXECUTABLE AND MAIN FOLDERS CONTENT
#	CONOSOLE COMMAND:
#                       make clean

clean :
	$(call PRINT_SHELL,$(DEL_RELEASE))
	$(call PRINT_SHELL,$(DEL_DEBUG))
