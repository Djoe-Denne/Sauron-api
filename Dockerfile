FROM openapitools/openapi-generator-cli:latest

WORKDIR /local

COPY openapi.yaml .

CMD ["generate", \
    "-i", "openapi.yaml", \
    "-g", "cpp-tiny", \
    "-o", "/local/cpp-sdk"] 