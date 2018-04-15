.DEFAULT_GOAL := all

UNAME := $(shell uname)
LDFLAGS := -pthread -lstdc++fs

ifeq ($(UNAME), Darwin)
	LIBS += -I/usr/local/include -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
endif

TARGET_EXEC ?= main

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src ./extras

CC := g++
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -std=c++17 $(INC_FLAGS) -MMD -MP -mavx -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused

all: CPPFLAGS += -g -O3
dev: CPPFLAGS += -g
prod: CPPFLAGS += -O3

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) -std=c++17 $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

all: $(BUILD_DIR)/$(TARGET_EXEC)

build: $(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

run:
	./build/main

up: all run

prod: clean build run

dev: up

load:
	./build/main -nets $(NET_DIR)

test:
	./build/main -test $(NET)

# no path defined for -play due to complexity of passing args through make
# sample call to -play is below, and assumes you are calling from the root of the project
# ./build/main -play NN_185gens/gen_185/nets/00 127.0.0.1:8080 test1 B