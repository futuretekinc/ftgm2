PROJ_ROOT=$(shell pwd)
SUBDIR=lib command ftom

export TOPDIR

all: subdirs

subdirs:
	@for dir in $(SUBDIR); do\
		make -C $$dir;\
	done

install:
	@for dir in $(SUBDIR); do\
		make -C $$dir install;\
	done

depend:
	@for dir in $(SUBDIR); do\
		make -C $$dir depend;\
	done

clean:
	@for dir in $(SUBDIR); do\
		make -C $$dir clean;\
	done

