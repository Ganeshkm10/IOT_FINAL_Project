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
# SIMPLICITY_STUDIO_METADATA=eJy9VO9v2jAQ/VeifJzauKVrV0WUfgA6MRWBCGya6goZxxCPxI5sBzoh/ved8wNCy9pNWichI787+17unt/GHY4GX7rt8TQYTEbtbuD6bvP2KYmdFVOaS3GD3XPvDLsOE1SGXCwAmIzvTq+xe9vCCotmquQPRo0D/ylT5mdA4R+yShy7eZrjNOcyDplyBElsmGRGLpjYhW0Cj1kVXhBjpuHMo1A6UxwQG/VR20cTDdTQNy5CudYo4Ekac8qhsMlCLtHqcrqWaqlTQhlilInL68bFKdGaL0TChLk6/UwE09EyOT9DJQtUK4deJxT9X0LRnlATFR2stlSKOV9kihiYU0Wzw+Yki6HrTkxmLD5AUqJMWyYpHJjxGAhCFGAjZUwjwsWRmA6Xz1EqE09zuF17EPW0IRTWDGbvX3gN78KbXn08azTOG5+u6sMNmaaKp5Zrq4nqu+rrDr6n0BYqRZTv3BM36PWH9712b/x9Gownnd5g2h90Jve5bB822FUskSsWYtefk1izE+zOMh4bLrpPNM6gKEQeHvdwIDNFC3SnyLoWTl6iMBB7QSLDLGZwELvVC/D7/Rx04PkI7ZfoDcbYjYxJfYTW63XVOugi0hoNiySPUamYzXRKZvkxo7IC5GG+z6hX1PU0M1nqEZDUcjqDodrOeW9oy1tQmt+WhsnB9S0M7c1nwAW0yT57DWIxhqmCh/fBrqjM202l+uBW3qiSOLTE3rt9PDquYae/sxiMn5sMIEdtJq+7txpIK9X+Dxpii75iXWVpSIJfaWH7+jUTO0is0sEh9sk1XQG4m3MxBgvBTQzUz8I6WHiNzq3Erzr/G9/6cw7RO3CIXnLYW9YhnOssZENiot3172mndWY1UymR40odDYbd0biXewtYyxuVcifY2GeiCThQYCRdfiWKk1kM/oJtGJbN1q7b/L3sfPmvDm6BLKiUs/AO+qWt7z2621/bUrRl=END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA