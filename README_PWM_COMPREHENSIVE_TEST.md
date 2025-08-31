# PWM Comprehensive Test Suite

A sectioned and grouped comprehensive testing framework for ESP32 PWM functionality, designed to allow testing of specific sections and groups individually.

## 🏗️ **Architecture Overview**

The PWM comprehensive test is organized into **sections** and **groups** for modular testing:

### **Test Sections**
- **Basic PWM Tests** - Core PWM functionality and initialization
- **Frequency Tests** - PWM frequency range and accuracy testing
- **Duty Cycle Tests** - PWM duty cycle setting and validation
- **Pin Tests** - Multi-pin PWM testing
- **Performance Tests** - High-frequency and timing performance
- **Error Handling Tests** - Invalid parameter and edge case testing
- **Stress Tests** - Long-term operation and stability testing

### **Test Groups** (within each section)
- **Initialization** - Setup and configuration tests
- **Functionality** - Core feature testing
- **Validation** - Output verification and measurement
- **Cleanup** - Resource cleanup and teardown

## 🚀 **Quick Start**

### **Run All Tests**
```cpp
void setup() {
    Serial.begin(115200);
    run_pwm_comprehensive_tests();  // Runs all sections and groups
}
```

### **Run Specific Test Sections**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Run only basic PWM tests
    TestSection sections[] = {SECTION_BASIC_PWM};
    run_specific_test_sections(sections, 1);
}
```

### **Run Multiple Sections**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Run frequency and duty cycle tests only
    TestSection sections[] = {SECTION_FREQUENCY_TESTS, SECTION_DUTY_CYCLE_TESTS};
    run_specific_test_sections(sections, 2);
}
```

## 🔧 **Test Control Functions**

### **Section Control**
```cpp
// Enable/disable specific test sections
enable_test_section(SECTION_BASIC_PWM, true);      // Enable basic tests
enable_test_section(SECTION_STRESS_TESTS, false);  // Disable stress tests

// Run only enabled sections
run_pwm_comprehensive_tests();
```

### **Group Control**
```cpp
// Enable/disable specific test groups
enable_test_group(GROUP_INITIALIZATION, true);     // Enable init tests
enable_test_group(GROUP_CLEANUP, false);           // Disable cleanup tests

// Run tests with group filtering
run_pwm_comprehensive_tests();
```

### **Combined Control**
```cpp
// Custom configuration: Only validation groups in basic PWM section
for (int i = 0; i < SECTION_MAX; i++) {
    enable_test_section((TestSection)i, false);
}
enable_test_section(SECTION_BASIC_PWM, true);

for (int i = 0; i < GROUP_MAX; i++) {
    enable_test_group((TestGroup)i, false);
}
enable_test_group(GROUP_VALIDATION, true);

run_pwm_comprehensive_tests();
```

## 📋 **Usage Examples**

### **Example 1: Development Testing**
```cpp
void setup() {
    Serial.begin(115200);
    
    // During development, run only basic tests
    TestSection dev_sections[] = {SECTION_BASIC_PWM};
    run_specific_test_sections(dev_sections, 1);
}
```

### **Example 2: Hardware Validation**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Validate hardware with basic and pin tests
    TestSection hw_sections[] = {SECTION_BASIC_PWM, SECTION_PIN_TESTS};
    run_specific_test_sections(hw_sections, 2);
}
```

### **Example 3: Performance Testing**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Focus on performance and stress testing
    TestSection perf_sections[] = {SECTION_PERFORMANCE_TESTS, SECTION_STRESS_TESTS};
    run_specific_test_sections(perf_sections, 2);
}
```

