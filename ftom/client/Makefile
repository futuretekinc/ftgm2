# Makefile template
PROJ_ROOT = ../..

include $(PROJ_ROOT)/IncludePre.mk

SUB_DIRS = lib

LIBS_CYCLING_DEPEND = 1

TARGET_NAME = ftomc
TARGET_SRCS = $(wildcard *.cpp)

DEPEND_LIBS = -lftomc -lftgm

LIBS += -ljson -lnetsnmp  /home/xtra/KompexSQLiteWrapper-Source_1.11.13/lib/debug/KompexSQLiteWrapper_Static_d.a -lpthread -ldl

include $(PROJ_ROOT)/IncludePost.mk
