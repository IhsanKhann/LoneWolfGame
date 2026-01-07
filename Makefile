# ========================
# Compiler
# ========================
CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra \
	-Iinclude \
	-Iimgui \
	-Iimgui/backends \
	-ID:/glfw-3.4/glfw-3.4/include

# ========================
# GLFW (Windows MinGW)
# ========================
GLFW_LIB_DIR = D:/glfw-3.4/glfw-3.4/build/src
LDFLAGS = -L$(GLFW_LIB_DIR) -lglfw3 -lopengl32 -lgdi32

# ========================
# Directories
# ========================
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
IMGUI_DIR = imgui
IMGUI_BACKENDS = imgui/backends

# ========================
# Target
# ========================
TARGET = $(BIN_DIR)/wolf_game.exe

# ========================
# Source files
# ========================
SRC_CPP = $(wildcard $(SRC_DIR)/*.cpp)

IMGUI_CPP = \
	imgui/imgui.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_demo.cpp \
	imgui/imgui_widgets.cpp \
	imgui/imgui_tables.cpp

IMGUI_BACKEND_CPP = \
	imgui/backends/imgui_impl_glfw.cpp \
	imgui/backends/imgui_impl_opengl3.cpp

# ========================
# Object files
# ========================
SRC_OBJ = $(patsubst src/%.cpp,obj/%.o,$(SRC_CPP))
IMGUI_OBJ = $(patsubst imgui/%.cpp,obj/imgui_%.o,$(IMGUI_CPP))
IMGUI_BACKEND_OBJ = $(patsubst imgui/backends/%.cpp,obj/imgui_backends/%.o,$(IMGUI_BACKEND_CPP))

OBJECTS = $(SRC_OBJ) $(IMGUI_OBJ) $(IMGUI_BACKEND_OBJ)

# ========================
# Default target
# ========================
all: dirs $(TARGET)

# ========================
# Link
# ========================
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# ========================
# Compile rules
# ========================
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/imgui_%.o: imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/imgui_backends/%.o: imgui/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ========================
# Create directories
# ========================
dirs:
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	if not exist $(OBJ_DIR)\imgui_backends mkdir $(OBJ_DIR)\imgui_backends

# ========================
# Clean
# ========================
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

.PHONY: all clean dirs
