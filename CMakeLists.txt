cmake_minimum_required(VERSION 3.10)
project(MyBrowser)

# Turn on C++17 (adjust as needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/include)
# 1) Find GLFW (assumes you installed via system package or Homebrew)
find_package(GLFW3 REQUIRED)

# 2) Add the ImGui source files to a library target
add_library(imgui
    # Core ImGui
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    # Optional demo
    third_party/imgui/imgui_demo.cpp

    # Backends
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Optionally include ImGui headers
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
)

# 3) Create your main executable
add_executable(MyBrowser src/main.cpp)

# 4) Link everything
#    - Link to 'imgui' library we just created
#    - Link to 'GLFW3::GLFW3' from the find_package call
#    - Also link to OpenGL (on Linux/macOS) or opengl32.lib on Windows
find_package(OpenGL REQUIRED)
if(APPLE)
    # On macOS, you might need "-framework Cocoa -framework OpenGL" etc. 
    target_link_libraries(MyBrowser PRIVATE
        imgui
        glfw
        OpenGL::GL
    )
elseif(WIN32)
    # On Windows, link opengl32.lib, GDI32, etc. if needed
    target_link_libraries(MyBrowser PRIVATE
        imgui
        glfw
        OpenGL::GL
    )
else()
    # On Linux
    target_link_libraries(MyBrowser PRIVATE
        imgui
        glfw
        OpenGL::GL
    )
endif()
