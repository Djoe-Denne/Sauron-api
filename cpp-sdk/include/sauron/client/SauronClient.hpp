#pragma once

#include "HttpClient.hpp"
#include "../dto/DTOs.hpp"
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace sauron {
namespace client {

/**
 * @brief Client for the Sauron AI Authentication & Query API
 *
 * Provides methods for authenticating with AI providers and making AI queries
 */
class SauronClient {
public:
    /**
     * @brief Constructor with custom HTTP client
     *
     * @param httpClient Custom HTTP client implementation
     */
    explicit SauronClient(std::unique_ptr<HttpClient> httpClient)
        : httpClient_(std::move(httpClient)) {}

    /**
     * @brief Destructor
     */
    ~SauronClient() = default;
    SauronClient(SauronClient&&) noexcept = default;
    SauronClient& operator=(SauronClient&&) noexcept = default;

    /**
     * @brief Login to an AI provider
     *
     * @param request The login request
     * @return dto::TokenResponse The token response
     * @throws std::runtime_error if the request fails
     */
    dto::TokenResponse login(const dto::LoginRequest& request) {
        request.validate();
        auto response = httpClient_->post("/auth/login", request.toJson());
        if (response.statusCode != 200) {
            throw std::runtime_error(dto::Error::fromJson(nlohmann::json::parse(response.body)).getError());
        }
        auto tokenResponse = dto::TokenResponse::fromJson(nlohmann::json::parse(response.body));
        setToken(tokenResponse.getToken());
        return tokenResponse;
    }

    /**
     * @brief Refresh the JWT token
     *
     * @return dto::TokenResponse The new token response
     * @throws std::runtime_error if the request fails
     */
    dto::TokenResponse refreshToken() {
        if (token_.empty()) {
            throw std::runtime_error("No token available for refresh");
        }
        httpClient_->setBearerToken(token_);
        auto response = httpClient_->post("/auth/refresh", nlohmann::json({}));
        if (response.statusCode != 200) {
            throw std::runtime_error(dto::Error::fromJson(nlohmann::json::parse(response.body)).getError());
        }
        auto tokenResponse = dto::TokenResponse::fromJson(nlohmann::json::parse(response.body));
        setToken(tokenResponse.getToken());
        return tokenResponse;
    }

    /**
     * @brief Send a query to an AI provider
     *
     * @param request The AI query request
     * @return dto::AIQueryResponse The AI response
     * @throws std::runtime_error if the request fails
     */
    dto::AIQueryResponse query(const dto::AIQueryRequest& request) {
        request.validate();
        if (token_.empty()) {
            throw std::runtime_error("No token available. Please login first.");
        }
        httpClient_->setBearerToken(token_);
        auto response = httpClient_->post("/ai/query", request.toJson());
        if (response.statusCode != 200) {
            throw std::runtime_error(dto::Error::fromJson(nlohmann::json::parse(response.body)).getError());
        }
        return dto::AIQueryResponse::fromJson(nlohmann::json::parse(response.body));
    }

    /**
     * @brief Stream a query to an AI provider
     *
     * @param request The AI query request
     * @param callback The callback function to handle streaming data
     * @return bool True if the request was successful
     * @throws std::runtime_error if the request fails
     */
    bool queryStream(const dto::AIQueryRequest& request, const std::function<bool(const std::string&, bool)>& callback) {
        request.validate();
        if (token_.empty()) {
            throw std::runtime_error("No token available. Please login first.");
        }
        httpClient_->setBearerToken(token_);
        int statusCode = httpClient_->postStream("/ai/query/stream", request.toJson(), callback);
        if (statusCode != 200) {
            throw std::runtime_error("Stream request failed with status code: " + std::to_string(statusCode));
        }
        return true;
    }

    /**
     * @brief Check the health of the API
     *
     * @return dto::HealthResponse The health response
     * @throws std::runtime_error if the request fails
     */
    dto::HealthResponse checkHealth() {
        auto response = httpClient_->get("/health");
        if (response.statusCode != 200) {
            throw std::runtime_error(dto::Error::fromJson(nlohmann::json::parse(response.body)).getError());
        }
        return dto::HealthResponse::fromJson(nlohmann::json::parse(response.body));
    }

    void setToken(const std::string& token) {
        token_ = token;
        httpClient_->setBearerToken(token);
    }

    std::string getToken() const { return token_; }

    void clearToken() {
        token_.clear();
        httpClient_->clearAuthorization();
    }

private:
    std::unique_ptr<HttpClient> httpClient_;
    std::string token_;
};

} // namespace client
} // namespace sauron