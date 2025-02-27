cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    include(FetchContent)
    message(STATUS "Fetching nlohmann_json")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3 # Change to the latest stable release
    )
    message(STATUS "Fetching nlohmann_json done")
    FetchContent_MakeAvailable(nlohmann_json)
    message(STATUS "nlohmann_json found")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create library
add_library(${PROJECT_NAME} INTERFACE)

# Link libraries
target_link_libraries(${PROJECT_NAME} INTERFACE
    nlohmann_json::nlohmann_json
)

# Set include directories for users of this library
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
