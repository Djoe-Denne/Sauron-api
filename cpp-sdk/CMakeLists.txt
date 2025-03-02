cmake_minimum_required(VERSION 3.20)

project(sauron_sdk VERSION 1.0.0 LANGUAGES CXX)
option(AUTO_INSTALL_DEPS "Automatically install dependencies" ON)

# Check if the target already exists
if(NOT TARGET sauron-sdk)
    # Create the interface library
    add_library(sauron-sdk INTERFACE)
    
    # Add include directories
    target_include_directories(sauron-sdk INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    
    # Add alias for the library
    add_library(sauron_sdk::sauron-sdk ALIAS sauron-sdk)
    
    # Install the library
    install(TARGETS sauron-sdk
        EXPORT sauron-sdk-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )
    
    # Install the include files
    install(DIRECTORY include/
        DESTINATION include
    )
    
    # Install the export file
    install(EXPORT sauron-sdk-targets
        FILE sauron-sdk-targets.cmake
        NAMESPACE sauron_sdk::
        DESTINATION lib/cmake/sauron-sdk
    )
    
    # Install the config file
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sauron-sdk-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/sauron-sdk-config.cmake
        INSTALL_DESTINATION lib/cmake/sauron-sdk
    )
    
    # Install the config file
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/sauron-sdk-config.cmake
        DESTINATION lib/cmake/sauron-sdk
    )
endif()