cmake_minimum_required(VERSION 3.20)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set vcpkg toolchain before project() command
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" 
        CACHE STRING "Vcpkg toolchain file")
endif()

project("chemical-market-analyzer")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find curl with more detailed error reporting
find_package(CURL CONFIG REQUIRED)
if(NOT CURL_FOUND)
    message(FATAL_ERROR "CURL not found. Please ensure it's installed via vcpkg: vcpkg install curl:x64-windows")
endif()

add_executable(chemical-market-analyzer "src/main.cpp")		

# Link curl with improved target specification
target_link_libraries(chemical-market-analyzer PRIVATE CURL::libcurl)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chemical-market-analyzer PROPERTY CXX_STANDARD 20)
endif()
