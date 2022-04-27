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
# SIMPLICITY_STUDIO_METADATA=eJy9VG1v2jAQ/itRPk5t3NK1qyJKPwCdmIpABDZNdYWMY4hHYke2A50Q/30XJ4HQsnaT1knIKM+d7x7fy7Nxh6PBl257PA0Gk1G7G7i+27x9SmJnxZTmUtxg99w7w67DBJUhFwsAJuO702vs3rawwqKZKvmDUePAJaH9RIYsBp/ImNRHaL1ee5rHZKY9KhOkNQpMFnLpMSoVg7BwO2XK/Awo/MO9Mhp2bXDHac5lHDLlCJLkZpIZuWBiZ84deMwq84IYMw1nHoXImeKA5FYftX000fAg9I2LUK6BBU/SmFMOiS0ftLqcrqVa6pRQhhhl4vK6cXFKtOYLkTBhLk8/E8F0tEzOz1DJAtXSodcJRf+XULQn1ERFBatPKsWcLzJFDHS3otlhc5LFUHUHWmX7t0dSokxbJilcmPEYCIIVYCNlTCPCxRGbDpfPUeh+NQhg9bQhFM4Meu9feA3vwptefTxrNM4bn67qzbXj5IdMU8XTnHGriV5i1UsP3lZMJyoHyn65J27Q6w/ve+3e+Ps0GE86vcG0P+hM7u3gP2ywq1giVyzErj8nsWYn2J1lPDZcdJ9onEFSsDw87uFAZooW6G4663Nx8hKF5uQB4BlZDCvgY7faIb/ft2C5SyV6g/Er+zQsnMqFAk+nZGavGZUVIA/td0a9Iq+nmclSj8B4LaczaHBeOe+NOfMWlNpoaZgchG9hKK/tARdQplw4NAyOMUwVPLwP+YlKv11Xqge3bKFK4lCSPO728Wi7hp3+TqQwfi5TgBwVKpt3L1bgVk7+PyhInvQVGStTgxP8Sjnb568J2oFj5Q5qsXeuzRWAuz4XbcghiMRg+llYBwvd0VZW/Kryv9GwP+cQvQOH6CWHvXwdwnbOQjYkJtqFf09prTOriUqJHJ/U0WDYHY17VltAWt7IZJVgk6+JJqBAgZF0+ZUoTmYx6AvOzXBstvm5tfuy0+i/urgFsjClnIV3UC+d696ju/0FounMmQ===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA