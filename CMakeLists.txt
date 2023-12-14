cmake_minimum_required(VERSION 3.22)  # CMake version check
project(OHLC)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 20)            # Enable c++14 standard
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")


# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES src/main.cpp)

# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)




find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(absl REQUIRED)
find_library(ABSL_LOG_INTERNAL_CHECK_OP_LIB NAMES absl_log_internal_check_op PATHS /usr/lib)

file(GLOB KAFKA_LIB
  "src/kafka/config_json/config_json.cpp"
)
add_library(kafkalib ${KAFKA_LIB})



target_link_libraries(kafkalib PRIVATE
    nlohmann_json::nlohmann_json
)

add_library(ProtoTest vector_message.proto)
target_link_libraries(ProtoTest PUBLIC gRPC::grpc++)
protobuf_generate(TARGET ProtoTest)
protobuf_generate(
    TARGET ProtoTest
    LANGUAGE grpc
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    PLUGIN_OPTIONS generate_mock_code=true
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc)


file(GLOB_RECURSE LIB_SOURCES
    "src/lib/*.cpp"
)

add_library(ohlc_lib ${LIB_SOURCES})

# Copy config.json to the build folder
configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)

# Copy RawData to the build folder
file(COPY ${CMAKE_SOURCE_DIR}/rawdata DESTINATION ${CMAKE_BINARY_DIR})



# Fetching all the deps
include(FetchContent)

# Arduino Json
FetchContent_Declare(ArduinoJson
  GIT_REPOSITORY https://github.com/bblanchon/ArduinoJson.git
  GIT_TAG v6.21.3
)


# Fetch dependancy: Spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.12.0
)

set(GTEST_GIT_TAG v1.13.0)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GTEST_GIT_TAG}
)


# Fetch dependancy for CppKafka: librdkafka
set(RDKAFKA_BUILD_EXAMPLES OFF ON CACHE INTERNAL "Turn off examples")
set(RDKAFKA_BUILD_TESTS OFF CACHE INTERNAL "Turn off tests")
FetchContent_Declare(
    librdkafka
    GIT_REPOSITORY https://github.com/confluentinc/librdkafka
    GIT_TAG        v2.3.0
)

# Fetch dependancy: CppKafka
set(CPPKAFKA_DISABLE_EXAMPLES ON CACHE INTERNAL "Turn off examples")
set(CPPKAFKA_DISABLE_TESTS ON CACHE INTERNAL "Turn off tests")
FetchContent_Declare(
    CppKafka
    GIT_REPOSITORY https://github.com/mfontanini/cppkafka.git
    GIT_TAG        v0.3.1
)

# Fetch dependancy: JSON Library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)

FetchContent_MakeAvailable(spdlog librdkafka CppKafka json googletest ArduinoJson)

# Setup the Kafka consumer/producer executable
set(KAFKA_EXEC_LIBS
    rdkafka cppkafka
    nlohmann_json::nlohmann_json spdlog::spdlog ArduinoJson
)

target_link_libraries(ohlc_lib 
    ${KAFKA_EXEC_LIBS}
)

target_include_directories(ohlc_lib PRIVATE
    ${nlohmann_json_SOURCE_DIR}/single_include   
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/lib
    ${nlohmann_json_SOURCE_DIR}/single_include   
    src/lib/data
    src/kafka/config_json
    src/lib/data/protobuf
    src/kafka/proto_lib
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}
    ${HIREDIS_HEADER}
    
)

# Set compiler flags
set(COMPILER_FLAGS
    # All the warnings
    -Wall -Wextra
    # Except for the ones that aren't mine
    -Wno-deprecated -Wno-unused-parameter
    # Add asan options
    -fsanitize=address -fsanitize=pointer-compare
    -fsanitize=pointer-subtract -fsanitize=undefined
    -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
    # Stack pointer
    -fno-omit-frame-pointer
)



# Add executable target with source files listed in SOURCE_FILES variable
# add_executable(OHLC ${SOURCE_FILES})

# target_link_libraries(OHLC
#     ohlc_lib
# )

# Add executable producer
add_executable(producer-kafka src/kafka/producer.cpp)

target_include_directories(producer-kafka PRIVATE 
  ${nlohmann_json_SOURCE_DIR}/single_include   
  src/kafka/config_json
)

file(GLOB_RECURSE KF_LIB_SOURCES
    src/kafka/*.cpp
)

add_library(kf_lib ${KF_LIB_SOURCES})

target_link_libraries(kf_lib PRIVATE
    ohlc_lib
    ProtoTest

)

target_link_libraries(producer-kafka
    absl::base
    absl::strings
    absl::str_format
    cppkafka
    rdkafka
    nlohmann_json::nlohmann_json
    kafkalib
    ProtoTest
    kf_lib
    ${PROTOBUF_LIBRARY}
    ohlc_lib
    
)
add_executable(consumer-kafka src/kafka/consumer.cpp)

target_include_directories(consumer-kafka PRIVATE 
  ${nlohmann_json_SOURCE_DIR}/single_include
  src/kafka/config_json
  ${REDIS_PLUS_PLUS_HEADER}
)
find_library(HIREDIS_LIB hiredis)

target_link_libraries(consumer-kafka
    ProtoTest
    cppkafka
    rdkafka
    nlohmann_json::nlohmann_json
    kafkalib
    kf_lib
    ${PROTOBUF_LIBRARY}
    ohlc_lib
    ${REDIS_PLUS_PLUS_LIB}
    ${HIREDIS_LIB}
)



# Get all C++ test files
file(GLOB_RECURSE TESTS_SOURCES
    tests/*.cpp
)

# Tests
add_executable(tests ${TESTS_SOURCES})
target_link_libraries(tests
    PRIVATE rdkafka
    PRIVATE cppkafka
    GTest::gtest_main
)