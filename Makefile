XBE_TITLE = ogxcloud
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c $(CURDIR)/lwftp.c
NXDK_DIR ?= $(CURDIR)/../nxdk/
NXDK_NET = y

include $(NXDK_DIR)/Makefile
