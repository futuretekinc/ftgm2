# Makefile template

include $(PROJ_ROOT)/IncludePre2.mk

# 라이브러리 이름입니다. 추후 빌드된 라이브러리 이름은 lib$(LIB_NAME).a가 됩니다.
LIB_NAME = # Example

# 라이브러리 소스 파일들의 목록입니다.
LIB_SRCS = # LibSource1.c LibSource2.c ...

# 라이브러리를 사용하는 실행 파일들의 소스 파일 목록입니다.
TARGET_SRCS = # TargetBin1.c TargetBin2.c ...

# 실행 파일들이 링킹해야 하는 라이브러리입니다.
LIBS += # -lLibrary1 -lLibrary2

include $(PROJ_ROOT)/IncludePost2.mk
