# Makefile template
PROJ_ROOT = ..

include $(PROJ_ROOT)/IncludePre.mk

LIBS_CYCLING_DEPEND = 1
SUB_DIRS = command client

TARGET_NAME = ftom
TARGET_SRCS = $(wildcard *.cpp)

DEPEND_LIBS = -lftom_command -lftgm

LIBS += -ljson -lnetsnmp  /home/xtra/KompexSQLiteWrapper-Source_1.11.13/lib/debug/KompexSQLiteWrapper_Static_d.a -lpthread -ldl

include $(PROJ_ROOT)/IncludePost.mk
