#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

namespace sauron {
namespace client {

/**
 * @brief HTTP response structure
 */
struct HttpResponse {
    int statusCode;                  ///< HTTP status code
    std::string body;                ///< Response body
    std::vector<std::string> headers; ///< Response headers
};

/**
 * @brief Callback for streaming responses
 * 
 * @param chunk The data chunk received
 * @param done Whether this is the last chunk
 * @return bool Whether to continue receiving data
 */
using StreamCallback = std::function<bool(const std::string& chunk, bool done)>;

/**
 * @brief HTTP client interface
 * 
 * Abstract class for making HTTP requests
 */
class HttpClient {
public:
    virtual ~HttpClient() = default;

    /**
     * @brief Set base URL for all requests
     * 
     * @param url The base URL
     */
    virtual void setBaseUrl(const std::string& url) = 0;

    /**
     * @brief Get the base URL
     * 
     * @return std::string The base URL
     */
    virtual std::string getBaseUrl() const = 0;

    /**
     * @brief Set a default header for all requests
     * 
     * @param name Header name
     * @param value Header value
     */
    virtual void setDefaultHeader(const std::string& name, const std::string& value) = 0;

    /**
     * @brief Remove a default header
     * 
     * @param name Header name
     */
    virtual void removeDefaultHeader(const std::string& name) = 0;

    /**
     * @brief Set the authorization header with a bearer token
     * 
     * @param token The bearer token
     */
    virtual void setBearerToken(const std::string& token) = 0;

    /**
     * @brief Clear the authorization header
     */
    virtual void clearAuthorization() = 0;

    /**
     * @brief Make a GET request
     * 
     * @param path The request path (will be appended to the base URL)
     * @param headers Additional headers for this request
     * @return HttpResponse The HTTP response
     */
    virtual HttpResponse get(const std::string& path, 
                            const std::vector<std::string>& headers = {}) = 0;

    /**
     * @brief Make a POST request with JSON body
     * 
     * @param path The request path (will be appended to the base URL)
     * @param body The JSON body
     * @param headers Additional headers for this request
     * @return HttpResponse The HTTP response
     */
    virtual HttpResponse post(const std::string& path, 
                             const nlohmann::json& body,
                             const std::vector<std::string>& headers = {}) = 0;

    /**
     * @brief Make a POST request with string body
     * 
     * @param path The request path (will be appended to the base URL)
     * @param body The request body
     * @param contentType The content type of the body
     * @param headers Additional headers for this request
     * @return HttpResponse The HTTP response
     */
    virtual HttpResponse post(const std::string& path, 
                             const std::string& body,
                             const std::string& contentType,
                             const std::vector<std::string>& headers = {}) = 0;

    /**
     * @brief Make a streaming POST request with JSON body
     * 
     * @param path The request path (will be appended to the base URL)
     * @param body The JSON body
     * @param callback The callback function to handle streaming data
     * @param headers Additional headers for this request
     * @return int The HTTP status code
     */
    virtual int postStream(const std::string& path, 
                          const nlohmann::json& body,
                          const StreamCallback& callback,
                          const std::vector<std::string>& headers = {}) = 0;

    /**
     * @brief Create a new HttpClient instance
     * 
     * @param baseUrl Optional base URL
     * @return std::unique_ptr<HttpClient> A new HttpClient instance
     */
    static std::unique_ptr<HttpClient> create(const std::string& baseUrl = "");
};

} // namespace client
} // namespace sauron 