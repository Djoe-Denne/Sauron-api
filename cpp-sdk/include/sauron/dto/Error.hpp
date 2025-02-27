#pragma once

#include "BaseDTO.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief Error response DTO
 * 
 * Represents an error response from the API
 */
class Error : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    Error() = default;

    /**
     * @brief Constructor with error message
     * 
     * @param errorMessage The error message
     */
    explicit Error(const std::string& errorMessage) : error(errorMessage) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     */
    static Error fromJson(const nlohmann::json& json) {
        Error error;
        if (json.contains("error") && json["error"].is_string()) {
            error.error = json["error"].get<std::string>();
        }
        return error;
    }

    /**
     * @brief Convert to JSON
     * 
     * @return nlohmann::json JSON representation
     */
    nlohmann::json toJson() const override {
        nlohmann::json json;
        json["error"] = error;
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(error, "error");
    }

    /**
     * @brief Get the error message
     * 
     * @return const std::string& The error message
     */
    const std::string& getError() const {
        return error;
    }

    /**
     * @brief Set the error message
     * 
     * @param errorMessage The error message
     */
    void setError(const std::string& errorMessage) {
        error = errorMessage;
    }

private:
    std::string error;
};

} // namespace dto
} // namespace sauron 