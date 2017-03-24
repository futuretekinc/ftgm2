# IncludePre.mk

CC = g++
AR = ar
RANLIB = ranlib
RM = rm
MKDIR = mkdir
MAKE = make

ROOT_SRC_DIR = $(PROJ_ROOT)/src
ROOT_OUT_DIR = $(PROJ_ROOT)/out
ROOT_LIB_DIR = $(ROOT_OUT_DIR)/lib

INC_DIRS = -I$(PROJ_ROOT)/Include -I$(PROJ_ROOT)/lib -I/home/xtra/KompexSQLiteWrapper-Source_1.11.13/inc

ifeq ($(RELEASE), 1)
OBJS_DIR = Release
DBG_FLAGS = -O2 -DNDEBUG -std=gnu++11
else
OBJS_DIR = Debug
DBG_FLAGS = -g -O0 -DDEBUG -std=gnu++11
endif

DEPEND_FILE = $(OBJS_DIR)/depend_file

LIB_DIRS = -L$(ROOT_LIB_DIR)/$(OBJS_DIR)
