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
# SIMPLICITY_STUDIO_METADATA=eJy9VGFv2jAQ/StRPk7Fbum6VRGlH4BKTEUgApumukLGMeCRxJHtlE6I/76zk0BoO7Rp6iRklHfn8/Pd89v6o/HwS68zmYXD6bjTC/3Ab90+J7H3xJUWMr0h/gU6J77HUyYjkS4BmE7uGtfEv20TRdJWpuQPzowHm1IdJDLiMeSsjMkCjDebDdIipnONmEyw1jg0eSQk4kwqDmVhd8aV+Rky+Id9ZTXiu+Ke11rIOOLKS2liwzQ3csnTfdgmiJhX4SU1ZhbNEYPKuRKA2GiAOwGeargQ/ibSSG6AhUiyWDABBzs++OlqtpFqrTPKOOaMp1fXzctGX04aHZkrzRujghguGeDaUfg0mdX/I7M6kGnhonPVJ5PpQixzRQ1MtaLY5Quax9BtD0bk5nZAMqpMRyYZbJiLGMhBFGAjZcxWVKRvxHS0fonC1CsBQBRpQxmsOcw8uERNdIlmnz6eN5sXzc+f6kN1MgoirpkSmWXcbuHXWHXTo7sVqsSlkNyXf+aH/cHovt/pT77Pwsm02x/OBsPu9N4J/mFLfMUT+cQj4gcLGmt+Rvx5LmIj0t4zi3M4FCIPjwc4hEGwAt2rsq6Js9coDMcWgGvkMUg/IH71doLBwIHlGyrRG0JOvKNSAuVDgkyvZOa2GZUXoIjcd85QcS7S3OQZoiCt9WwOA7adQyc0hpaMuUpZlByVbhNoreu/SKFF1iw0iMYYrgoO6INdcZm3n0h12bZrUkka2mHr7h7fHNWoO9gbEyEvrQmQN83JnXswKEgrVf+PzbAHnrCt8lhIgl9pX4ezawZ2lFilg0Mckmt6AnA/32IEFoJKHFTPozpYeI12VhJUXf+Nb/05h9U7cFi95nCwrWPYaSziI2pW+/LvZad1VjUjKZG3FToejnrjSd/5ybZmo/DKt/YZaAruEhrJ1l+pEnQeg3cQG4Zlu7Przr2HE/T+stgOiII6BY/uoE/a+tyjv/sFw/3DsQ===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA