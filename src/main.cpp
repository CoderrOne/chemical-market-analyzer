#include <curl/curl.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <vector>
#include <algorithm>

using json = nlohmann::json;

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
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return response.data;
}

void displayMenu() {
    std::cout << "\nAvailable Operations:" << std::endl;
    std::cout << "1. Choose a Producer Tag to get Data from" << std::endl;
    std::cout << "2. Calculate average for a date range" << std::endl;
    std::cout << "3. Find highest/lowest values" << std::endl;
    std::cout << "4. Filter by value range" << std::endl;
	std::cout << "5. Display latest N entries" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Enter your choice (1-5): ";
}


void showLatestEntries(const json& data, int n) {
    if (!data.contains("observations") || !(data.at("observations").is_array())) {
        std::cerr << "Error: 'observations' is missing or not an array in the JSON data." << std::endl;
        return;
    }
    const auto& observations = data["observations"];
    int count = (std::min)(n, static_cast<int>(observations.size()));
    
    std::cout << "\nLatest " << count << " entries:" << std::endl;
    std::cout << std::setw(12) << "Date" << std::setw(15) << "Value" << std::endl;
    std::cout << std::string(27, '-') << std::endl;
    
    for (int i = observations.size() - count; i < observations.size(); ++i) {
        std::cout << std::setw(12) << observations[i]["date"].get<std::string>()
                  << std::setw(15) << observations[i]["value"].get<std::string>() << std::endl;
    }
}

void calculateAverage(const json& data) {
    std::string startDate, endDate;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;

    double sum = 0.0;
    int count = 0;
    const auto& observations = data["observations"];

    for (const auto& obs : observations) {
        std::string date = obs["date"];
        if (date >= startDate && date <= endDate && !obs["value"].get<std::string>().empty()) {
            sum += std::stod(obs["value"].get<std::string>());
            count++;
        }
    }

    if (count > 0) {
        std::cout << "\nAverage value between " << startDate << " and " << endDate 
                  << ": " << std::fixed << std::setprecision(2) << (sum / count) << std::endl;
    } else {
        std::cout << "No valid data found in the specified date range." << std::endl;
    }
}

void findExtremeValues(const json& data) {
    double max_val = -std::numeric_limits<double>::infinity();
    double min_val = std::numeric_limits<double>::infinity();
    std::string max_date, min_date;
    
    const auto& observations = data["observations"];
    for (const auto& obs : observations) {
        if (!obs["value"].get<std::string>().empty()) {
            double value = std::stod(obs["value"].get<std::string>());
            if (value > max_val) {
                max_val = value;
                max_date = obs["date"];
            }
            if (value < min_val) {
                min_val = value;
                min_date = obs["date"];
            }
        }
    }

    std::cout << "\nHighest value: " << max_val << " on " << max_date << std::endl;
    std::cout << "Lowest value: " << min_val << " on " << min_date << std::endl;
}

void filterByRange(const json& data) {
    double min_val, max_val;
    std::cout << "Enter minimum value: ";
    std::cin >> min_val;
    std::cout << "Enter maximum value: ";
    std::cin >> max_val;

    std::cout << "\nEntries between " << min_val << " and " << max_val << ":" << std::endl;
    std::cout << std::setw(12) << "Date" << std::setw(15) << "Value" << std::endl;
    std::cout << std::string(27, '-') << std::endl;

    const auto& observations = data["observations"];
    for (const auto& obs : observations) {
        if (!obs["value"].get<std::string>().empty()) {
            double value = std::stod(obs["value"].get<std::string>());
            if (value >= min_val && value <= max_val) {
                std::cout << std::setw(12) << obs["date"].get<std::string>()
                          << std::setw(15) << value << std::endl;
            }
        }
    }
}

json data;

int parseData(const std::string& ID, const std::string& apiKey) {
    std::cout << "\nFetching chemical market data..." << std::endl;
    std::string chemicalData = getFredData(ID, apiKey);

    if (chemicalData.empty()) {
        std::cout << "No data received from FRED API" << std::endl;
        return 1;
    }

    data = json::parse(chemicalData);
    return 1;
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

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cout << "CURL initialization has failed." << std::endl;
        return 1;
    }
    
    std::cout << "CURL initialized: " << curl_version() << std::endl;
    curl_easy_cleanup(curl);


    
    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int n;
                std::cout << "Enter choice of Producer Price Index to display: ";
                std::cout << R"(
                            Producer Price Index by Industry: Chemical Manufacturing [1] (PCU325325)
                            Producer Price Index by Commodity: Chemicals and Allied Products: Industrial Chemicals [2] (WPU061)
                            Producer Price Index by Commodity: Chemicals and Allied Products [3] (WPU06)
                    )";
                std::cin >> n;

                switch (n) {
                    case 1:
                        parseData("PCU325325", apiKey);
                        break;
                    case 2:
                        parseData("WPU061", apiKey);
                        break;
                    case 3:
                        parseData("WPU06", apiKey);
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
						continue;
                }

                break;
            }
            case 2:
                calculateAverage(data);
                break;
            case 3:
                findExtremeValues(data);
                break;
            case 4:
                filterByRange(data);
                break;
            case 5:
                int n;
                std::cout << "Enter choice of Latest Entries: ";
                std::cin >> n;
                showLatestEntries(data, n);
                break;
            case 6:
                std::cout << "Exiting program..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 5);

    return 0;
}