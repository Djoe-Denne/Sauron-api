cmake_minimum_required(VERSION 3.20)

project(sauron_sdk VERSION 1.0.0 LANGUAGES CXX)
option(AUTO_INSTALL_DEPS "Automatically install dependencies" ON)

# Add the client SDK
add_subdirectory(cpp-sdk)

