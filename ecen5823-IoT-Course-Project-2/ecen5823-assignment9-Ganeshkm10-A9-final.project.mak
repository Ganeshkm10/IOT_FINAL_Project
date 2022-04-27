####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 10                                              #
####################################################################

BASE_SDK_PATH = C:/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2
UNAME:=$(shell uname -s | sed -e 's/^\(CYGWIN\).*/\1/' | sed -e 's/^\(MINGW\).*/\1/')
ifeq ($(UNAME),MINGW)
# Translate "C:/super" into "/C/super" for MinGW make.
SDK_PATH := /$(shell echo $(BASE_SDK_PATH) | sed s/://)
endif
SDK_PATH ?= $(BASE_SDK_PATH)
COPIED_SDK_PATH ?= 

# This uses the explicit build rules below
PROJECT_SOURCE_FILES =

C_SOURCE_FILES   += $(filter %.c, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cpp, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cc, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.s, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.S, $(PROJECT_SOURCE_FILES))
LIB_FILES        += $(filter %.a, $(PROJECT_SOURCE_FILES))

C_DEFS += \

ASM_DEFS += \

INCLUDES += \
 -Iautogen

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \

LIBS += $(GROUP_START) $(PROJECT_LIBS) $(GROUP_END)

LIB_FILES += $(filter %.a, $(PROJECT_LIBS))

C_FLAGS += \

CXX_FLAGS += \

ASM_FLAGS += \

LD_FLAGS += \


####################################################################
# SDK Build Rules                                                  #
####################################################################
$(OUTPUT_DIR)/project/autogen/gatt_db.o: autogen/gatt_db.c
	@echo 'Building autogen/gatt_db.c'
	@mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/gatt_db.c
CDEPS += $(OUTPUT_DIR)/project/autogen/gatt_db.d
OBJS += $(OUTPUT_DIR)/project/autogen/gatt_db.o

# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=eJzFVP1v2jAQ/Vei/DiVuKVr10aUagI6MRWBCGya6goZxxCPxI5sp3RC/O875wNC21WdtA8JGfndxfd89/w27mg8/NzrTGbBcDru9ALXd1vXj0nsPDCluRRX2D3xjrHrMEFlyMUSgOnkpnGB3es2Vli0UiW/M2oc+E+ZMj8CCv+QVeLYzdMcp7WQcciUI0hiwyQzcsnELmwTeMyq8JIYMwvnHoXSmeKA2KiPOj6aaqCGvnIRyrVGAU/SmFMOhU0WcokezmZrqVY6JZQhRpk4u2ieNojWfCkSJsxl4xMRTEer5OS48fGyseCCxKikg2p10evMov/ELNoza6Gip9WWSrHgy0wRA5Or+HbZgmQxzMGJyZzFB0hKlOnIJIUP5jwGphAF2EgZ04hw8UJMh6unKJWJpzmcrj2IetoQCmsGavBPvaZ36s3O3x83myfND+f1cYdMU8VTy7XdQvVddbuD+xRqQ6Ws8p175Ab9wei23+lPvs2CybTbH84Gw+70Nhfy3Qa7iiXygYXY9Rck1uwIu/OMx4aL3iONMygKkbv7PRzITNEC3Wm0Loqj5ygMxB6QyDCLGXyI3epN+INBDjrwoIT2S/QKY+xGxqQ+Quv1umoddBFpjUZFkseoVMxmOiWz/DOjsgLkYb7PqFfU9TQzWeoR0NZqNoeh2s55bxWZt6Q0PzYNk4M6bQx9zofBBfTLOoIG1RjDVEHIe2dXVObtxlPdvJ13rLwB9Maeu71/cW6j7mDnPhg/9R9AXnSgvO7ehSCtlP2f7Iyt/oq9lRwgCX6lze2J1IzuILFKB/PYJ9eUBuBu8sU8LAQnMXgPLKyDhQ3p3GX8agS/sLS3c4j+AofoOYe9iR3CueBCNiIm2h3/T5y2TrHmNyXysnbHw1FvPOnntrOpOSyYwcY+EE3AhAIj6eoLUZzMY7AYbMOwbLZ23eYv5a1cf/PkLbAG3XIW3kAHtfXGe3f7E65Sxas==END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA