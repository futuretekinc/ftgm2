MODEL=i686

TARGET=$(TOPDIR)/target/$(MODEL)
EXEC_PREFIX=$(TARGET)/usr/bin
INC_PREFIX=$(TARGET)/usr/include
LIB_PREFIX=$(TARGET)/usr/lib
CONF_PREFIX=$(TARGET)/etc
CGI_PREFIX=$(TARGET)/www/cgi-bin
export MODEL TOPDIR

CFLAGS:= -fPIC -c -g -Wall \
				-std=gnu++11\
				-I. \
				-I/usr/include/azureiot\
				-I/usr/include/azureiot/inc\
				-I$(TOPDIR)/lib\
				-I/home/xtra/KompexSQLiteWrapper-Source_1.11.13/inc

LDFLAGS:= -Wl,--no-as-needed\
				-L.\
				-L$(TOPDIR)/lib

CC=g++
#CC=armv5-linux-g++
#CC=arm-openwrt-linux-uclibcgnueabi-gcc
CFLAGS:= -fPIC \
	$(CFLAGS) \
		-I/usr/local/include \
		-I/usr/local/include/cjson 

LDFLAGS:= $(LDFLAGS)\
		-L/usr/local/lib \
		-I/home/xtra/KompexSQLiteWrapper-Source_1.11.13/lib/debug


BIN_CFLAGS=$(CFLAGS)
BIN_LDFLAGS=$(LDFLAGS)
LIB_CFLAGS=$(CFLAGS)
LIB_LDFLAGS=$(LDFLAGS)
