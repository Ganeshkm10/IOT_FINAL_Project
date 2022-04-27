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
# SIMPLICITY_STUDIO_METADATA=eJy9VGtv2jAU/StRPk5t3NL1oYjSD0AnpiIQgU1TXSHjGOKR2JHtlFaI/77rPCC0rN2kdRIy8rnXvif3Hp+1OxwNvnbb42kwmIza3cD13ebNUxI7j0xpLsU1dk+9E+w6TFAZcrEAYDK+Pb7C7k0LKyyaqZI/GTUO/KdMmeeAwj9klTh28zTHac5lHDLlCJLYMMmMXDCxDdsEHrMqvCDGTMOZR6F0pjggNuqjto8mGqih71yEcqVRwJM05pRDYZOFXKLH8+lKqqVOCWWIUSbOrxpnx0RrvhAJE+by+AsRTEfL5PQElSxQrRx6m1D0fwlFO0JNVHSw2lIp5nyRKWJgThXNDpuTLIauOzGZsXgPSYkybZmkcGDGYyAIUYCNlDGNCBcHYjpcvkSpTDzN4XbtQdTThlBYM5i9f+Y1vDNvevH5pNE4bVxe1IcbMk0VTy3XVhPVd9XX7X1PoS1UiijfuUdu0OsP73rt3vjHNBhPOr3BtD/oTO5y2d6vsatYIh9ZiF1/TmLNjrA7y3hsuOg+0TiDohC5f9jBgcwULdCtIutaOHqNwkDsBYkMs5jBQexWL8Dv93PQgecjtF+i1xhjNzIm9RFarVZV66CLSGs0LJI8RqViNtMpmeXHjMoKkIf5PqNeUdfTzGSpR0BSy+kMhmo7572jLW9BaX5bGiZ717cwtDefARfQJvvsNYjFGKYKHt4nu6IybzuV6oNbeaNK4tASe+/m4eC4hp3+1mIwfmkygBy0mbzuzmogrVT7P2iILfqGdZWlIQl+pYXt6tdMbC+xSgeH2CXXdAXgds7FGCwENzFQPwvrYOE1OrcSv+r8b3zrzzlEH8Ahes1hZ1n7cK6zkA2JibbXf6Sd1pnVTKVEDit1NBh2R+Ne7i1gLe9Uyp1gbZ+JJuBAgZF0+Y0oTmYx+Au2YVjWG7tu8vey9eW/OrgBsqBSzsJb6Je2vvfgbn4B8KS0aw===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA