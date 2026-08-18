rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = main.cpp $(call rwildcard,src,*.cpp)
OBJS = $(SRCS:.cpp=.o)
BINFILE = ./bigpotatofile.exe
CC = g++

COMPILER_FLAGS = -finline-functions -std=c++17 -Iinclude
LINKER_FLAGS = -lm -lpthread

# Release:
#CFLAGS = -O3 -fomit-frame-pointer -ffast-math -w $(COMPILER_FLAGS)
#LFLAGS = $(LINKER_FLAGS)

# Debug:
CFLAGS = -g -W -Wall $(COMPILER_FLAGS) -Wno-write-strings -Wno-unused-parameter -Wno-switch -Wno-reorder -DDEBUGMODE -DDEBUG -MMD -MP
LFLAGS = $(LINKER_FLAGS)

DEPS = $(OBJS:.o=.d)

all : $(BINFILE)

$(BINFILE) : $(OBJS)
	@$(CC) $(OBJS) -o $(BINFILE) $(LFLAGS)

%.o : %.cpp
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(DEPS)
	@rm -f $(BINFILE)

depend:
	@$(CC) -MM $(CFLAGS) $(SRCS) > Makefile.dep

info:
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo $(BINFILE)


include Makefile.dep

