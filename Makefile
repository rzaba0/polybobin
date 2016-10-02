CC=g++
CFLAGS=-c --std=c++11 -Wall

# The flags below have to be used after specifying source file.
WXCONFIG_FLAGS=`wx-config --cxxflags --libs all`

BIN_DIR=bin
BUILD_DIR=build
SRC_DIR=src
TARGET=$(BIN_DIR)/polybobin

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) $(WXCONFIG_FLAGS) -o $(TARGET) -lGL

# Generic recipe for compilation.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $< $(WXCONFIG_FLAGS) -o $@

clean:
	rm -r $(BIN_DIR) $(BUILD_DIR)


