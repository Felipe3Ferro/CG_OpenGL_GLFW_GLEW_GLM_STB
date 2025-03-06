# 🌍 Real-Time 3D Rendering with OpenGL  

This project is a **3D rendering study** using **OpenGL** with a terrain rendered in the center of the screen. The user can navigate the scene using the **WASD keys** and control the **camera with the mouse** for an immersive experience.  

## 🚀 Technologies Used  

- **OpenGL** – Graphics API for rendering  
- **GLFW** – Window and input handling  
- **GLEW** – OpenGL extension loader  
- **GLM** – Mathematics library for transformations  
- **STB** – Image loading library  
- **GLSL** – Shading language for rendering  

## 🎮 Controls  

| Key | Action |
|-----|--------|
| W   | Move forward |
| S   | Move backward |
| A   | Move left |
| D   | Move right |
| Mouse | Look around (camera movement) |

## 👅 Features  

- Real-time **terrain rendering** with OpenGL.
- **Camera movement** using keyboard and mouse.
- **Shader-based rendering** with GLSL.
- **Texture loading** using STB library.
- **Efficient mathematical operations** with GLM.

## 👅 Future Improvements  

- Implement **lighting effects** (Phong, Blinn-Phong, etc.).
- Add **textures and materials** for better realism.
- Optimize rendering using **vertex buffer objects (VBOs)**.
- Introduce **shadow mapping** for depth perception.

## 👅 Installation & Setup  

### 🔹 **Prerequisites**  

Make sure you have the following installed:  
- **C++ Compiler** (GCC, Clang, or MSVC)  
- **CMake** (for project build)  
- **GLFW, GLEW, GLM, STB** libraries  

### 🔹 **Building the Project**  

1. Clone this repository:  
   ```bash
   git clone https://github.com/Felipe3Ferro/CG_OpenGL_GLFW_GLEW_GLM_STB.git
   cd CG_OpenGL_GLFW_GLEW_GLM_STB
   ```

2. Create a **build** directory and run CMake:  
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the executable:  
   ```bash
   ./OpenGL_Terrain
   ```
