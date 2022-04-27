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
# SIMPLICITY_STUDIO_METADATA=eJy9VO9v2jAQ/VeifJzauKVr10WUfgA6MRWBCGya6goZxxCPxI5sp7RC/O875weElrWbtE5CRn539r3cPb+1OxwNvnbb42kwmIza3cD13eb1YxI7D0xpLsUVdk+9E+w6TFAZcrEAYDK+Ob7E7nULKyyaqZI/GTUO/KdMmaeAwj9klTh28zTHac5lHDLlCJLYMMmMXDCxDdsEHrMqvCDGTMOZR6F0pjggNuqjto8mGqih71yEcqVRwJM05pRDYZOFXKKH8+lKqqVOCWWIUSbOLxtnx0RrvhAJE+bz8RcimI6WyekJKlmgWjn0OqHo/xKKdoSaqOhgtaVSzPkiU8TAnCqaHTYnWQxdd2IyY/EekhJl2jJJ4cCMx0AQogAbKWMaES4OxHS4fI5SmXiaw+3ag6inDaGwZjB7/8xreGfe9OLjSaNx2vh0UR9uyDRVPLVcW01U31Vft/c9hbZQKaJ85x65Qa8/vO21e+Mf02A86fQG0/6gM7nNZXu3xq5iiXxgIXb9OYk1O8LuLOOx4aL7SOMMikLk7n4HBzJTtEC3iqxr4eglCgOxFyQyzGIGB7FbvQC/389BB56P0H6JXmGM3ciY1EdotVpVrYMuIq3RsEjyGJWK2UynZJYfMyorQB7m+4x6RV1PM5OlHgFJLaczGKrtnPeGtrwFpfltaZjsXd/C0N58BlxAm+yz1yAWY5gqeHgf7IrKvO1Uqg9u5Y0qiUNL7L2b+4PjGnb6W4vB+LnJAHLQZvK6O6uBtFLt/6Ahtugr1lWWhiT4lRa2q18zsb3EKh0cYpdc0xWA2zkXY7AQ3MRA/Sysg4XX6NxK/Krzv/GtP+cQvQOH6CWHnWXtw7nOQjYkJtpe/552WmdWM5USOazU0WDYHY17ubeAtbxRKXeCtX0mmoADBUbS5TeiOJnF4C/YhmFZb+y6yd/L1pf/6uAGyIJKOQtvoF/a+t69u/kFG1e0dw===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA