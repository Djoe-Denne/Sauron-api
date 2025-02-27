#pragma once

/**
 * @file Sauron.hpp
 * @brief Main header file for the Sauron SDK
 */

#include "dto/DTOs.hpp"
#include "client/HttpClient.hpp"
#include "client/SauronClient.hpp"

/**
 * @namespace sauron
 * @brief Root namespace for the Sauron SDK
 */
namespace sauron {

/**
 * @brief Version information for the Sauron SDK
 */
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 0;
    static constexpr int PATCH = 0;
    
    /**
     * @brief Get the version string
     * 
     * @return const char* The version string in format "MAJOR.MINOR.PATCH"
     */
    static const char* toString() {
        return "1.0.0";
    }
};

} // namespace sauron 