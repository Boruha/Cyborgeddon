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
#   $(5) = includes
#	$(6) = flags
define COMPILE_CPP
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5) $(6)
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
CCACHE		:= ccache
CC          := $(CCACHE) g++
C           := $(CCACHE) gcc

# FLAGS
CCFLAGS     := -pthread -Wall -pedantic -std=c++17
CFLAGS      := $(CCFLAGS)
RELEASEFLAG := -O0
DEBUGFLAG   := -Og

# MAIN FOLDERS
APPDIR      :=
RELEASEDIR  := Release/
DEBUGDIR    := Debug/

# DEBUG OR RELEASE
ifeq ($(debug), true)
    CCFLAGS += $(DEBUGFLAG)
    CFLAGS  += $(DEBUGFLAG)
    APPDIR  := $(DEBUGDIR)
else
    CCFLAGS += $(RELEASEFLAG)
    CFLAGS  += $(RELEASEFLAG)
    APPDIR  := $(RELEASEDIR)
endif

# STRUCTURE FOLDERS
MKDIR 		:= mkdir -p
SRC 		:= src
OBJ 		:= obj
OBJDIR		:= $(APPDIR)$(OBJ)
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJDIR)%,$(SUBDIRS))
LIBDIR      := lib

# FILES
APP 		:= cyborgeddon
ALLC        := $(shell find $(SRC)/ -type f -iname *.c)
ALLCPP    	:= $(shell find $(SRC)/ -type f -iname *.cpp)
ALLOBJ      := $(foreach F,$(ALLCPP) $(ALLC),$(call C2O,$(F)))

# HEADERS AND LIBRARIES
INCLUDE 	:= -I/usr/include/irrlicht/ -I./$(SRC)/ -I.
LIBS 		:= -lIrrlicht -lfmod -lfmodL -lfmodstudio -lfmodstudioL -Wl,-rpath,$(LIBDIR)
GOLD_OPTION	:= -fuse-ld=gold

# CLEAN

RM  := rm -f

DEL_RELEASE := $(RM) $(RELEASEDIR)$(APP) -r $(RELEASEDIR)$(OBJ)
DEL_DEBUG   := $(RM) $(DEBUGDIR)$(APP) -r $(DEBUGDIR)$(OBJ)

#========================================================================
#	LINKER

# CREATES AN EXECUTABLE NAMED CYBORGEDDON LINKING ALLOBJ AND LIBS
#   CONSOLE COMMANDS:
#                       make
#                       make debug=true

$(APPDIR)$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $(APPDIR)$(APP) $(ALLOBJ) $(LIBS) $(GOLD_OPTION)

#========================================================================
#	COMPILER C++

# COMPILES EVERY .CPP / .C IF IT HAS NOT CHANGED SINCE THE LAST MAKE

$(foreach F,$(ALLCPP),$(eval $(call COMPILE_CPP,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(INCLUDE),$(CCFLAGS))))
$(foreach F,$(ALLC),$(eval $(call COMPILE_CPP,$(C),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(INCLUDE),$(CFLAGS))))

#========================================================================
#	FOLDER STRUCTURE

$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)

#========================================================================
#	OTHER COMMANDS

.PHONY: info clean libs libs-clean

# SHOW WHICH ELEMENTS ARE BEING INCLUDED IN THIS VARIABLES
#	CONSOLE COMMAND:
#                       make info

info :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPP))
	$(info $(ALLOBJ))

# DELETE EXECUTABLE AND MAIN FOLDERS CONTENT
#	CONSOLE COMMAND:
#                       make clean

clean :
	$(call PRINT_SHELL,$(DEL_RELEASE))
	$(call PRINT_SHELL,$(DEL_DEBUG))

# COMPILES EVERY LIBRARY INTO STATIC ONES
#	CONSOLE COMMAND:
#						make libs

libs :
	$(MAKE) -C $(LIBDIR)

# CLEANS EVERY LIBRARY
#	CONSOLE COMMAND:
#						make libs-clean

libs-clean :
	$(MAKE) -C $(LIBDIR) clean