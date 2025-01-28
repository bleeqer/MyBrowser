#include <iostream>
#include "browser/Page.hpp"
// #include "layout/LayoutEngine.hpp"
#include <browser/Browser.hpp>
#include <GLFW/glfw3.h>
#include <ui/BrowserUI.hpp>

#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/backends/imgui_impl_glfw.h"
#include "../third_party/imgui/backends/imgui_impl_opengl3.h"


int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "My Browser Example", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    BrowserUI browserUI;
    Page myPage;
    Browser myBrowser;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // 1) Poll window events (mouse, keyboard, etc.)
        glfwPollEvents();

        // 2) Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 3) Update page logic
        myPage.update();


        // 4) Draw the address bar
        //    Optionally wrap it in its own ImGui window if desired:
        ImGui::Begin("Browser"); // Begin an ImGui window titled "Browser"

        browserUI.draw(myBrowser);


        ImGui::End(); // End the "Browser" window

        // 5) Render the ImGui frame
        ImGui::Render();

        // 6) Clear the screen
        glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 7) Draw ImGui over the cleared background
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 8) Swap front/back buffers
        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
