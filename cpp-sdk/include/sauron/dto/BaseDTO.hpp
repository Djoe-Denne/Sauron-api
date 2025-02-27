#pragma once

#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief Base class for all Data Transfer Objects
 * 
 * Provides common functionality for serialization and validation
 */
class BaseDTO {
public:
    virtual ~BaseDTO() = default;

    /**
     * @brief Convert the DTO to a JSON object
     * 
     * @return nlohmann::json JSON representation of the DTO
     */
    virtual nlohmann::json toJson() const = 0;

    /**
     * @brief Convert the DTO to a JSON string
     * 
     * @return std::string JSON string representation of the DTO
     */
    std::string toString() const {
        return toJson().dump();
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    virtual void validate() const = 0;

protected:
    /**
     * @brief Validate that a required field is not empty
     * 
     * @param value The value to check
     * @param fieldName The name of the field for error reporting
     * @throws std::invalid_argument if the field is empty
     */
    void validateRequired(const std::string& value, const std::string& fieldName) const {
        if (value.empty()) {
            throw std::invalid_argument(fieldName + " is required");
        }
    }

    /**
     * @brief Validate that a value is one of the allowed values
     * 
     * @tparam T The type of the value
     * @param value The value to check
     * @param allowedValues The list of allowed values
     * @param fieldName The name of the field for error reporting
     * @throws std::invalid_argument if the value is not in the allowed values
     */
    template<typename T>
    void validateEnum(const T& value, const std::vector<T>& allowedValues, const std::string& fieldName) const {
        if (std::find(allowedValues.begin(), allowedValues.end(), value) == allowedValues.end()) {
            throw std::invalid_argument(fieldName + " has an invalid value");
        }
    }
};

} // namespace dto
} // namespace sauron 