CXX = g++

CXXFLAGS = --std=c++20 -pthread -Wall -Wextra -I./headers
DEBUGFLAGS = -g

TARGET = program

OUT_DIR = build
SRC_DIR = sources

SRC = main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OUT_DIR)/sources/%.o)

$(OUT_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET)
	./$(OUT_DIR)/$(TARGET)

$(OUT_DIR)/sources/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OUT_DIR)/sources
	$(CXX) $(CXXFLAGS) -c $< -o $@

debugger: CXXFLAGS += $(DEBUGFLAGS)
debugger: $(OUT_DIR)/$(TARGET)
	gdb $(OUT_DIR)/$(TARGET)

clean:
	rm -rf $(OUT_DIR)

run:
	./$(OUT_DIR)/$(TARGET)
