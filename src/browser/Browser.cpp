#include "Browser.hpp"
#include "../net/HTTPClient.hpp"   // or your networking code
#include "../parser/HTMLParser.hpp" // or your parser
#include "browser/Page.hpp"        // for Page
#include <iostream>

Browser::Browser()
{
    // Constructor logic (if any)
}

Browser::~Browser()
{
    // Cleanup, if needed
}

void Browser::loadURL(const std::string &url)
{
    // If we're adding a new page in the history, remove anything "ahead" of the current index
    // in case the user had gone back. This is how typical browsers handle a new nav after back.
    if (m_historyIndex >= 0 && (size_t) m_historyIndex < m_history.size() - 1)
    {
        m_history.erase(m_history.begin() + m_historyIndex + 1, m_history.end());
    }

    // Append the new URL and set index to the end
    m_history.push_back(url);
    m_historyIndex = static_cast<int>(m_history.size()) - 1;

    // 1. Download the page
    HTTPClient client;
    std::string html = client.get(url); // pseudo-code, implement your GET request

    // 2. Create a new Page
    m_currentPage = std::make_shared<Page>();

    // 3. Parse HTML into the Page’s DOM
    // HTMLParser parser;
    // m_currentPage->loadHTML(parser, html);

    // Log info
    std::cout << "Loaded URL: " << url << "\n";
}

void Browser::update()
{
    // e.g. handle scripts, resource loads, or reflow triggers
    if (m_currentPage)
    {
        m_currentPage->update();
    }
}

std::shared_ptr<Page> Browser::currentPage() const
{
    return m_currentPage;
}

void Browser::goBack()
{
    // Move back in history, if possible
    if (m_historyIndex > 0)
    {
        --m_historyIndex;
        std::string url = m_history[m_historyIndex];
        std::cout << "Going Back to: " << url << "\n";
        loadURL(url);
    } else
    {
        std::cout << "Cannot go back; already at earliest history entry.\n";
    }
}

void Browser::goForward()
{
    // Move forward in history, if possible
    if ((size_t) m_historyIndex < m_history.size() - 1)
    {
        ++m_historyIndex;
        std::string url = m_history[m_historyIndex];
        std::cout << "Going Forward to: " << url << "\n";
        loadURL(url);
    } else
    {
        std::cout << "Cannot go forward; already at latest history entry.\n";
    }
}

void Browser::refresh()
{
    if (m_historyIndex >= 0 && (size_t) m_historyIndex < m_history.size())
    {
        std::string url = m_history[m_historyIndex];
        std::cout << "Refreshing: " << url << "\n";
        loadURL(url);
    } else
    {
        std::cout << "Cannot refresh; no current page.\n";
    }
}
