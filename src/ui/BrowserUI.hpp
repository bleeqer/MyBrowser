#pragma once

class Browser; // forward-declare

class BrowserUI
{
public:
    // Called once per frame to render all UI panels/windows
    void draw(Browser &browser);

private:
    void drawAddressBar(Browser &browser);

    void drawToolbar(Browser &browser);
};
