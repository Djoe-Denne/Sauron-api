#pragma once

#include "BaseDTO.hpp"
#include "AIProvider.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief Login request DTO
 * 
 * Represents a request to authenticate with an AI provider
 */
class LoginRequest : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    LoginRequest() = default;

    /**
     * @brief Constructor with API key and provider
     * 
     * @param apiKey The API key for the AI provider
     * @param provider The AI provider
     */
    LoginRequest(const std::string& apiKey, AIProvider provider)
        : apiKey(apiKey), provider(provider) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return LoginRequest The constructed object
     */
    static LoginRequest fromJson(const nlohmann::json& json) {
        LoginRequest request;
        
        if (json.contains("api_key") && json["api_key"].is_string()) {
            request.apiKey = json["api_key"].get<std::string>();
        }
        
        if (json.contains("provider") && json["provider"].is_string()) {
            try {
                request.provider = toAIProvider(json["provider"].get<std::string>());
            } catch (const std::invalid_argument&) {
                // Keep default value if invalid
            }
        }
        
        return request;
    }

    /**
     * @brief Convert to JSON
     * 
     * @return nlohmann::json JSON representation
     */
    nlohmann::json toJson() const override {
        nlohmann::json json;
        json["api_key"] = apiKey;
        json["provider"] = toString(provider);
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(apiKey, "api_key");
        // Provider is already validated by the enum
    }

    /**
     * @brief Get the API key
     * 
     * @return const std::string& The API key
     */
    const std::string& getApiKey() const {
        return apiKey;
    }

    /**
     * @brief Set the API key
     * 
     * @param key The API key
     */
    void setApiKey(const std::string& key) {
        apiKey = key;
    }

    /**
     * @brief Get the AI provider
     * 
     * @return AIProvider The AI provider
     */
    AIProvider getProvider() const {
        return provider;
    }

    /**
     * @brief Set the AI provider
     * 
     * @param aiProvider The AI provider
     */
    void setProvider(AIProvider aiProvider) {
        provider = aiProvider;
    }

private:
    std::string apiKey;
    AIProvider provider = AIProvider::OPENAI; // Default to OpenAI
};

} // namespace dto
} // namespace sauron 