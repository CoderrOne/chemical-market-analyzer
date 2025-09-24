# Chemical Market Analyzer
A C++ application that fetches and analyzes chemical market pricing data from the Federal Reserve Economic Data (FRED) API to identify market trends, volatility patterns, and pricing dynamics in the chemical industry.

## Features

Real-time Data Fetching: Integrates with FRED API to retrieve Producer Price Index data for chemicals
Secure API Key Management: Uses environment variables to protect sensitive API credentials
HTTP Client Implementation: Built with libcurl for reliable data transmission
Cross-platform Build System: CMake configuration with vcpkg dependency management
Market Data Analysis: Processes chemical pricing trends and market dynamics

## Technical Stack

Language: C++20
Build System: CMake 3.16+
HTTP Library: libcurl (via vcpkg)
Package Manager: vcpkg
Data Source: Federal Reserve Economic Data (FRED) API
Platform: Windows/Linux/macOS compatible
