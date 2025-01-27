// main.cpp
#include <GLFW/glfw3.h>            // from your system or package manager
#include "imgui.h"                 // from your imgui folder
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>

// This is optional, but recommended if you need modern GL symbols
// e.g. #include <glad/glad.h> or #include <GL/glew.h>
// For a quick test, might be enough with <GLFW/glfw3.h> alone.

int main()
{
    // 1) Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    // 2) Set up OpenGL context (e.g., 3.0 or 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // If you prefer core profile:
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3) Create a window + OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "My GLFW + OpenGL3 ImGui", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // (Optional) Enable vsync
    glfwSwapInterval(1);

    // If using GLAD or GLEW, you'd initialize here (e.g. gladLoadGL()).

    // 4) Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();

    // 5) Initialize ImGui backends: GLFW and OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // Pass your GLSL version string. For a 3.3 core profile: "#version 330"
    ImGui_ImplOpenGL3_Init("#version 330");

    // 6) Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // 7) Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
