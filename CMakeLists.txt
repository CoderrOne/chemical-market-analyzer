# CMakeList.txt : CMake project for chemical-market-analyzer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("chemical-market-analyzer")

# Add source to this project's executable.
add_executable (chemical-market-analyzer "chemical-market-analyzer.cpp" "chemical-market-analyzer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET chemical-market-analyzer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
