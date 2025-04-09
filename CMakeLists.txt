# CMakeList.txt : CMake project for logfmtxx, modules support.
# https://www.kitware.com/import-cmake-the-experiment-is-over/
#
cmake_minimum_required (VERSION 3.28)
project ("logfmtxx")

set(CMAKE_CXX_STANDARD 23)

enable_testing()

# Add the header only configuration
add_library (logfmtxx INTERFACE include/logfmtxx.hpp )

add_subdirectory(module)
add_subdirectory(module-tests)
