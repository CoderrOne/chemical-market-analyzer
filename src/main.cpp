#include <curl/curl.h>
#include <iostream>

int main() {
    // Just test if curl initializes
    CURL* curl = curl_easy_init();
    if (curl) {
        std::cout << "CURL initialized successfully!" << std::endl;
        curl_easy_cleanup(curl);

        // Test a simple request
        std::cout << "CURL version: " << curl_version() << std::endl;
    }
    else {
        std::cout << "CURL initialization failed!" << std::endl;
    }

    return 0;
}