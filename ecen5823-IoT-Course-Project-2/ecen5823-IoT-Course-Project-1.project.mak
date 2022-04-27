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
# SIMPLICITY_STUDIO_METADATA=eJy9VGFv2jAQ/StRPk7Fbum6VRGlH4BKTEUgApumukLGMeCRxJHtlE6I/76zk0BoO7Rp6iRklHf23fPd89v6o/HwS68zmYXD6bjTC/3Ab90+J7H3xJUWMr0h/gU6J77HUyYjkS4BmE7uGtfEv20TRdJWpuQPzowHh1IdJDLiMexZGZMFGG82G6RFTOcaMZlgrXFo8khIxJlUHNLC6Ywr8zNk8A/nymzEd8k9r7WQccSVl9LEhmlu5JKn+7DdIGJehZfUmFk0Rwwy50oAYqMB7gR4quFC+JtII7kBFiLJYsEEFHZ88NPVbCPVWmeUccwZT6+um5eNvpw0OjJXmjdGBbHGBS454FoxfJrO6n/SWR3otHDRveqTyXQhlrmiBiZbkezyBc1j6LgHY3KzOyAZVaYjkwwOzEUM9CAKsJEyZisq0jdiOlq/RGHylQggirShDNYc5h5coia6RLNPH8+bzYvm50/1wTopBRHXTInMMm638GusuunR3Qpl4lJM7ss/88P+YHTf7/Qn32fhZNrtD2eDYXd670T/sCW+4ol84hHxgwWNNT8j/jwXsRFp75nFORSFyMPjAQ5hFKxA98qsq+LsNQrDsQngGnkM8g+IX72fYDBwYPmOSvSGkBNvqRRB+Zhgp1cyc8eMygtQRO47Z6ioizQ3eYYoiGs9m8OAbefQSZWhJWMuVxYlR8nbBJrrJiBSaJK1DA2yMYarggX6YFdc7tvPpLpu27WppA0NsXl3j28Oa9Qd7O2JkJcGBcibFuXqHmwKtpW6/+d22JIn7KssDJvgV9rYoXrNyI42VtvBJw6ba5oCcD/jYggWgkwclM+jOlg4jnaGElR9/417/TmH1TtwWL3mcLCuY9ipLOIjalb79O9nqnVeNTspkbdVOh6OeuNJ37kKmMrJOs4BtvaBaArOExrJ1l+pEnQeg68QG4Zlu7Przr2UvTf/1cEdUAWFCh7dQa+09btHf/cLD3HF5Q===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA