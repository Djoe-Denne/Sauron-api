#pragma once

#include "BaseDTO.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief Token response DTO
 * 
 * Represents a JWT token response from the authentication API
 */
class TokenResponse : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    TokenResponse() = default;

    /**
     * @brief Constructor with token
     * 
     * @param jwtToken The JWT token
     */
    explicit TokenResponse(const std::string& jwtToken) : token(jwtToken) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return TokenResponse The constructed object
     */
    static TokenResponse fromJson(const nlohmann::json& json) {
        TokenResponse response;
        if (json.contains("token") && json["token"].is_string()) {
            response.token = json["token"].get<std::string>();
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
        json["token"] = token;
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(token, "token");
    }

    /**
     * @brief Get the JWT token
     * 
     * @return const std::string& The JWT token
     */
    const std::string& getToken() const {
        return token;
    }

    /**
     * @brief Set the JWT token
     * 
     * @param jwtToken The JWT token
     */
    void setToken(const std::string& jwtToken) {
        token = jwtToken;
    }

private:
    std::string token;
};

} // namespace dto
} // namespace sauron 