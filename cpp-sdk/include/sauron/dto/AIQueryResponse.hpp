#pragma once

#include "BaseDTO.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief AI Query response DTO
 * 
 * Represents a response from an AI provider
 */
class AIQueryResponse : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    AIQueryResponse() = default;

    /**
     * @brief Constructor with response text
     * 
     * @param responseText The AI-generated response text
     */
    explicit AIQueryResponse(const std::string& responseText) : response(responseText) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return AIQueryResponse The constructed object
     */
    static AIQueryResponse fromJson(const nlohmann::json& json) {
        AIQueryResponse response;
        
        if (json.contains("response") && json["response"].is_string()) {
            response.response = json["response"].get<std::string>();
        }
        
        return response;
    }

    /**
     * @brief Convert to JSON
     * 
     * @return nlohmann::json JSON representation
     */
    nlohmann::json toJson() const override {
        nlohmann::json json;
        json["response"] = response;
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(response, "response");
    }

    /**
     * @brief Get the response text
     * 
     * @return const std::string& The AI-generated response text
     */
    const std::string& getResponse() const {
        return response;
    }

    /**
     * @brief Set the response text
     * 
     * @param responseText The AI-generated response text
     */
    void setResponse(const std::string& responseText) {
        response = responseText;
    }

private:
    std::string response;
};

} // namespace dto
} // namespace sauron 