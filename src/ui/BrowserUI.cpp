#include "BrowserUI.hpp"
#include "browser/Browser.hpp"
#include <imgui.h>

void BrowserUI::draw(Browser &browser)
{
    // Start an overall window for the top UI
    ImGui::Begin("Top Bar");

    drawAddressBar(browser);
    drawToolbar(browser);

    ImGui::End();
}

void BrowserUI::drawAddressBar(Browser &browser)
{
    // A static buffer to hold the current URL text
    static char urlBuffer[256] = "http://example.com";

    // Label: "URL:" on the same line as the input
    ImGui::Text("URL:");
    ImGui::SameLine();

    // Draw a text field. If user presses Enter, returns true
    if (ImGui::InputText("##url", urlBuffer, IM_ARRAYSIZE(urlBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue))
    {
        // If user hits Enter, load that URL
        browser.loadURL(urlBuffer);
    }

    // A "Go" button after the text field
    ImGui::SameLine();
    if (ImGui::Button("Go"))
    {
        browser.loadURL(urlBuffer);
    }
}

void BrowserUI::drawToolbar(Browser &browser)
{
    // Simple row of buttons: Back, Forward, Refresh, Home, etc.

    if (ImGui::Button("Back"))
    {
        browser.goBack(); // If your Browser class has a back method
    }
    ImGui::SameLine();

    if (ImGui::Button("Forward"))
    {
        browser.goForward();
    }
    ImGui::SameLine();

    if (ImGui::Button("Refresh"))
    {
        browser.refresh();
    }
    ImGui::SameLine();

    if (ImGui::Button("Home"))
    {
        browser.loadURL("http://example.com");
    }
}
