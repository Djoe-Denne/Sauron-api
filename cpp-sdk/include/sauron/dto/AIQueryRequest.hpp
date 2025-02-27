#pragma once

#include "BaseDTO.hpp"
#include "AIProvider.hpp"
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace sauron {
namespace dto {

/**
 * @brief AI Query request DTO
 * 
 * Represents a request to query an AI provider
 */
class AIQueryRequest : public BaseDTO {
public:
    /**
     * @brief Default constructor
     */
    AIQueryRequest() = default;

    /**
     * @brief Constructor with required fields
     * 
     * @param prompt The prompt to send to the AI
     * @param provider The AI provider to use
     */
    AIQueryRequest(const std::string& prompt, AIProvider provider)
        : prompt(prompt), provider(provider) {}

    /**
     * @brief Constructor with all fields
     * 
     * @param prompt The prompt to send to the AI
     * @param provider The AI provider to use
     * @param model The AI model to use
     * @param images Optional array of base64-encoded images
     */
    AIQueryRequest(const std::string& prompt, AIProvider provider, 
                  const std::string& model, 
                  const std::vector<std::string>& images = {})
        : prompt(prompt), provider(provider), model(model), images(images) {}

    /**
     * @brief Construct from JSON
     * 
     * @param json JSON object to parse
     * @return AIQueryRequest The constructed object
     */
    static AIQueryRequest fromJson(const nlohmann::json& json) {
        AIQueryRequest request;
        
        if (json.contains("prompt") && json["prompt"].is_string()) {
            request.prompt = json["prompt"].get<std::string>();
        }
        
        if (json.contains("provider") && json["provider"].is_string()) {
            try {
                request.provider = toAIProvider(json["provider"].get<std::string>());
            } catch (const std::invalid_argument&) {
                // Keep default value if invalid
            }
        }
        
        if (json.contains("model") && json["model"].is_string()) {
            request.model = json["model"].get<std::string>();
        }
        
        if (json.contains("images") && json["images"].is_array()) {
            for (const auto& image : json["images"]) {
                if (image.is_string()) {
                    request.images.push_back(image.get<std::string>());
                }
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
        json["prompt"] = prompt;
        json["provider"] = AIProviderToString(provider);
        
        if (!model.empty()) {
            json["model"] = model;
        }
        
        if (!images.empty()) {
            json["images"] = images;
        }
        
        return json;
    }

    /**
     * @brief Validate the DTO
     * 
     * @throws std::invalid_argument if validation fails
     */
    void validate() const override {
        validateRequired(prompt, "prompt");
        // Provider is already validated by the enum
    }

    /**
     * @brief Get the prompt
     * 
     * @return const std::string& The prompt
     */
    const std::string& getPrompt() const {
        return prompt;
    }

    /**
     * @brief Set the prompt
     * 
     * @param text The prompt text
     */
    void setPrompt(const std::string& text) {
        prompt = text;
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

    /**
     * @brief Get the AI model
     * 
     * @return const std::string& The AI model
     */
    const std::string& getModel() const {
        return model;
    }

    /**
     * @brief Set the AI model
     * 
     * @param modelName The AI model name
     */
    void setModel(const std::string& modelName) {
        model = modelName;
    }

    /**
     * @brief Get the images
     * 
     * @return const std::vector<std::string>& The base64-encoded images
     */
    const std::vector<std::string>& getImages() const {
        return images;
    }

    /**
     * @brief Set the images
     * 
     * @param imageData The base64-encoded images
     */
    void setImages(const std::vector<std::string>& imageData) {
        images = imageData;
    }

    /**
     * @brief Add an image
     * 
     * @param imageData The base64-encoded image
     */
    void addImage(const std::string& imageData) {
        images.push_back(imageData);
    }

private:
    std::string prompt;
    AIProvider provider = AIProvider::OPENAI; // Default to OpenAI
    std::string model = "default";           // Default model
    std::vector<std::string> images;         // Optional images
};

} // namespace dto
} // namespace sauron 