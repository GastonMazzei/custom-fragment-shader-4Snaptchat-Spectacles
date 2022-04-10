cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(ShaderToy)

# Search here for AppKit so we can conditionally include/exclude
# apps based on AppKit runtime availability.
find_package(VOSAppKit REQUIRED)

# Include sample applications.
add_subdirectory(ShaderToy)
