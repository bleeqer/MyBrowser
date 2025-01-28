#include "HTTPClient.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <iostream>

// This callback is called by libcurl as it receives data.
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    // Cast user pointer to std::string
    std::string *buffer = static_cast<std::string *>(userp);
    // Calculate actual size
    size_t totalSize = size * nmemb;
    // Append to the string’s buffer
    buffer->append(static_cast<char *>(contents), totalSize);
    return totalSize; // Return the number of bytes we handled
}

std::string HTTPClient::get(const std::string &url)
{
    // Initialize the output string
    std::string responseBody;

    // Initialize a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to init CURL\n";
        return "";
    }

    // Set the URL we want
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Provide a callback to store the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "Curl request failed: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        return "";
    }

    // Optionally, check HTTP response code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode != 200)
    {
        std::cerr << "HTTP status code: " << httpCode << "\n";
        // You might decide to return "" or partial content
    }

    // Clean up
    curl_easy_cleanup(curl);

    return responseBody;
}
