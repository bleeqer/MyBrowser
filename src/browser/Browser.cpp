#include "Browser.hpp"
#include "../net/HTTPClient.hpp"  // or your networking code
#include "../parser/HTMLParser.hpp"  // or your parser
#include <iostream>

Browser::Browser()
{
    // Constructor logic (if any)
}

Browser::~Browser()
{
    // Cleanup, if needed
}

void Browser::loadURL(const std::string& url)
{
    // 1. Download the page
    HTTPClient client;
    std::string html = client.get(url);   // pseudo-code, implement your GET request

    // 2. Create a new Page
    m_currentPage = std::make_shared<Page>();

    // 3. Parse HTML into the Page’s DOM
    HTMLParser parser;
    m_currentPage->loadHTML(parser, html);

    // Log info
    std::cout << "Loaded URL: " << url << "\n";
}

void Browser::update()
{
    // e.g. handle scripts, resource loads, or reflow triggers
    if (m_currentPage) {
        m_currentPage->update();
    }
}

std::shared_ptr<Page> Browser::currentPage() const
{
    return m_currentPage;
}
