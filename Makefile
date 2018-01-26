UNAME := $(shell uname)
LDFLAGS := -lz -lssl -luv -luWS

ifeq ($(UNAME), Darwin)
	LIBS += -I/usr/local/include -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
endif

TARGET_EXEC ?= server

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

CC := g++
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

fCPPFLAGS ?= -std=c++14 $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) -std=c++14 $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIBS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

run: 
	./build/server

up: $(BUILD_DIR)/$(TARGET_EXEC) run
