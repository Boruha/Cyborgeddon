LIBSDIRS	:= $(shell find -maxdepth 1 -type d)

ifeq ($(LIBSDIRS),.)
	LIBSDIRS :=
endif

.PHONY : all clean

all :
	$(foreach L,$(LIBSDIRS),$(MAKE) -C $(L))

clean :
	$(foreach L,$(LIBSDIRS),$(MAKE) -C $(L) clean)