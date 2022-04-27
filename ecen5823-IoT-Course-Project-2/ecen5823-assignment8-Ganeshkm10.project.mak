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
# SIMPLICITY_STUDIO_METADATA=eJy9VG1v2jAQ/isoHydIKLSURpR+ADoxFYF42TQ1FTKOIR6OHdlO6YT477s4L0DL2m5aJyGjPHe+O98992yt0Xj4pdeZzifD2bjTm1iu1bp5ClnpkUhFBb/2rDO76lklwrHwKV8BMJveVpqeddP2pMdbkRQ/CNYluMSVGwqfMPAJtI5cx9lsNraiDC2UjUXoKOVMdOxTYRMsJIGwcDsiUv+cYPiHe1k0zzLBS6XWUjCfyBJHYWJGsRYrwgtz4kAZyc0rpPXcX9gYIseSApJYXafjOjMFD3K+Ue6LDVRBw4hRTCGxqcd5vJhvhFyrCGHiEEz4RbNWryCl6IqHhOtm5TPiRAXr8KzqZFU4B+mc1wsK/m9Bwb6glpN2MP/Egi/pKpZIw3TzMrtkiWIGXS/BqMz89kiEpO6IMIILC8qgQLACrIVgOECUn7Apf/0chennRACrrTTCcMYwe7du1+y6PW+cV2u1s9pl43C4hk6uTxSWNEoqbrecl1j+0qO3pex0MkKZL6tsTfqD0V2/059+n0+ms25/OB8Mu7M7Q/z7rWdJEopH4nuWu0RMkbJnLWLKNOW9J8xiSAqW+4c9PBGxxClasPOQF+WXKAwnCQDPiBmsgOtZ+Q65g4EBs13K0GvPe2WfRqlTtlDgWcoqM9e0jFOQ+uY7xnaa11ZEx5GNgF7r+QIGnHTOfoNn9gpjEy3yw6PwbQ/aa2ZAObQpEQ4FxNGayLQO+1NyOplfMZX8wW3TqKxwaEkSd/dwclyj7qAQKc97LlOAnBQqk3cvVuCWMf8fNCRJ+oqMZanBCX6ZnO3zHwjakWPuDmqxdz7gFYDFnNMxJBBEIsB+4h+Cqe4oIytu3vnfaNj7awg+oIbgZQ17+TqGDc98MkI6KMJ/pLQeVnYgKhlymqnj4ag3nvaNtoC0vJHJKME2WROFQIEmWuD1VyQpWjDQFy8xw7HdJefO7Euh0X90cQfFAksp8W+hXyrTvWJq2faVCw6bFicpTqubkHRFOWKZG48ZKydyzzW8D4BKrdoAVa9dNZu78t8lCt6b6LJxflWt1kE5dr8A8ysT5w===END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA