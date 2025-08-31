# GPIO Comprehensive Test - Sectioned Approach

This document describes how to use the sectioned GPIO comprehensive test system, which allows you to run specific sections of GPIO tests independently rather than running all tests at once.

## Overview

The sectioned GPIO comprehensive test is designed to provide flexibility in testing GPIO functionality by grouping related tests into logical sections. This approach offers several benefits:

- **Selective Testing**: Run only the sections you need to test
- **Faster Development**: Test specific functionality without waiting for the entire test suite
- **Better Debugging**: Isolate issues to specific test sections
- **Configurable**: Easy to enable/disable sections and configure test parameters
- **Modular**: Add new test sections without affecting existing ones

## Test Sections

The GPIO comprehensive test is organized into the following sections:

### 1. Basic GPIO Operations (`basic_gpio_operations`)
Tests fundamental GPIO functionality:
- Pin configuration
- Digital write operations
- Digital read operations
- Pin mode setting
- Pull-up/pull-down resistor configuration

**Timeout**: 25 seconds
**Tests**: 5 tests

### 2. GPIO Interrupts (`gpio_interrupts`)
Tests GPIO interrupt functionality:
- Rising edge interrupt detection
- Falling edge interrupt detection
- Both edge interrupt detection
- Interrupt debouncing
- Interrupt priority handling

**Timeout**: 35 seconds
**Tests**: 5 tests

### 3. GPIO PWM Functionality (`gpio_pwm_functionality`)
Tests PWM generation capabilities:
- PWM frequency control
- PWM duty cycle accuracy
- PWM resolution testing
- PWM stability under load

**Timeout**: 40 seconds
**Tests**: 4 tests

### 4. GPIO Analog Reads (`gpio_analog_reads`)
Tests analog input functionality:
- ADC resolution accuracy
- ADC linearity
- ADC noise characteristics
- Voltage measurement accuracy

**Timeout**: 30 seconds
**Tests**: 4 tests

### 5. GPIO Stress Testing (`gpio_stress_testing`)
Tests GPIO reliability under stress:
- High-frequency toggle operations
- Load handling capabilities
- Temperature stability
- Long-term reliability

**Timeout**: 120 seconds
**Tests**: 4 tests

### 6. GPIO Edge Cases (`gpio_edge_cases`)
Tests boundary conditions and error handling:
- Invalid pin numbers
- Invalid configurations
- Boundary values
- Error recovery mechanisms

**Timeout**: 20 seconds
**Tests**: 4 tests

## Usage

### Command Line Interface

The test runner provides a command-line interface for easy control:

```bash
# List all available test sections
python3 run_gpio_tests.py --list

# Run all enabled test sections
python3 run_gpio_tests.py --all

# Run specific sections
python3 run_gpio_tests.py basic_gpio_operations gpio_interrupts

# Enable a section
python3 run_gpio_tests.py --enable gpio_stress_testing

# Disable a section
python3 run_gpio_tests.py --disable gpio_edge_cases

# Show configuration summary
python3 run_gpio_tests.py --summary

# Use a different configuration file
python3 run_gpio_tests.py --config my_config.json
```

### C++ Interface

You can also use the C++ interface directly:

```cpp
#include "GpioComprehensiveTest.h"

int main() {
    GpioComprehensiveTest test;
    
    // List available sections
    test.listSections();
    
    // Run a specific section
    test.runSection(GpioTestSection::BASIC_GPIO_OPERATIONS);
    
    // Run all sections
    test.runAllSections();
    
    // Enable/disable sections
    test.enableSection(GpioTestSection::GPIO_STRESS_TESTING, true);
    test.disableSection(GpioTestSection::GPIO_EDGE_CASES);
    
    return 0;
}
```

## Configuration

### JSON Configuration File

The test system uses a JSON configuration file (`gpio_test_config.json`) to control test behavior:

```json
{
  "test_configuration": {
    "verbose_output": false,
    "stop_on_failure": false,
    "generate_report": true,
    "report_filename": "gpio_test_report.txt",
    "default_timeout_seconds": 30
  },
  "test_sections": {
    "basic_gpio_operations": {
      "enabled": true,
      "timeout_seconds": 25,
      "description": "Basic GPIO functionality",
      "tests": ["gpio_pin_configuration", "gpio_digital_write"]
    }
  },
  "test_execution": {
    "run_mode": "selected_sections",
    "selected_sections": ["basic_gpio_operations", "gpio_interrupts"],
    "exclude_sections": ["gpio_stress_testing"],
    "parallel_execution": false,
    "max_parallel_tests": 1
  }
}
```

### Configuration Options

#### Test Configuration
- `verbose_output`: Enable detailed test output
- `stop_on_failure`: Stop testing when a test fails
- `generate_report`: Generate test report files
- `report_filename`: Name of the test report file
- `default_timeout_seconds`: Default timeout for test sections

#### Test Execution
- `run_mode`: How to determine which sections to run
  - `"all_sections"`: Run all enabled sections
  - `"selected_sections"`: Run only specified sections
  - `"exclude_sections"`: Run all enabled sections except specified ones
- `selected_sections`: List of sections to run (when run_mode is "selected_sections")
- `exclude_sections`: List of sections to exclude (when run_mode is "exclude_sections")
- `parallel_execution`: Enable parallel test execution
- `max_parallel_tests`: Maximum number of tests to run in parallel

