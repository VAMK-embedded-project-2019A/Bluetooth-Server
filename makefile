CC = gcc
CXX = g++

BIN = bt

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

OBJS := $(patsubst $(SRC_DIR)/%.cpp, %, $(wildcard $(SRC_DIR)/*.cpp))
DEPS_OBJS = $(filter-out main, $(OBJS))

INCLUDE_FILES = $(foreach OBJ, $(DEPS_OBJS), $(INC_DIR)/$(OBJ).h)
OBJ_FILES = $(foreach OBJ, $(OBJS), $(OBJ_DIR)/$(OBJ).o)

INCLUDE_OPTS = -iquote include
LIB_OPTS = -lbluetooth -lcrypto
TOOL_OPTS = -std=c++11 -g 

OPTS_OBJ = $(INCLUDE_OPTS) $(TOOL_OPTS)
OPTS_BIN = $(INCLUDE_OPTS) $(TOOL_OPTS) $(LIB_OPTS)

first: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CXX) -o $(BIN) $(OBJ_FILES) $(OPTS_BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DEPS)
	$(CXX) $< -c -o $@ $(OPTS_OBJ)

clean:
	rm -f $(OBJ_DIR)/*.o 
