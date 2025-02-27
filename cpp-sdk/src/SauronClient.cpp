#include <sauron/client/SauronClient.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace sauron {
namespace client {

class SauronClient::Impl {
public:
    explicit Impl(const std::string& baseUrl)
        : httpClient(HttpClient::create(baseUrl)) {}

    explicit Impl(std::unique_ptr<HttpClient> client)
        : httpClient(std::move(client)) {}

    std::unique_ptr<HttpClient> httpClient;
    std::string token;
};

SauronClient::SauronClient(const std::string& baseUrl)
    : pImpl(std::make_unique<Impl>(baseUrl)) {}

SauronClient::SauronClient(std::unique_ptr<HttpClient> httpClient)
    : pImpl(std::make_unique<Impl>(std::move(httpClient))) {}

SauronClient::~SauronClient() = default;

SauronClient::SauronClient(SauronClient&&) noexcept = default;
SauronClient& SauronClient::operator=(SauronClient&&) noexcept = default;

dto::TokenResponse SauronClient::login(const dto::LoginRequest& request) {
    json requestBody = {
        {"provider", request.provider},
        {"credentials", request.credentials}
    };

    auto response = pImpl->httpClient->post("/auth/login", requestBody);
    if (response.statusCode != 200) {
        throw std::runtime_error("Login failed: " + response.body);
    }

    auto responseJson = json::parse(response.body);
    dto::TokenResponse tokenResponse;
    tokenResponse.token = responseJson["token"].get<std::string>();
    tokenResponse.expiresIn = responseJson["expiresIn"].get<int>();

    setToken(tokenResponse.token);
    return tokenResponse;
}

dto::TokenResponse SauronClient::refreshToken() {
    if (pImpl->token.empty()) {
        throw std::runtime_error("No token available for refresh");
    }

    pImpl->httpClient->setBearerToken(pImpl->token);
    auto response = pImpl->httpClient->post("/auth/refresh", json::object());
    
    if (response.statusCode != 200) {
        throw std::runtime_error("Token refresh failed: " + response.body);
    }

    auto responseJson = json::parse(response.body);
    dto::TokenResponse tokenResponse;
    tokenResponse.token = responseJson["token"].get<std::string>();
    tokenResponse.expiresIn = responseJson["expiresIn"].get<int>();

    setToken(tokenResponse.token);
    return tokenResponse;
}

dto::AIQueryResponse SauronClient::query(const dto::AIQueryRequest& request) {
    if (pImpl->token.empty()) {
        throw std::runtime_error("No authentication token available");
    }

    json requestBody = {
        {"provider", request.provider},
        {"query", request.query},
        {"options", request.options}
    };

    pImpl->httpClient->setBearerToken(pImpl->token);
    auto response = pImpl->httpClient->post("/ai/query", requestBody);

    if (response.statusCode != 200) {
        throw std::runtime_error("Query failed: " + response.body);
    }

    auto responseJson = json::parse(response.body);
    dto::AIQueryResponse queryResponse;
    queryResponse.response = responseJson["response"].get<std::string>();
    queryResponse.metadata = responseJson["metadata"];
    
    return queryResponse;
}

bool SauronClient::queryStream(const dto::AIQueryRequest& request,
                             const std::function<bool(const std::string&, bool)>& callback) {
    if (pImpl->token.empty()) {
        throw std::runtime_error("No authentication token available");
    }

    json requestBody = {
        {"provider", request.provider},
        {"query", request.query},
        {"options", request.options}
    };

    pImpl->httpClient->setBearerToken(pImpl->token);
    int statusCode = pImpl->httpClient->postStream("/ai/query/stream", requestBody, callback);

    return statusCode == 200;
}

dto::HealthResponse SauronClient::checkHealth() {
    auto response = pImpl->httpClient->get("/health");
    
    if (response.statusCode != 200) {
        throw std::runtime_error("Health check failed: " + response.body);
    }

    auto responseJson = json::parse(response.body);
    dto::HealthResponse healthResponse;
    healthResponse.status = responseJson["status"].get<std::string>();
    healthResponse.version = responseJson["version"].get<std::string>();
    
    return healthResponse;
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