## Adding New Test Sections

To add a new test section, follow these steps:

### 1. Update the Header File

Add the new section to the `GpioTestSection` enum in `GpioComprehensiveTest.h`:

```cpp
enum class GpioTestSection {
    BASIC_GPIO_OPERATIONS,
    GPIO_INTERRUPTS,
    GPIO_PWM_FUNCTIONALITY,
    GPIO_ANALOG_READS,
    GPIO_STRESS_TESTING,
    GPIO_EDGE_CASES,
    NEW_SECTION_NAME,  // Add your new section here
    ALL_SECTIONS
};
```

### 2. Add Test Functions

Add your test function declarations to the `GpioComprehensiveTest` class:

```cpp
// Individual Test Functions
static TestResult testNewFunctionality1();
static TestResult testNewFunctionality2();
static TestResult testNewFunctionality3();
```

### 3. Update the Implementation

In `GpioComprehensiveTest.cpp`, add your test section initialization:

```cpp
void GpioComprehensiveTest::initializeTestSections() {
    // ... existing sections ...
    
    // New Test Section
    test_sections[GpioTestSection::NEW_SECTION_NAME] = {
        "New Section Name",
        "Description of what this section tests",
        {},
        true
    };
    
    // ... rest of the function ...
}

void GpioComprehensiveTest::populateTestSections() {
    // ... existing sections ...
    
    // New Section Tests
    auto& new_section = test_sections[GpioTestSection::NEW_SECTION_NAME];
    new_section.tests = {
        []() { return testNewFunctionality1(); },
        []() { return testNewFunctionality2(); },
        []() { return testNewFunctionality3(); }
    };
}
```

### 4. Implement Test Functions

Implement your actual test functions:

```cpp
TestResult GpioComprehensiveTest::testNewFunctionality1() {
    // Your test implementation here
    bool test_passed = true;  // Your test logic
    std::string message = "Test completed successfully";
    
    return {"New Functionality Test 1", test_passed, message, 0.0};
}
```

### 5. Update Configuration

Add your new section to the JSON configuration file:

```json
{
  "test_sections": {
    "new_section_name": {
      "enabled": true,
      "timeout_seconds": 45,
      "description": "Description of what this section tests",
      "tests": [
        "test_new_functionality1",
        "test_new_functionality2",
        "test_new_functionality3"
      ]
    }
  }
}
```

## Best Practices

### 1. Test Organization
- Group related tests into logical sections
- Keep sections focused on specific functionality
- Use descriptive section names and descriptions

### 2. Timeout Configuration
- Set appropriate timeouts for each section
- Consider hardware limitations when setting timeouts
- Use longer timeouts for stress testing sections

### 3. Error Handling
- Implement proper error handling in test functions
- Provide meaningful error messages
- Consider using the `stop_on_failure` option during development

### 4. Reporting
- Enable test reports for debugging
- Use verbose output during development
- Save test logs for analysis

### 5. Configuration Management
- Use different configuration files for different test scenarios
- Version control your configuration files
- Document configuration changes

## Troubleshooting

### Common Issues

1. **Section Not Found**
   - Check that the section name exists in the configuration
   - Verify the section is properly initialized in the C++ code

2. **Tests Not Running**
   - Ensure the section is enabled in the configuration
   - Check that the section has tests assigned to it

3. **Timeout Issues**
   - Adjust timeout values in the configuration
   - Check for hardware issues that might cause delays

4. **Configuration File Errors**
   - Validate JSON syntax
   - Check file permissions
   - Verify file path is correct

### Debug Mode

Enable verbose output to get more detailed information:

```json
{
  "test_configuration": {
    "verbose_output": true
  }
}
```

## Integration with ESP-IDF

To integrate this test system with ESP-IDF:

1. **Include in your project**: Add the test files to your ESP-IDF project
2. **Configure CMakeLists.txt**: Ensure the test files are compiled
3. **Set up test environment**: Configure GPIO pins and hardware connections
4. **Run tests**: Use the test runner or integrate with ESP-IDF's test framework

## Example Workflow

Here's a typical workflow for using the sectioned GPIO test:

1. **Initial Setup**
   ```bash
   # List available sections
   python3 run_gpio_tests.py --list
   
   # Check configuration
   python3 run_gpio_tests.py --summary
   ```

2. **Development Testing**
   ```bash
   # Test basic functionality first
   python3 run_gpio_tests.py basic_gpio_operations
   
   # Test interrupts
   python3 run_gpio_tests.py gpio_interrupts
   ```

3. **Full Testing**
   ```bash
   # Run all enabled sections
   python3 run_gpio_tests.py --all
   ```

4. **Custom Configuration**
   ```bash
   # Enable stress testing
   python3 run_gpio_tests.py --enable gpio_stress_testing
   
   # Run with custom config
   python3 run_gpio_tests.py --config production_config.json
   ```

## Conclusion

The sectioned GPIO comprehensive test provides a flexible and maintainable approach to testing GPIO functionality. By organizing tests into logical sections and providing easy configuration options, you can efficiently test specific functionality during development while maintaining the ability to run comprehensive tests when needed.

For questions or issues, refer to the main project documentation or create an issue in the project repository.