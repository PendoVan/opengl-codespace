cmake_minimum_required(VERSION 3.10)
project(OpenGLApp)

# Usar el estándar C++17
set(CMAKE_CXX_STANDARD 17)

# Indicar cuál es el archivo fuente
add_executable(app main.cpp)
set(OpenGL_GL_PREFERENCE GLVND)

# Encontrar OpenGL en el sistema y enlazar las librerías a nuestro ejecutable
find_package(OpenGL REQUIRED)
target_link_libraries(app glfw GL GLEW)