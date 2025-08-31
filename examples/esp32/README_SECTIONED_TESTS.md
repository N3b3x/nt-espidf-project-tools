# Sectioned Comprehensive Tests for ESP32

This directory contains comprehensive test suites for WiFi, Bluetooth, and I2C functionality on ESP32 devices. Each test suite is organized into logical sections that can be run individually or in combination, allowing for targeted testing and debugging.

## Test Structure

All three test suites follow the same architecture:

- **Sectioned Organization**: Tests are grouped into logical sections
- **Individual Execution**: Each section can be run independently
- **Comprehensive Results**: Detailed reporting for each section and overall results
- **Command Line Control**: Easy section selection via command line arguments

## Available Test Suites

### 1. WiFi Comprehensive Test (`WifiComprehensiveTest.cpp`)

**Test Sections:**
- `WIFI_INITIALIZATION` - WiFi driver initialization and deinitialization
- `WIFI_BASIC_OPERATIONS` - Basic WiFi operations like scanning
- `WIFI_CONNECTIVITY` - Connection, disconnection, and reconnection tests
- `WIFI_PERFORMANCE` - Signal strength, channel switching, power management
- `WIFI_STRESS_TESTING` - Multiple connect/disconnect cycles
- `WIFI_ERROR_HANDLING` - Invalid credentials and timeout handling

### 2. Bluetooth Comprehensive Test (`BluetoothComprehensiveTest.cpp`)

**Test Sections:**
- `BLUETOOTH_INITIALIZATION` - Bluetooth driver initialization and deinitialization
- `BLUETOOTH_BASIC_OPERATIONS` - Basic operations like scanning and advertising
- `BLUETOOTH_GATT_SERVER` - GATT server functionality tests
- `BLUETOOTH_GATT_CLIENT` - GATT client functionality tests
- `BLUETOOTH_PERFORMANCE` - Signal strength and power control
- `BLUETOOTH_STRESS_TESTING` - Multiple connection/disconnection cycles
- `BLUETOOTH_ERROR_HANDLING` - Invalid operations and timeout handling

### 3. I2C Comprehensive Test (`I2cComprehensiveTest.cpp`)

**Test Sections:**
- `I2C_INITIALIZATION` - I2C driver initialization and deinitialization
- `I2C_BASIC_OPERATIONS` - Basic I2C read/write operations
- `I2C_ERROR_HANDLING` - Timeout and invalid address handling
- `I2C_PERFORMANCE` - Frequency changes and timing tests
- `I2C_STRESS_TESTING` - Multiple read/write operations

## Usage

### Running All Tests

To run all tests in all sections:

```bash
# Build and flash the test
./scripts/build_app.sh wifi_test Release
./scripts/flash_app.sh flash wifi_test Release

# Monitor output (will run all sections)
./scripts/flash_app.sh monitor
```

### Running Specific Sections

You can run specific test sections using command line arguments:

```bash
# Run only WiFi initialization tests
./scripts/flash_app.sh flash wifi_test Release
./scripts/flash_app.sh monitor -- --section WIFI_INITIALIZATION

# Run multiple WiFi sections
./scripts/flash_app.sh monitor -- --section WIFI_INITIALIZATION --section WIFI_BASIC_OPERATIONS

# Run Bluetooth basic operations only
./scripts/flash_app.sh flash bluetooth_test Release
./scripts/flash_app.sh monitor -- --section BLUETOOTH_BASIC_OPERATIONS

# Run I2C error handling tests
./scripts/flash_app.sh flash i2c_test Release
./scripts/flash_app.sh monitor -- --section I2C_ERROR_HANDLING
```

### Available Commands

Each test supports these command line options:

- `--section SECTION_NAME` - Enable a specific test section
- `--disable-section SECTION_NAME` - Disable a specific test section
- `--list-sections` - List all available test sections
- `--help` - Show help information

### Examples

```bash
# List available sections for WiFi test
./scripts/flash_app.sh monitor -- --list-sections

# Run WiFi connectivity and performance tests only
./scripts/flash_app.sh monitor -- --section WIFI_CONNECTIVITY --section WIFI_PERFORMANCE

# Run all Bluetooth tests except stress testing
./scripts/flash_app.sh monitor -- --disable-section BLUETOOTH_STRESS_TESTING

# Run I2C basic operations and error handling
./scripts/flash_app.sh monitor -- --section I2C_BASIC_OPERATIONS --section I2C_ERROR_HANDLING
```

## Test Output

Each test section provides:

1. **Section Header**: Clear indication of which section is running
2. **Individual Test Results**: ✓ for passed tests, ✗ for failed tests
3. **Section Summary**: Total tests, passed tests, and success rate
4. **Overall Results**: Comprehensive summary across all sections

### Sample Output

