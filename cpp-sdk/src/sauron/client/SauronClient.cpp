#include "sauron/client/SauronClient.hpp"
#include "sauron/dto/DTOs.hpp"
#include <stdexcept>

namespace sauron {
namespace client {

class SauronClient::Impl {
public:
    explicit Impl(std::unique_ptr<HttpClient> client)
        : httpClient(std::move(client)) {}

    std::unique_ptr<HttpClient> httpClient;
    std::string token;
};

SauronClient::SauronClient(std::unique_ptr<HttpClient> httpClient)
    : pImpl(std::make_unique<Impl>(std::move(httpClient))) {}

SauronClient::~SauronClient() = default;

SauronClient::SauronClient(SauronClient&&) noexcept = default;
SauronClient& SauronClient::operator=(SauronClient&&) noexcept = default;

dto::TokenResponse SauronClient::login(const dto::LoginRequest& request) {
    request.validate();
    auto response = pImpl->httpClient->post("/auth/login", request.toJson());
    
    if (response.statusCode != 200) {
        auto jsonResponse = nlohmann::json::parse(response.body);
        auto error = dto::Error::fromJson(jsonResponse);
        throw std::runtime_error(error.getError());
    }

    auto jsonResponse = nlohmann::json::parse(response.body);
    auto tokenResponse = dto::TokenResponse::fromJson(jsonResponse);
    setToken(tokenResponse.getToken());
    return tokenResponse;
}

dto::TokenResponse SauronClient::refreshToken() {
    if (pImpl->token.empty()) {
        throw std::runtime_error("No token available for refresh");
    }

    pImpl->httpClient->setBearerToken(pImpl->token);
    auto response = pImpl->httpClient->post("/auth/refresh", nlohmann::json({}));
    
    if (response.statusCode != 200) {
        auto jsonResponse = nlohmann::json::parse(response.body);
        auto error = dto::Error::fromJson(jsonResponse);
        throw std::runtime_error(error.getError());
    }

    auto jsonResponse = nlohmann::json::parse(response.body);
    auto tokenResponse = dto::TokenResponse::fromJson(jsonResponse);
    setToken(tokenResponse.getToken());
    return tokenResponse;
}

dto::AIQueryResponse SauronClient::query(const dto::AIQueryRequest& request) {
    request.validate();
    
    if (pImpl->token.empty()) {
        throw std::runtime_error("No token available. Please login first.");
    }

    pImpl->httpClient->setBearerToken(pImpl->token);
    auto response = pImpl->httpClient->post("/ai/query", request.toJson());
    
    if (response.statusCode != 200) {
        auto jsonResponse = nlohmann::json::parse(response.body);
        auto error = dto::Error::fromJson(jsonResponse);
        throw std::runtime_error(error.getError());
    }

    auto jsonResponse = nlohmann::json::parse(response.body);
    return dto::AIQueryResponse::fromJson(jsonResponse);
}

bool SauronClient::queryStream(const dto::AIQueryRequest& request,
                             const std::function<bool(const std::string&, bool)>& callback) {
    request.validate();
    
    if (pImpl->token.empty()) {
        throw std::runtime_error("No token available. Please login first.");
    }

    pImpl->httpClient->setBearerToken(pImpl->token);
    int statusCode = pImpl->httpClient->postStream("/ai/query/stream", request.toJson(), callback);
    
    if (statusCode != 200) {
        throw std::runtime_error("Stream request failed with status code: " + std::to_string(statusCode));
    }

    return true;
}

dto::HealthResponse SauronClient::checkHealth() {
    auto response = pImpl->httpClient->get("/health");
    
    if (response.statusCode != 200) {
        auto jsonResponse = nlohmann::json::parse(response.body);
        auto error = dto::Error::fromJson(jsonResponse);
        throw std::runtime_error(error.getError());
    }

    auto jsonResponse = nlohmann::json::parse(response.body);
    return dto::HealthResponse::fromJson(jsonResponse);
}

void SauronClient::setToken(const std::string& token) {
    pImpl->token = token;
    pImpl->httpClient->setBearerToken(token);
}

std::string SauronClient::getToken() const {
    return pImpl->token;
}

void SauronClient::clearToken() {
    pImpl->token.clear();
    pImpl->httpClient->clearAuthorization();
}

} // namespace client
} // namespace sauron 