# Sauron C++ SDK

A C++ SDK for the Sauron AI Authentication & Query API.

## Overview

This SDK provides a header-only C++ interface for securely managing AI provider authentication using JWT and making AI queries with optional images.

## Features

- Secure authentication with AI providers (OpenAI, Anthropic, Google, Mistral, Custom)
- JWT token management
- AI query support with optional image attachments
- Streaming response support
- Strong typing and validation for all DTOs
- Header-only implementation for easy integration

## Requirements

- C++17 or higher
- CMake 3.14 or higher
- nlohmann_json (automatically fetched if not found)

## Installation

### Using CMake

Add to your CMakeLists.txt:

```cmake
# Add the Sauron SDK directory
add_subdirectory(path/to/sauron-sdk)

# Link against the SDK
target_link_libraries(your_target PRIVATE sauron_sdk)
```

Or install system-wide:

```bash
mkdir build && cd build
cmake ..
make install
```

## Usage

### Basic Example

```cpp
#include <sauron/Sauron.hpp>
#include <iostream>

int main() {
    // Create a client with custom HTTP implementation
    auto httpClient = std::make_unique<YourHttpClient>();
    sauron::client::SauronClient client(std::move(httpClient));
    
    try {
        // Login with OpenAI
        sauron::dto::LoginRequest loginRequest("your-api-key", sauron::dto::AIProvider::OPENAI);
        auto tokenResponse = client.login(loginRequest);
        
        // The token is automatically set in the client
        
        // Make an AI query
        sauron::dto::AIQueryRequest queryRequest("Tell me a joke", sauron::dto::AIProvider::OPENAI);
        queryRequest.setModel("gpt-4");
        
        auto response = client.query(queryRequest);
        
        std::cout << "AI Response: " << response.getResponse() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

### Streaming Example

```cpp
#include <sauron/Sauron.hpp>
#include <iostream>

int main() {
    // Create a client with custom HTTP implementation
    auto httpClient = std::make_unique<YourHttpClient>();
    sauron::client::SauronClient client(std::move(httpClient));
    
    try {
        // Login with Anthropic
        sauron::dto::LoginRequest loginRequest("your-api-key", sauron::dto::AIProvider::ANTHROPIC);
        client.login(loginRequest);
        
        // Make a streaming AI query
        sauron::dto::AIQueryRequest queryRequest("Write a short story", sauron::dto::AIProvider::ANTHROPIC);
        queryRequest.setModel("claude-3-opus-20240229");
        
        client.queryStream(queryRequest, [](const std::string& chunk, bool done) {
            std::cout << chunk;
            if (done) {
                std::cout << std::endl;
            }
            return true; // Continue receiving data
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## License

This project is licensed under the MIT License - see the LICENSE file for details. 