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
# SIMPLICITY_STUDIO_METADATA=eJy9VG1v2jAQ/itRPk7FprB2VUTpB6ASUxGIwF7UVMg4BjySOLKd0gnx33d2EggtQ5umTkJGfu589+Renq07Gg8/9zqTmT+cjjs93/Xc1t1LHDnPTCouktvAvUT1wHVYQkXIkyUA08l97SZw79qBDJJWKsUPRrUDjxLlvSgOHiutUw/jzWaDNk0k5BI36vVL/G3w4NMVi0mNJ0qThDIIDC88ZdEHQYnOcyZZFDmVKIpHZK4QFTFWCvs6C7lAjAppIgCDlEn906fwD48LRoFrCTpOayGikEknIbExk0yLJUv2ZuPAI1aal0TrWThHFCJn0nyNsXq44+GpgqLgrzwJxQZY8DiNOOWQ2PLBz1ezjZBrlRLKMKMsubppNGt9Mal1RCYVq41yYrUGLjjgSjJ8ns7qf9JZHei0cF698kpFsuDLTNpOlSS7bEGyCCruQJtYdISkROqOiFN4MOcR0AMrwFqIiK4IT07YVLh+jULnyyEAK4LpoXBm0HeviRqoiWbXH+uNxmXj03W1sSFTVPLUcG23cPVWft3R9+QTjYsBsjf3wvX7g9FDv9OffJ/5k2m3P5wNht3pg12Wx23gShaLZxYGrrcgkWIXgTvPeKR50nuhUQZJwfL4dIB9KD/N0f00Vifh4i0KDTEBYhFmEYy8F7jl3nmDgQWL/SvQ2yA42sLj/SkaXywQeDoFM/tMyywHeWjvGUV5XqSYzlJEYKDWszk01VQOnZ0stKTUxkrD+Ch4O4Di2g7wBIpkpEbBqGjNZM4CfTAnLvz2PSk/t23LVNCGgpi4u6eTzRp1B3tZC4LXwgbISWmzeQ/yBm7FrP9zOUzKM5JVJAYn+BXSdcheEa8jx9IdtOHgXJkpAPc9zptgIIjEYPJZWAVzlVFWRLyy7r9RrD/nsHoHDqu3HA5ydQzbKQvZiOjVPvz7CWmVV0VOCuT0lI6Ho9540reqAqJyNo9VgK1ZEEVAeXwt6PoLkZzMI9CVwJjh2O7MubObstfjv3q4A6owoZyF91ArZfTuyd39AvdO2PQ==END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA