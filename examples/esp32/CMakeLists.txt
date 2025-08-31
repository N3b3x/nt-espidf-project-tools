# ESP32 Comprehensive Tests Project
cmake_minimum_required(VERSION 3.16)

# Set project name
set(PROJECT_NAME "esp32_comprehensive_tests")

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Define the project
project(${PROJECT_NAME})

# Set component requirements
set(EXTRA_COMPONENT_DIRS "main")

# Set build directory
set(BUILD_DIRECTORY "build")

# Set log level
set(LOG_LEVEL "INFO")

# Set optimization level for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "-O2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

# Set debug flags for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# Print project information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build directory: ${BUILD_DIRECTORY}")
message(STATUS "Log level: ${LOG_LEVEL}")

# Add custom targets for each test
add_custom_target(wifi_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target wifi_test
    COMMENT "Building WiFi comprehensive test"
    DEPENDS wifi_test.elf
)

add_custom_target(bluetooth_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target bluetooth_test
    COMMENT "Building Bluetooth comprehensive test"
    DEPENDS bluetooth_test.elf
)

add_custom_target(i2c_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target i2c_test
    COMMENT "Building I2C comprehensive test"
    DEPENDS i2c_test.elf
)

# Add custom target to build all tests
add_custom_target(all_tests
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target wifi_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target bluetooth_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target i2c_test
    COMMENT "Building all comprehensive tests"
    DEPENDS wifi_test bluetooth_test i2c_test
)

# Print available targets
message(STATUS "Available targets:")
message(STATUS "  wifi_test      - WiFi comprehensive test")
message(STATUS "  bluetooth_test - Bluetooth comprehensive test")
message(STATUS "  i2c_test       - I2C comprehensive test")
message(STATUS "  all_tests      - Build all tests")