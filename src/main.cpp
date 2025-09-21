#include <curl/curl.h>
#include <iostream>
#include <cstdlib>
#include <string>

struct APIResponse {
    std::string data;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, APIResponse* response) {
    size_t totalSize = size * nmemb;
    response->data.append((char*)contents, totalSize);
    return totalSize;
}

std::string getFredData(const std::string& seriesId, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    APIResponse response;

    if (curl) {
        std::string url = "https://api.stlouisfed.org/fred/series/observations?series_id="
            + seriesId + "&api_key=" + apiKey + "&file_type=json";

        std::cout << "Fetching from: " << url.substr(0, 50) << "..." << std::endl;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return response.data;
}

int main() {
    std::cout << "Chemical Market Analyzer - FRED Data Fetcher" << std::endl;
    std::cout << "=============================================" << std::endl;

    const char* apiKeyPtr = std::getenv("FRED_API_KEY");
    if (!apiKeyPtr) {
        std::cerr << "Error: FRED_API_KEY environment variable is not set." << std::endl;
        return 1;
    }

    std::string apiKey = std::string(apiKeyPtr);
    std::cout << "API Key loaded successfully!" << std::endl;

    // Test CURL
    CURL* curl = curl_easy_init();
    if (curl) {
        std::cout << "CURL initialized: " << curl_version() << std::endl;
        curl_easy_cleanup(curl);
    }
    else {
        std::cout << "CURL initialization has failed." << std::endl;
        return 1;
    }

    std::cout << "\nFetching chemical market data..." << std::endl;

    // Get Producer Price Index for Chemicals
    std::string chemicalData = getFredData("PCU325325", apiKey);

    if (!chemicalData.empty()) {
        std::cout << "\nChemical Market Data (JSON):" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << chemicalData.substr(0, 500) << "..." << std::endl; // Show first 500 chars
        std::cout << "\nData fetched successfully! Total size: " << chemicalData.length() << " characters" << std::endl;
    }
    else {
        std::cout << "No data received from FRED API" << std::endl;
    }

    return 0;
}