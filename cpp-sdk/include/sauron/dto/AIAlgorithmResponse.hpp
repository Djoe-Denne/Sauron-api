#pragma once

#include "BaseDTO.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

class ComplexityInfo {
public:
    ComplexityInfo() = default;

    ComplexityInfo(const std::string& val, const std::string& exp)
        : value(val), explanation(exp) {}

    static ComplexityInfo fromJson(const nlohmann::json& json) {
        ComplexityInfo info;
        if (json.contains("value") && json["value"].is_string()) {
            info.value = json["value"].get<std::string>();
        }
        if (json.contains("explanation") && json["explanation"].is_string()) {
            info.explanation = json["explanation"].get<std::string>();
        }
        return info;
    }

    nlohmann::json toJson() const {
        nlohmann::json json;
        json["value"] = value;
        json["explanation"] = explanation;
        return json;
    }

    std::string value;
    std::string explanation;
};

class AlgorithmComplexity {
public:
    AlgorithmComplexity() = default;

    AlgorithmComplexity(const ComplexityInfo& t, const ComplexityInfo& s)
        : time(t), space(s) {}

    static AlgorithmComplexity fromJson(const nlohmann::json& json) {
        AlgorithmComplexity complexity;
        if (json.contains("time") && json["time"].is_object()) {
            complexity.time = ComplexityInfo::fromJson(json["time"]);
        }
        if (json.contains("space") && json["space"].is_object()) {
            complexity.space = ComplexityInfo::fromJson(json["space"]);
        }
        return complexity;
    }

    nlohmann::json toJson() const {
        nlohmann::json json;
        json["time"] = time.toJson();
        json["space"] = space.toJson();
        return json;
    }

    ComplexityInfo time;
    ComplexityInfo space;
};

/**
 * @brief AI Algorithm Query response DTO
 * 
 * Represents a response from an AI provider for algorithm queries
 */
class AIAlgorithmResponse : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    AIAlgorithmResponse() = default;

    /**
     * @brief Constructor with all fields
     * 
     * @param exp The explanation text
     * @param resp The response code
     * @param comp The complexity analysis
     */
    AIAlgorithmResponse(const std::string& exp, const std::string& resp, const AlgorithmComplexity& comp)
        : explanation(exp), response(resp), complexity(comp) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return AIAlgorithmResponse The constructed object
     */
    static AIAlgorithmResponse fromJson(const nlohmann::json& json) {
        AIAlgorithmResponse response;
        
        if (json.contains("explanation") && json["explanation"].is_string()) {
            response.explanation = json["explanation"].get<std::string>();
        }
        
        if (json.contains("response") && json["response"].is_string()) {
            response.response = json["response"].get<std::string>();
        }
        
        if (json.contains("complexity") && json["complexity"].is_object()) {
            response.complexity = AlgorithmComplexity::fromJson(json["complexity"]);
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
        json["explanation"] = explanation;
        json["response"] = response;
        json["complexity"] = complexity.toJson();
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(explanation, "explanation");
        validateRequired(response, "response");
    }

    /**
     * @brief Get the explanation text
     */
    const std::string& getExplanation() const { return explanation; }

    /**
     * @brief Set the explanation text
     */
    void setExplanation(const std::string& exp) { explanation = exp; }

    /**
     * @brief Get the response code
     */
    const std::string& getResponse() const { return response; }

    /**
     * @brief Set the response code
     */
    void setResponse(const std::string& resp) { response = resp; }

    /**
     * @brief Get the complexity analysis
     */
    const AlgorithmComplexity& getComplexity() const { return complexity; }

    /**
     * @brief Set the complexity analysis
     */
    void setComplexity(const AlgorithmComplexity& comp) { complexity = comp; }

private:
    std::string explanation;
    std::string response;
    AlgorithmComplexity complexity;
};

} // namespace dto
} // namespace sauron 