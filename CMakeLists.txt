cmake_minimum_required(VERSION 3.10)

project(MyBrowser)
include(FetchContent)

# 1) Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2) Find libraries
find_package(OpenGL REQUIRED)        # e.g. -lGL on Linux, or frameworks on macOS
message(STATUS "OpenGL::GL = ${OPENGL_gl_LIBRARY}")

find_package(GLFW3 REQUIRED)         # Provided by Homebrew/system package
message(STATUS "GLFW3::GLFW = ${GLFW_LIBRARIES}")

find_package(CURL REQUIRED)          # For HTTPClient
message(STATUS "CURL::libcurl = ${CURL_LIBRARIES}")

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
FetchContent_MakeAvailable(googletest)

# If needed, add any global include paths:
include_directories(/opt/homebrew/include)
include_directories(third_party/imgui)

# 3) Build the ImGui library
add_library(imgui
        # Core ImGui
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imgui_tables.cpp

        # Backends
        third_party/imgui/backends/imgui_impl_glfw.cpp
        third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Let any target linking 'imgui' see ImGui headers:
target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
)

# 4) Build the 'browser' library from your source files
#    We'll recursively grab all .cpp files in src/
file(GLOB_RECURSE BROWSER_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
)

add_library(browser
        ${BROWSER_SOURCES}
)

target_include_directories(browser PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 5) Main executable (entry point is src/main.cpp).
#    We assume main.cpp is included in BROWSER_SOURCES already.
#    Alternatively, remove main.cpp from the GLOB and list it separately.
add_executable(MyBrowser src/main.cpp)

# 6) Link everything into MyBrowser
target_link_libraries(MyBrowser PRIVATE
        imgui          # The ImGui library
        browser        # Your app/library code
        glfw           # Found by find_package(GLFW3)
        OpenGL::GL     # The official CMake target for OpenGL
        CURL::libcurl  # Found by find_package(CURL)
)

# 7) macOS-specific frameworks (if needed)
if(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(COREVIDEO CoreVideo)
    if(COCOA AND IOKIT AND COREVIDEO)
        target_link_libraries(MyBrowser PRIVATE ${COCOA} ${IOKIT} ${COREVIDEO})
    endif()
endif()

file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/**/*.cpp"
)

add_executable(MyBrowserTests
        ${TEST_SOURCES}
)

# Link your browser library and GTest
target_link_libraries(MyBrowserTests PRIVATE
        browser       # your app/library code
        imgui
        GTest::gtest_main   # gtest_main provides main() entry for testing
)

# If your tests need the same include dirs:
target_include_directories(MyBrowserTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends
        # Possibly also: ${CMAKE_CURRENT_SOURCE_DIR}/src
)