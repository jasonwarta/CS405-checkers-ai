.DEFAULT_GOAL := main
SHELL := /bin/bash

MAIN_EXEC := main
MAIN_SRC := src/main.cpp
MAIN_OBJ := build/$(MAIN_SRC).o

TESTS_EXEC := tests
TESTS_SRC := src/tests.cpp
TESTS_OBJ := build/$(TESTS_SRC).o

SERVER_EXEC := server
SERVER_SRC := src/server.cpp
SERVER_OBJ := build/$(SERVER_SRC).o

EXCLUDE := ! -path *$(MAIN_SRC) -and ! -path *$(TESTS_SRC) -and ! -path *$(SERVER_SRC)

UNAME := $(shell uname)
LDFLAGS := -pthread

ifeq ($(UNAME), Darwin)
	LIBS += -I/usr/local/include -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
endif

BUILD_DIR ?= build
SRC_DIRS ?= src extras

CC := g++

SRCS := $(shell find $(SRC_DIRS) $(EXCLUDE) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -std=c++17 $(INC_FLAGS) -MMD -MP -mavx -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused

-include $(DEPS)

MKDIR_P ?= mkdir -p

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

dev: CPPFLAGS += -g
prod: CPPFLAGS += -O3

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@


$(MAIN_OBJ): $(MAIN_SRC)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR)/$(MAIN_EXEC): $(OBJS)
	$(MAKE) $(MAIN_OBJ)
	$(CC) -std=c++17 $(OBJS) -o $@ $(LDFLAGS)


$(TESTS_OBJ): $(TESTS_SRC)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR)/$(TESTS_EXEC): $(OBJS)
	$(MAKE) $(TESTS_OBJ)
	$(CC) -std=c++17 $(OBJS) -o $@ $(LDFLAGS)


$(SERVER_OBJ): $(SERVER_SRC)
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR)/$(SERVER_EXEC): $(OBJS)
	$(MAKE) $(SERVER_OBJ)
	$(CC) -std=c++17 $(OBJS) -o $@ $(LDFLAGS)

main: OBJS += $(MAIN_OBJ)
main: LDFLAGS += -lstdc++fs
main: $(BUILD_DIR)/$(MAIN_EXEC)

tests: OBJS += $(TESTS_OBJ)
tests: $(BUILD_DIR)/$(TESTS_EXEC)

server: OBJS += $(SERVER_OBJ)
server: LDFLAGS += -lz -lssl -luv -luWS
server: $(BUILD_DIR)/$(SERVER_EXEC)


run-main: CPPFLAGS += -O3
run-main: main
	./build/main $(ARGS)

run-tests: CPPFLAGS += -O3
run-tests: tests
	./build/tests

run-server: CPPFLAGS += -O3
run-server: server
	./build/server


debug-main: CPPFLAGS += -g
debug-main: main
	./build/main $(ARGS)

debug-tests: CPPFLAGS += -g
debug-tests: tests
	./build/tests

debug-server: CPPFLAGS += -g
debug-server: server
	./build/server


help:
	@echo
	@echo "If switching between running and debugging, 'make clean' should be used between compiles"
	@echo
	@echo "Compile"
	@echo "    main"
	@echo "    tests"
	@echo "    server"
	@echo
	@echo "Compile with -O3 and run"
	@echo "    run-main"
	@echo "    run-tests"
	@echo "    run-server"
	@echo 
	@echo "Compile with -g and run"
	@echo "    debug-main"
	@echo "    debug-tests"
	@echo "    debug-server"
	@echo
	@echo
	@echo "'main can take several args to tweak functionality"
	@echo "to pass args to the executable, use one of the following"
	@echo "Load a specific set of nets, to resume training at a given point:"
	@echo "    run-main ARGS=\"-nets path/to/nets/dir\""
	@echo
	@echo "Load a specific net, and test against the piece count"
	@echo "    run-main ARGS=\"-test path/to/specific/net\""
	@echo
	@echo "Load a specific net, and play against an opponent through a skynet server."
	@echo "    serverAddress can be an IP or URL"
	@echo "    gameName is the name of your game"
	@echo "    color is any one of (R,r,red,B,b,black)"
	@echo
	@echo "    run-main ARGS=\"-play path/to/specific/net serverAddress gameName color\""
	@echo
	@echo
	@echo "'server' runs a server on 127.0.0.1:3000, for the player to compete against a neural net, and requires an net"
	@echo "    run-server ARGS=\"path/to/specific/net\""
	@echo
	@echo
