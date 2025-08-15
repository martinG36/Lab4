MODULES = module/freertos
BOARD = edu-ciaa-nxp
MUJU = ./muju

CFLAGS += -MMD -MP

include $(MUJU)/module/base/makefile

-include $(OBJECTS:.o=.d)

doc:
	doxygen Doxyfile
