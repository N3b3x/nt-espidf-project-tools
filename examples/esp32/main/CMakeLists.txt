# Main component CMakeLists.txt for ESP32 Comprehensive Tests

# Set component name
set(COMPONENT_NAME "main")

# Set component requirements
set(COMPONENT_REQUIRES "driver" "esp_wifi" "esp_netif" "esp_event" "nvs_flash" "esp_bt" "esp_gap_ble_api" "esp_gatts_api" "esp_gatt_defs" "esp_bt_main" "esp_bt_device" "freertos" "esp_system" "esp_log" "esp_common")

# Set component sources
set(COMPONENT_SRCDIRS ".")

# Set component include directories
set(COMPONENT_ADD_INCLUDEDIRS ".")

# Set component dependencies
set(COMPONENT_DEPENDS "")

# Register the component
idf_component_register(
    SRCDIRS ${COMPONENT_SRCDIRS}
    INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
    REQUIRES ${COMPONENT_REQUIRES}
)

# Set C++ standard
set_target_properties(${COMPONENT_LIB} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add compile definitions
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    CONFIG_IDF_TARGET_ESP32C6
    CONFIG_ESP32C6_DEFAULT_CPU_FREQ_160
    CONFIG_FREERTOS_HZ=1000
)

# Print component information
message(STATUS "Component: ${COMPONENT_NAME}")
message(STATUS "Sources: ${COMPONENT_SRCDIRS}")
message(STATUS "Includes: ${COMPONENT_ADD_INCLUDEDIRS}")
message(STATUS "Requirements: ${COMPONENT_REQUIRES}")