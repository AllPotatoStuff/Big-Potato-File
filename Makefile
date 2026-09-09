rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = main.cpp $(call rwildcard,src,*.cpp)
SRCS_C = $(call rwildcard,src,*.c)
OBJS = $(SRCS:.cpp=.o) $(SRCS_C:.c=.o)
BINFILE = ./bigpotatofile.exe
CC = g++
CC_C = gcc

COMPILER_FLAGS = -finline-functions -std=c++20 -Iinclude
LINKER_FLAGS = -lm -lpthread

# Release:
#CFLAGS = -O3 -fomit-frame-pointer -ffast-math -w $(COMPILER_FLAGS)
#LFLAGS = $(LINKER_FLAGS)

# Debug:
CFLAGS = -g -W -Wall $(COMPILER_FLAGS) -Wno-write-strings -Wno-unused-parameter -Wno-switch -Wno-reorder -DDEBUGMODE -DDEBUG -MMD -MP
CFLAGS_C = -g -MMD -MP
LFLAGS = $(LINKER_FLAGS)

DEPS = $(OBJS:.o=.d)

all : $(BINFILE)

$(BINFILE) : $(OBJS)
	@$(CC) $(OBJS) -o $(BINFILE) $(LFLAGS)

%.o : %.cpp
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.c
	@echo CC $<
	@$(CC_C) $(CFLAGS_C) -c $< -o $@

clean:
	@rm -f $(OBJS) $(DEPS)
	@rm -f $(BINFILE)

depend:
	@$(CC) -MM $(CFLAGS) $(SRCS) > Makefile.dep
	@$(CC_C) -MM $(CFLAGS_C) $(SRCS_C) >> Makefile.dep

info:
	@echo SRCS: $(SRCS) $(SRCS_C)
	@echo OBJS: $(OBJS)
	@echo $(BINFILE)

include Makefile.dep