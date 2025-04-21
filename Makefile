XBE_TITLE = ogxcloud
GEN_XISO = $(XBE_TITLE).iso
NXDK_DIR ?= $(CURDIR)/../nxdk/
NXDK_NET = y
NXDK_SDL = y

SRCS = \
	$(CURDIR)/src/main.c \
	$(CURDIR)/lib/lwftp/lwftp.c \
	$(CURDIR)/lib/ogxc_ttf/ogxc_ttf.c \
	$(CURDIR)/lib/ogxc_osk/ogxc_osk.c
FORMAT_SRCS = \
	$(SRCS) \
	$(CURDIR)/lib/lwftp/lwftp.h \
	$(CURDIR)/lib/ogxc_ttf/ogxc_ttf.h \
	$(CURDIR)/lib/ogxc_osk/ogxc_osk.h

all_local: cp_font all

# Include global NXDK makefile
include $(NXDK_DIR)/Makefile

# Copy over image resources to output ISO
TARGET += $(OUTPUT_DIR)/bg_720_480.jpg
$(GEN_XISO): $(OUTPUT_DIR)/bg_720_480.jpg
$(OUTPUT_DIR)/bg_720_480.jpg: $(CURDIR)/resources/bg_720_480_transparent.jpg $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'
TARGET += $(OUTPUT_DIR)/bg_osk.png
$(GEN_XISO): $(OUTPUT_DIR)/bg_osk.png
$(OUTPUT_DIR)/bg_osk.png: $(CURDIR)/lib/ogxc_osk/resources/bg.png $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'

cp_font:
	@mkdir -p $(OUTPUT_DIR)
	cp resources/vegur-regular.ttf $(OUTPUT_DIR)/

format:
	clang-format --style="Microsoft" -i $(FORMAT_SRCS)
