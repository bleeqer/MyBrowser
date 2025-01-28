#pragma once
#include <memory>
#include <string>
#include <vector>

class Page; // forward declare

class Browser
{
public:
    Browser();

    ~Browser();

    void loadURL(const std::string &url);

    void update();

    // Navigation history:
    void goBack();

    void goForward();

    void refresh();

    // If you need direct access to the current Page object:
    std::shared_ptr<Page> currentPage() const;

private:
    std::shared_ptr<Page> m_currentPage;

    // Simple URL history
    std::vector<std::string> m_history;
    int m_historyIndex = -1; // which element of m_history is current
};
