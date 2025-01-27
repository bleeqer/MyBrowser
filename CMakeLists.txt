cmake_minimum_required(VERSION 3.10)
project(MyBrowser)

# 1) Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2) Find libraries
find_package(OpenGL REQUIRED)            # e.g. -lGL on Linux, or frameworks on macOS
find_package(GLFW3 REQUIRED)            # Provided by Homebrew or system package

include_directories(/opt/homebrew/include)
include_directories(third_party/imgui)

# 3) Build the ImGui library
add_library(imgui
        # Core ImGui
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_demo.cpp      # Optional (for the demo window)

        # Backends
        third_party/imgui/backends/imgui_impl_glfw.cpp
        third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Let any target linking 'imgui' see ImGui headers:
target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
)

# 4) Build the 'browser' library from your own source files
#    We'll recursively grab all .cpp files in src/
file(GLOB_RECURSE BROWSER_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(browser ${BROWSER_SOURCES})

target_include_directories(browser PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 5) Main executable (entry point is src/main.cpp)
#    but since we already have main.cpp in BROWSER_SOURCES,
#    you can either remove it from the GLOB above or create a separate target.
#    Here, let's assume main.cpp is part of the 'browser' library.
#    Alternatively, do not add main.cpp in the GLOB, and just place it here.
add_executable(MyBrowser src/main.cpp)

# 6) Link everything into MyBrowser
target_link_libraries(MyBrowser PRIVATE
        imgui      # The ImGui library
        browser    # Your app/library code
        glfw       # Found by find_package(GLFW3)
        OpenGL::GL # The official CMake target for OpenGL
)

# 7) macOS-specific frameworks
#    Depending on your GLFW build, you may also need Cocoa, IOKit, CoreVideo, etc.
if(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(COREVIDEO CoreVideo)

    if(COCOA AND IOKIT AND COREVIDEO)
        target_link_libraries(MyBrowser PRIVATE ${COCOA} ${IOKIT} ${COREVIDEO})
    endif()
endif()
