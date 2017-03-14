ifeq ($(TOPDIR),)
TOPDIR=$(PWD)/..
endif

include $(TOPDIR)/IncludePre.mk

#CFLAGS=$(CFLAGS)\
		-D__malloc_and_calloc_defined=1 \
		-D__need_malloc_and_calloc=1

BIN_TARGET=test
BIN_OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp) $(wildcard command/*.cpp))
BIN_LDFLAGS=$(LDFLAGS) -pthread -lftm -ljson -lnetsnmp  /home/xtra/KompexSQLiteWrapper-Source_1.11.13/lib/debug/KompexSQLiteWrapper_Static_d.a -ldl
include $(TOPDIR)/IncludePost.mk
