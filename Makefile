# ========================
# Compiler and flags
# ========================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra \
	-Iinclude \
	-Iimgui \
	-I$(GLFW_INCLUDE)

# ========================
# Detect platform for linker
# ========================
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS = -lglfw -lGL -ldl -lpthread
	GLFW_INCLUDE = /usr/include
	GLFW_LIB = /usr/lib
else ifeq ($(OS),Windows_NT)
	# Windows (MinGW)
	LDFLAGS = -L$(GLFW_LIB) -lglfw3 -lopengl32 -lgdi32
	GLFW_INCLUDE = glfw/include
	GLFW_LIB = glfw/lib-mingw-w64
endif

# ========================
# Directories
# ========================
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
IMGUI_DIR = imgui

# ========================
# Target executable
# ========================
TARGET = $(BIN_DIR)/wolf_game

ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
endif

# ========================
# Source files
# ========================
SRC_CPP   = $(wildcard $(SRC_DIR)/*.cpp)
IMGUI_CPP = \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_impl_glfw.cpp \
	$(IMGUI_DIR)/imgui_impl_opengl3.cpp

# ========================
# Object files
# ========================
SRC_OBJ   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))
IMGUI_OBJ = $(patsubst $(IMGUI_DIR)/%.cpp,$(OBJ_DIR)/imgui_%.o,$(IMGUI_CPP))
OBJECTS   = $(SRC_OBJ) $(IMGUI_OBJ)

# ========================
# Default target
# ========================
all: $(TARGET)

# ========================
# Link executable
# ========================
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# ========================
# Compile project sources
# ========================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ========================
# Compile ImGui sources
# ========================
$(OBJ_DIR)/imgui_%.o: $(IMGUI_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ========================
# Create directories
# ========================
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# ========================
# Clean build
# ========================
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
