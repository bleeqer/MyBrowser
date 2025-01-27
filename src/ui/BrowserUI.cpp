#include "BrowserUI.hpp"
#include "../browser/Browser.hpp"
#include <string>

void drawAddressBar(Browser& browser)
{
    static char urlBuffer[256] = "http://example.com";

    ImGui::Begin("Address Bar");

    ImGui::Text("URL:");
    ImGui::SameLine();

    if (ImGui::InputText("##url", urlBuffer, IM_ARRAYSIZE(urlBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        browser.loadURL(urlBuffer);
    }

    ImGui::SameLine();
    if (ImGui::Button("Go"))
    {
        browser.loadURL(urlBuffer);
    }

    ImGui::End();
}
