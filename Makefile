XBE_TITLE = ogxcloud
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/src/main.c $(CURDIR)/src/lwftp/lwftp.c $(CURDIR)/src/ogxc_ttf/ogxc_ttf.c
NXDK_DIR ?= $(CURDIR)/../nxdk/
NXDK_NET = y
NXDK_SDL = y

all_local: cp_font all

include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/bg_720_480.jpg
$(GEN_XISO): $(OUTPUT_DIR)/bg_720_480.jpg
$(OUTPUT_DIR)/bg_720_480.jpg: $(CURDIR)/resources/bg_720_480.jpg $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'

cp_font:
	@mkdir -p $(OUTPUT_DIR)
	cp resources/vegur-regular.ttf $(OUTPUT_DIR)/
