cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("chemical-market-analyzer")

set(CMAKE_CXX_STANDARD 20)

# Set vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find curl
find_package(CURL CONFIG REQUIRED)

add_executable (chemical-market-analyzer "src/main.cpp")

# Link curl
target_link_libraries(chemical-market-analyzer PRIVATE CURL::libcurl)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chemical-market-analyzer PROPERTY CXX_STANDARD 20)
endif()