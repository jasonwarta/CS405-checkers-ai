UNAME := $(shell uname)
LDFLAGS := -pthread -lstdc++fs -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

ifeq ($(UNAME), Darwin)
	LIBS += -I/usr/local/include -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
endif

TARGET_EXEC ?= main

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC := g++
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -std=c++17 $(INC_FLAGS) -MMD -MP -mavx 

all: CPPFLAGS += -g -O3
dev: CPPFLAGS += -g
prod: CPPFLAGS += -O3

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) -std=c++17 $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

all: $(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

run: 
	./build/main

up: all run

prod: clean up

dev: up

load:
	./build/main -nets $(NET_DIR)

testrun:
	./build/main -test $(NET)