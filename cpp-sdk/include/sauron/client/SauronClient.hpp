#pragma once

#include "HttpClient.hpp"
#include "../dto/DTOs.hpp"
#include <string>
#include <memory>
#include <functional>

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
     * @brief Constructor
     * 
     * @param baseUrl The base URL of the API
     */
    explicit SauronClient(const std::string& baseUrl = "http://localhost:3000");

    /**
     * @brief Constructor with custom HTTP client
     * 
     * @param httpClient Custom HTTP client implementation
     */
    explicit SauronClient(std::unique_ptr<HttpClient> httpClient);

    /**
     * @brief Destructor
     */
    ~SauronClient();

    // Prevent copying
    SauronClient(const SauronClient&) = delete;
    SauronClient& operator=(const SauronClient&) = delete;

    // Allow moving
    SauronClient(SauronClient&&) noexcept;
    SauronClient& operator=(SauronClient&&) noexcept;

    /**
     * @brief Login to an AI provider
     * 
     * @param request The login request
     * @return dto::TokenResponse The token response
     * @throws std::runtime_error if the request fails
     */
    dto::TokenResponse login(const dto::LoginRequest& request);

    /**
     * @brief Refresh the JWT token
     * 
     * @return dto::TokenResponse The new token response
     * @throws std::runtime_error if the request fails
     */
    dto::TokenResponse refreshToken();

    /**
     * @brief Send a query to an AI provider
     * 
     * @param request The AI query request
     * @return dto::AIQueryResponse The AI response
     * @throws std::runtime_error if the request fails
     */
    dto::AIQueryResponse query(const dto::AIQueryRequest& request);

    /**
     * @brief Stream a query to an AI provider
     * 
     * @param request The AI query request
     * @param callback The callback function to handle streaming data
     * @return bool True if the request was successful
     * @throws std::runtime_error if the request fails
     */
    bool queryStream(const dto::AIQueryRequest& request, 
                    const std::function<bool(const std::string&, bool)>& callback);

    /**
     * @brief Check the health of the API
     * 
     * @return dto::HealthResponse The health response
     * @throws std::runtime_error if the request fails
     */
    dto::HealthResponse checkHealth();

    /**
     * @brief Set the JWT token for authentication
     * 
     * @param token The JWT token
     */
    void setToken(const std::string& token);

    /**
     * @brief Get the current JWT token
     * 
     * @return std::string The JWT token
     */
    std::string getToken() const;

    /**
     * @brief Clear the JWT token
     */
    void clearToken();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace client
} // namespace sauron 