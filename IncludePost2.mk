# MakefilePost.mk

LIB_FULL_NAME = $(ROOT_LIB_DIR)/$(OBJS_DIR)/lib$(LIB_NAME).a
LIB_OBJS = $(LIB_SRCS:%.cpp=$(OBJS_DIR)/%.o)

ALL_LIBS = $(LIBS)
ALL_LIBS += -lpthread

TARGET_OBJS = $(TARGET_SRCS:%.cpp=$(OBJS_DIR)/%.o)
TARGET_NAMES = $(TARGET_SRCS:%.cpp=$(OBJS_DIR)/%)

.SUFFIXES : .cpp .o

all : $(LIB_FULL_NAME) $(TARGET_NAMES)

$(LIB_FULL_NAME) : $(LIB_OBJS)
	@`[ -d $(ROOT_LIB_DIR)/$(OBJS_DIR) ] || $(MKDIR) -p $(ROOT_LIB_DIR)/$(OBJS_DIR)`
	$(AR) rcv $@ $(LIB_OBJS)
	$(RANLIB) $@

$(OBJS_DIR)/%.o : %.cpp
	@echo "==================================================="
	@echo "= Compiling $@ "
	@echo "==================================================="
	@`[ -d $(OBJS_DIR) ] || $(MKDIR) $(OBJS_DIR)`
	$(CC) $(CFLAGS) $(DBG_FLAGS) $(INC_DIRS) -c $< -o $@

.SECONDEXPANSION:
$(TARGET_NAMES): $$@.o
	@echo "==================================================="
	@echo "= Linking $@ "
	@echo "==================================================="
ifeq ($(LIBS_CYCLING_DEPEND),1)
	$(CC) -o $@ $< $(LIB_DIRS) -Wl,-\( $(ALL_LIBS) -Wl,-\)
else
	$(CC) -o $@ $< $(LIB_DIRS) $(ALL_LIBS)
endif

depend :
	@`[ -d $(OBJS_DIR) ] || $(MKDIR) $(OBJS_DIR)`
	@$(RM) -f $(DEPEND_FILE)
	@for FILE in $(LIB_SRCS:%.cpp=%) $(TARGET_SRCS:%.cpp=%); do \
		$(CC) -MM -MT $(OBJS_DIR)/$$FILE.o $$FILE.cpp \
			$(CFLAGS) $(DBG_FLAGS) $(INC_DIRS) >> $(DEPEND_FILE); \
	done

clean :
	$(RM) -fr $(OBJS_DIR) $(LIB_FULL_NAME)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(strip $(LIB_SRCS) $(TARGET_SRCS)),)
-include $(DEPEND_FILE)
endif
endif
endif