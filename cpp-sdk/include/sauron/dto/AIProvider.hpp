#pragma once

#include <string>
#include <stdexcept>
#include <vector>

namespace sauron {
namespace dto {

/**
 * @brief Enum representing AI providers
 */
enum class AIProvider {
    OPENAI,
    ANTHROPIC,
    GOOGLE,
    MISTRAL,
    CUSTOM
};

/**
 * @brief Convert AIProvider enum to string
 * 
 * @param provider The AIProvider enum value
 * @return std::string The string representation
 */
inline std::string toString(AIProvider provider) {
    switch (provider) {
        case AIProvider::OPENAI:
            return "openai";
        case AIProvider::ANTHROPIC:
            return "anthropic";
        case AIProvider::GOOGLE:
            return "google";
        case AIProvider::MISTRAL:
            return "mistral";
        case AIProvider::CUSTOM:
            return "custom";
        default:
            throw std::invalid_argument("Invalid AIProvider value");
    }
}

/**
 * @brief Convert string to AIProvider enum
 * 
 * @param providerStr The string representation
 * @return AIProvider The AIProvider enum value
 * @throws std::invalid_argument if the string is not a valid provider
 */
inline AIProvider toAIProvider(const std::string& providerStr) {
    if (providerStr == "openai") {
        return AIProvider::OPENAI;
    } else if (providerStr == "anthropic") {
        return AIProvider::ANTHROPIC;
    } else if (providerStr == "google") {
        return AIProvider::GOOGLE;
    } else if (providerStr == "mistral") {
        return AIProvider::MISTRAL;
    } else if (providerStr == "custom") {
        return AIProvider::CUSTOM;
    } else {
        throw std::invalid_argument("Invalid provider string: " + providerStr);
    }
}

/**
 * @brief Get all valid AIProvider values as strings
 * 
 * @return std::vector<std::string> Vector of valid provider strings
 */
inline std::vector<std::string> getValidProviderStrings() {
    return {"openai", "anthropic", "google", "mistral", "custom"};
}

/**
 * @brief Get all valid AIProvider enum values
 * 
 * @return std::vector<AIProvider> Vector of valid provider enum values
 */
inline std::vector<AIProvider> getValidProviders() {
    return {
        AIProvider::OPENAI,
        AIProvider::ANTHROPIC,
        AIProvider::GOOGLE,
        AIProvider::MISTRAL,
        AIProvider::CUSTOM
    };
}

} // namespace dto
} // namespace sauron 