```
I (1234) WIFI_COMPREHENSIVE_TEST: Running test section: WIFI_INITIALIZATION
I (1235) WIFI_COMPREHENSIVE_TEST: ✓ WiFi initialization test passed
I (1236) WIFI_COMPREHENSIVE_TEST: ✓ WiFi deinitialization test passed
I (1237) WIFI_COMPREHENSIVE_TEST: Section WIFI_INITIALIZATION completed: 2/2 tests passed

I (1238) WIFI_COMPREHENSIVE_TEST: === WIFI COMPREHENSIVE TEST RESULTS ===
I (1239) WIFI_COMPREHENSIVE_TEST: Section: WIFI_INITIALIZATION
I (1240) WIFI_COMPREHENSIVE_TEST:   Tests: 2/2 passed
I (1241) WIFI_COMPREHENSIVE_TEST: === OVERALL RESULTS ===
I (1242) WIFI_COMPREHENSIVE_TEST: Total Tests: 2
I (1243) WIFI_COMPREHENSIVE_TEST: Passed: 2
I (1244) WIFI_COMPREHENSIVE_TEST: Failed: 0
I (1245) WIFI_COMPREHENSIVE_TEST: Success Rate: 100.0%
I (1246) WIFI_COMPREHENSIVE_TEST: 🎉 ALL TESTS PASSED! 🎉
```

## Configuration

### WiFi Configuration

Update the WiFi credentials in `WifiComprehensiveTest.cpp`:

```cpp
#define WIFI_SSID      "YOUR_WIFI_SSID"
#define WIFI_PASS      "YOUR_WIFI_PASSWORD"
```

### I2C Configuration

Update the I2C pin configuration in `I2cComprehensiveTest.cpp`:

```cpp
#define I2C_MASTER_SCL_IO           22      // SCL pin
#define I2C_MASTER_SDA_IO           21      // SDA pin
#define I2C_MASTER_FREQ_HZ          100000  // I2C frequency
```

### Bluetooth Configuration

The Bluetooth test uses default BLE configuration. Modify device name and UUIDs in `BluetoothComprehensiveTest.cpp` if needed:

```cpp
#define DEVICE_NAME "ESP32_BLE_TEST"
#define SERVICE_UUID    0x00FF
#define CHARACTERISTIC_UUID 0xFF01
```

## Building and Flashing

### Prerequisites

- ESP-IDF environment set up
- Target ESP32 device connected
- Serial port access

### Build Commands

```bash
# Build WiFi test
./scripts/build_app.sh wifi_test Release

# Build Bluetooth test
./scripts/build_app.sh bluetooth_test Release

# Build I2C test
./scripts/build_app.sh i2c_test Release
```

### Flash Commands

```bash
# Flash WiFi test
./scripts/flash_app.sh flash wifi_test Release

# Flash Bluetooth test
./scripts/flash_app.sh flash bluetooth_test Release

# Flash I2C test
./scripts/flash_app.sh flash i2c_test Release
```

## Troubleshooting

### Common Issues

1. **WiFi Connection Failures**
   - Verify WiFi credentials are correct
   - Check WiFi network availability
   - Ensure ESP32 has sufficient power

2. **Bluetooth Initialization Errors**
   - Check if Bluetooth is supported on your ESP32 variant
   - Verify NVS flash is working properly
   - Ensure no other Bluetooth processes are running

3. **I2C Communication Issues**
   - Verify I2C device is connected and powered
   - Check I2C pin connections
   - Ensure proper pull-up resistors

### Debug Output

Enable verbose logging by modifying the log level in each test file:

```cpp
esp_log_level_set("*", ESP_LOG_VERBOSE);
```

## Extending the Tests

### Adding New Test Sections

1. Add the section name to the `test_sections` array
2. Implement the section logic in `run_test_section()`
3. Add individual test functions
4. Update the section test count and logic

### Adding New Test Functions

1. Declare the function prototype
2. Implement the test logic
3. Add proper error handling
4. Return `ESP_OK` for success, error code for failure

### Example: Adding a New WiFi Section

```cpp
// Add to test_sections array
{"WIFI_SECURITY", true, 0, 0, 0},

// Add to run_test_section() function
} else if (strcmp(section->section_name, "WIFI_SECURITY") == 0) {
    section->total_tests = 2;
    
    if (test_wifi_wpa3() == ESP_OK) {
        section->passed_tests++;
        ESP_LOGI(TAG, "✓ WiFi WPA3 test passed");
    } else {
        section->failed_tests++;
        ESP_LOGE(TAG, "✗ WiFi WPA3 test failed");
    }
    
    // Add more tests...
}

// Implement the test function
esp_err_t test_wifi_wpa3(void)
{
    // Test WPA3 functionality
    return ESP_OK;
}
```

## Contributing

When adding new tests or modifying existing ones:

1. Follow the existing code structure and naming conventions
2. Add proper error handling and logging
3. Include comprehensive test coverage
4. Update this documentation
5. Test on multiple ESP32 variants if possible

## License

These test suites are part of the ESP32 development tools and follow the same licensing terms as the ESP-IDF framework.