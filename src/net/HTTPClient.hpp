#pragma once
#include <string>

/**
 * @brief A simple blocking HTTP client that retrieves data from a given URL.
 */
class HTTPClient {
public:
    /**
     * @brief Performs a GET request for the specified URL.
     * @param url The URL to fetch (e.g. "http://example.com")
     * @return The response body as a string (or empty string if error).
     */
    std::string get(const std::string& url);
};
