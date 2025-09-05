# ESP32 HardFOC Interface Wrapper - CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Set the project name
set(PROJECT_NAME "esp32_iid_gpio_test_app")

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the project name
project(${PROJECT_NAME})