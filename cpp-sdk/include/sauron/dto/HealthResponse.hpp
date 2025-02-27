#pragma once

#include "BaseDTO.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief Health response DTO
 * 
 * Represents a health check response from the API
 */
class HealthResponse : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    HealthResponse() : status("ok") {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return HealthResponse The constructed object
     */
    static HealthResponse fromJson(const nlohmann::json& json) {
        HealthResponse response;
        
        if (json.contains("status") && json["status"].is_string()) {
            response.status = json["status"].get<std::string>();
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
        json["status"] = status;
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(status, "status");
        validateEnum(status, std::vector<std::string>{"ok"}, "status");
    }

    /**
     * @brief Get the status
     * 
     * @return const std::string& The status
     */
    const std::string& getStatus() const {
        return status;
    }

    /**
     * @brief Check if the status is OK
     * 
     * @return bool True if the status is "ok"
     */
    bool isOk() const {
        return status == "ok";
    }

private:
    std::string status;
};

} // namespace dto
} // namespace sauron 