### **Example 4: Sequential Testing**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Run tests in phases with delays
    Serial.println("=== PHASE 1: Basic Tests ===");
    TestSection basic[] = {SECTION_BASIC_PWM};
    run_specific_test_sections(basic, 1);
    
    delay(2000);
    
    Serial.println("=== PHASE 2: Frequency Tests ===");
    TestSection freq[] = {SECTION_FREQUENCY_TESTS};
    run_specific_test_sections(freq, 1);
}
```

## ⚙️ **Configuration**

### **Test Parameters**
```cpp
// Modify these defines to change test behavior
#define PWM_TEST_PIN 18           // GPIO pin for testing
#define PWM_FREQ_HZ 1000         // Default test frequency
#define PWM_RESOLUTION LEDC_TIMER_13_BIT  // PWM resolution
#define PWM_DUTY_CYCLE 50        // Default duty cycle
```

### **Hardware-Dependent Testing**
```cpp
void setup() {
    Serial.begin(115200);
    
    // Conditional testing based on available hardware
    bool has_oscilloscope = check_oscilloscope_availability();
    bool has_multimeter = check_multimeter_availability();
    
    if (has_oscilloscope) {
        enable_test_section(SECTION_PERFORMANCE_TESTS, true);
    }
    
    if (has_multimeter) {
        enable_test_section(SECTION_BASIC_PWM, true);
    }
    
    run_pwm_comprehensive_tests();
}
```

## 📊 **Test Results**

### **Output Format**
```
==================================================
SECTION: Basic PWM Tests
==================================================

--- GROUP: Initialization Tests ---
✅ PASS: PWM Timer Configuration (100 ms)

--- GROUP: Functionality Tests ---
✅ PASS: PWM Frequency Setting (150 ms)
✅ PASS: PWM Duty Cycle Setting (120 ms)

==================================================
TEST SUMMARY
==================================================
Total Tests: 15
Passed: 15
Failed: 0
Success Rate: 100.0%
```

### **Result Analysis**
The test framework provides:
- Individual test results with execution times
- Group and section organization
- Overall success/failure summary
- Execution time tracking for performance analysis

## 🔍 **Customization**

### **Adding New Test Sections**
```cpp
enum TestSection {
    SECTION_BASIC_PWM = 0,
    SECTION_FREQUENCY_TESTS,
    SECTION_DUTY_CYCLE_TESTS,
    SECTION_PIN_TESTS,
    SECTION_PERFORMANCE_TESTS,
    SECTION_ERROR_HANDLING,
    SECTION_STRESS_TESTS,
    SECTION_CUSTOM_TESTS,  // Add new section
    SECTION_MAX
};

void run_custom_tests() {
    if (!test_section_enabled[SECTION_CUSTOM_TESTS]) {
        return;
    }
    
    print_test_section_header("Custom Tests");
    // Implement custom test logic
}
```

### **Adding New Test Groups**
```cpp
enum TestGroup {
    GROUP_INITIALIZATION = 0,
    GROUP_FUNCTIONALITY,
    GROUP_VALIDATION,
    GROUP_CLEANUP,
    GROUP_CUSTOM_GROUP,  // Add new group
    GROUP_MAX
};
```

### **Implementing Individual Tests**
```cpp
bool test_custom_functionality() {
    // Implement actual test logic here
    // Return true for pass, false for fail
    
    // Example: Test PWM frequency accuracy
    uint32_t target_freq = 1000;
    uint32_t actual_freq = measure_pwm_frequency();
    
    uint32_t error = abs((int32_t)(actual_freq - target_freq));
    return error <= 50;  // Allow 50Hz tolerance
}
```

## 🚨 **Troubleshooting**

### **Common Issues**

1. **Tests not running**: Check if sections/groups are enabled
2. **Compilation errors**: Ensure ESP32 Arduino core is properly installed
3. **Hardware issues**: Verify GPIO pin connections and power supply

### **Debug Mode**
```cpp
// Enable debug output
#define PWM_TEST_DEBUG 1

// Add debug prints in test functions
if (PWM_TEST_DEBUG) {
    Serial.printf("Debug: Testing frequency %lu Hz\n", test_freq);
}
```

## 📁 **File Structure**

```
PwmComprehensiveTest.h          # Header file with declarations
PwmComprehensiveTest.cpp        # Main test implementation
PwmTestUsageExamples.cpp        # Usage examples and patterns
README_PWM_COMPREHENSIVE_TEST.md # This documentation
```

## 🤝 **Contributing**

To add new tests or improve existing ones:

1. Follow the existing section/group structure
2. Implement proper error handling and validation
3. Add appropriate documentation
4. Test with different ESP32 boards and configurations

## 📝 **License**

This test framework is provided as-is for ESP32 development and testing purposes.