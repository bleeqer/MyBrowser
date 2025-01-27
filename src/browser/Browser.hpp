#pragma once
#include <memory>
#include <string>
#include "Page.hpp"

// Simple Browser interface that can load a URL into a Page.
class Browser {
public:
    Browser();
    ~Browser();

    // Load a URL: fetch, parse, etc.
    void loadURL(const std::string& url);

    // Per-frame or periodic updates (scripts, timers, etc.).
    void update();

    // Get the current Page object (might be null if not loaded yet).
    std::shared_ptr<Page> currentPage() const;

private:
    // The active page (in a real browser, you might have multiple tabs).
    std::shared_ptr<Page> m_currentPage;
};
