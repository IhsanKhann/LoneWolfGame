# ?? Wolf Game (C++ / OpenGL / ImGui / GLFW)

This is a C++ game project built using **OpenGL**, **GLFW**, and **Dear ImGui**.
The project is designed to be **easy to clone, build, and run** using **MinGW (g++)**
without relying on any IDE.

---

## ?? Tech Stack

- **Language:** C++17
- **Compiler:** g++ (MinGW)
- **Graphics:** OpenGL
- **Windowing/Input:** GLFW 3.4
- **GUI:** Dear ImGui
- **Build System:** Makefile

---

## ?? Project Structure

Game/
�
+-- src/ # Game source files (.cpp)
+-- include/ # Project header files (.h)
�
+-- imgui/ # Dear ImGui source (included in repo)
� +-- imgui.cpp
� +-- imgui_draw.cpp
� +-- imgui_widgets.cpp
� +-- imgui_tables.cpp
� +-- imgui_demo.cpp
� +-- backends/
� +-- imgui_impl_glfw.cpp
� +-- imgui_impl_opengl3.cpp
�
+-- obj/ # Compiled object files (auto-generated)
+-- bin/ # Final executable (auto-generated)
�
+-- Makefile # Build instructions
+-- .gitignore
+-- README.md

yaml
Copy code

?? **`obj/` and `bin/` are NOT committed to GitHub**

---

## ?? Requirements (One-Time Setup)

Each team member must install the following **once**:

---

### 1?? MinGW (g++)

Install MinGW-w64 and ensure it is added to **PATH**.

Verify installation:
```bash
g++ --version
mingw32-make --version
If these commands work, MinGW is correctly installed.
