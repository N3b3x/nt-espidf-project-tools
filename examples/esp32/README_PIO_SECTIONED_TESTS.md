# PIO Comprehensive Test Suite - Sectioned Testing

This document describes the **sectioned PIO comprehensive test suite** that allows you to test individual PIO (Programmable Input/Output) functionality areas independently, similar to the I2C comprehensive test structure.

## 📋 Overview

The PIO comprehensive test is organized into **8 distinct test sections** that can be enabled or disabled individually. This modular approach allows for:

- **Targeted Testing**: Focus on specific PIO functionality areas
- **Debugging**: Isolate issues to specific subsystems  
- **Development**: Test incremental changes without running the full suite
- **CI/CD Integration**: Run different test combinations for different scenarios

## 🧩 Test Sections

### Section 1: Basic PIO Configuration Tests (`CONFIG_PIO_TEST_BASIC_CONFIG`)
- **Purpose**: Verify fundamental PIO initialization and setup
- **Tests**:
  - PIO block initialization
  - State machine allocation/deallocation
  - GPIO pin configuration for PIO use
- **When to Use**: Always run first - validates basic PIO functionality

### Section 2: State Machine Program Loading Tests (`CONFIG_PIO_TEST_STATE_MACHINE`)
- **Purpose**: Test PIO assembly program loading and execution
- **Tests**:
  - Program loading into PIO memory
  - State machine initialization with programs
  - Basic program execution verification
- **When to Use**: When developing or debugging PIO assembly programs

### Section 3: FIFO Operations Tests (`CONFIG_PIO_TEST_FIFO_OPERATIONS`)
- **Purpose**: Validate data communication between CPU and PIO
- **Tests**:
  - TX FIFO operations (put, level, status)
  - RX FIFO operations (get, level, status) 
  - FIFO full/empty condition handling
- **When to Use**: When troubleshooting data transfer issues

### Section 4: GPIO Interaction Tests (`CONFIG_PIO_TEST_GPIO_INTERACTION`)
- **Purpose**: Verify PIO control of GPIO pins
- **Tests**:
  - GPIO output control through PIO
  - GPIO input reading through PIO
  - Dynamic GPIO direction changes
- **When to Use**: When testing PIO-GPIO interface functionality

### Section 5: Interrupt Handling Tests (`CONFIG_PIO_TEST_INTERRUPTS`)
- **Purpose**: Test PIO interrupt system
- **Tests**:
  - Interrupt handler setup and configuration
  - Interrupt triggering and response
  - Interrupt cleanup and teardown
- **When to Use**: When implementing interrupt-driven PIO applications

### Section 6: DMA Integration Tests (`CONFIG_PIO_TEST_DMA_INTEGRATION`)
- **Purpose**: Validate PIO-DMA integration for high-speed transfers
- **Tests**:
  - DMA channel allocation and setup
  - PIO-DMA integration configuration
  - DMA transfer operations with PIO
- **When to Use**: When implementing high-performance PIO applications

### Section 7: Performance and Timing Tests (`CONFIG_PIO_TEST_PERFORMANCE`)
- **Purpose**: Measure PIO performance characteristics
- **Tests**:
  - Timing accuracy measurements
  - Clock divider setting validation
  - Throughput benchmarks
- **When to Use**: When optimizing PIO performance or validating timing

### Section 8: Error Handling and Recovery Tests (`CONFIG_PIO_TEST_ERROR_HANDLING`)
- **Purpose**: Verify robustness and error recovery
- **Tests**:
  - Invalid program handling
  - Resource exhaustion scenarios
  - Recovery procedure validation
- **When to Use**: When validating system robustness and reliability

## ⚙️ Configuration Methods

### Method 1: Menuconfig (Recommended)
```bash
idf.py menuconfig
```
Navigate to: `Component config → PIO Comprehensive Test Configuration`

**Individual Section Control:**
- ✅ Enable Basic PIO Configuration Tests
- ✅ Enable State Machine Program Loading Tests  
- ✅ Enable FIFO Operations Tests
- ✅ Enable GPIO Interaction Tests
- ✅ Enable Interrupt Handling Tests
- ✅ Enable DMA Integration Tests
- ✅ Enable Performance and Timing Tests
- ✅ Enable Error Handling and Recovery Tests

**Group Control:**
- ✅ Enable Basic Test Group (Sections 1-2)
- ✅ Enable Communication Test Group (Sections 3-4)
- ✅ Enable Advanced Test Group (Sections 5-6)
- ✅ Enable Diagnostics Test Group (Sections 7-8)

### Method 2: Compile-Time Flags
Add to your `sdkconfig` or build flags:
```ini
# Enable specific sections
CONFIG_PIO_TEST_BASIC_CONFIG=y
CONFIG_PIO_TEST_STATE_MACHINE=y
CONFIG_PIO_TEST_FIFO_OPERATIONS=n
CONFIG_PIO_TEST_GPIO_INTERACTION=y
# ... etc
```

### Method 3: Code-Level Configuration
Modify the defines in `PioComprehensiveTest.cpp`:
```cpp
#define CONFIG_PIO_TEST_BASIC_CONFIG 1      // Enable
#define CONFIG_PIO_TEST_STATE_MACHINE 1     // Enable
#define CONFIG_PIO_TEST_FIFO_OPERATIONS 0   // Disable
#define CONFIG_PIO_TEST_GPIO_INTERACTION 1  // Enable
// ... etc
```

## 🚀 Usage Examples

### Example 1: Full Test Suite
Run all sections (default configuration):
```bash
idf.py build flash monitor
```

### Example 2: Basic Functionality Only
Enable only basic tests for quick validation:
```bash
idf.py menuconfig  # Enable only Sections 1-2
idf.py build flash monitor
```

### Example 3: Development Testing
Test specific functionality during development:
```bash
# Enable only FIFO and GPIO tests
idf.py menuconfig  # Enable Sections 3-4 only
idf.py build flash monitor
```

### Example 4: Performance Analysis
Run only performance and timing tests:
```bash
idf.py menuconfig  # Enable Section 7 only
idf.py build flash monitor
```

### Example 5: CI/CD Integration
Different test combinations for different pipeline stages:

**Quick Smoke Tests:**
```ini
CONFIG_PIO_TEST_GROUP_BASIC=y
CONFIG_PIO_TEST_GROUP_COMMUNICATION=n
CONFIG_PIO_TEST_GROUP_ADVANCED=n
CONFIG_PIO_TEST_GROUP_DIAGNOSTICS=n
```

**Full Regression Tests:**
```ini
CONFIG_PIO_TEST_GROUP_BASIC=y
CONFIG_PIO_TEST_GROUP_COMMUNICATION=y
CONFIG_PIO_TEST_GROUP_ADVANCED=y
CONFIG_PIO_TEST_GROUP_DIAGNOSTICS=y
```

## 📊 Test Output and Results

### Console Output Format
```
🔬 =============================================================================
PIO COMPREHENSIVE TEST SUITE - SECTIONED TESTING
=============================================================================

🔧 =============================================================================
SECTION 1: Basic PIO Configuration Tests
=============================================================================
▶️  Starting: PIO Initialization
✅ PASSED: PIO Initialization PIO0 initialized successfully
▶️  Starting: State Machine Allocation
✅ PASSED: State Machine Allocation Successfully allocated and freed SM

📊 =============================================================================
TEST SUMMARY REPORT
=============================================================================
✅ PASSED Basic PIO Configuration: 3/3 tests passed
✅ PASSED State Machine Programs: 3/3 tests passed
⏭️  SKIPPED FIFO Operations: Section disabled
✅ PASSED GPIO Interaction: 3/3 tests passed
=============================================================================
OVERALL RESULTS:
• Sections Enabled: 3/8
• Total Tests: 9
• Passed: 9
• Failed: 0
• Success Rate: 100.0%
🎉 ALL TESTS PASSED!
```

### Result Interpretation
- **✅ PASSED**: All tests in section completed successfully
- **❌ FAILED**: One or more tests in section failed
- **⏭️ SKIPPED**: Section was disabled in configuration
- **Success Rate**: Overall percentage of tests that passed

## 🔧 Advanced Configuration

### Hardware Configuration
```ini
# Customize hardware settings
CONFIG_PIO_TEST_PIN_BASE=2          # Starting GPIO pin
CONFIG_PIO_TEST_TIMEOUT_MS=5000     # Test timeout
CONFIG_PIO_TEST_DMA_CHANNEL=0       # DMA channel for tests
```

### Test Behavior Configuration
```ini
CONFIG_PIO_TEST_VERBOSE_LOGGING=y        # Detailed logs
CONFIG_PIO_TEST_PERFORMANCE_ITERATIONS=10000  # Benchmark iterations
CONFIG_PIO_TEST_QUICK_MODE=n             # Quick mode (fewer tests)
CONFIG_PIO_TEST_STRESS_MODE=n            # Stress mode (more tests)
```

## 🎯 Common Use Cases

### 1. Initial Hardware Validation
```bash
# Enable basic tests only
CONFIG_PIO_TEST_GROUP_BASIC=y
# Disable other groups
CONFIG_PIO_TEST_GROUP_COMMUNICATION=n
CONFIG_PIO_TEST_GROUP_ADVANCED=n
CONFIG_PIO_TEST_GROUP_DIAGNOSTICS=n
```

### 2. Communication Debug
```bash
# Focus on FIFO and GPIO functionality
CONFIG_PIO_TEST_FIFO_OPERATIONS=y
CONFIG_PIO_TEST_GPIO_INTERACTION=y
# Disable others
CONFIG_PIO_TEST_BASIC_CONFIG=n
CONFIG_PIO_TEST_STATE_MACHINE=n
```

### 3. Performance Optimization
```bash
# Enable performance tests only
CONFIG_PIO_TEST_PERFORMANCE=y
# Increase iteration count for accuracy
CONFIG_PIO_TEST_PERFORMANCE_ITERATIONS=50000
```

### 4. Stress Testing
```bash
# Enable all tests with stress mode
CONFIG_PIO_TEST_STRESS_MODE=y
# All sections enabled
CONFIG_PIO_TEST_GROUP_BASIC=y
CONFIG_PIO_TEST_GROUP_COMMUNICATION=y
CONFIG_PIO_TEST_GROUP_ADVANCED=y
CONFIG_PIO_TEST_GROUP_DIAGNOSTICS=y
```

## 🔍 Troubleshooting

### Common Issues and Solutions

**1. No State Machines Available**
- Check if other PIO code is using state machines
- Verify PIO reset in initialization
- Enable verbose logging for details

**2. GPIO Configuration Failures**  
- Verify pin numbers are valid for your board
- Check for GPIO conflicts with other peripherals
- Adjust `CONFIG_PIO_TEST_PIN_BASE` if needed

**3. DMA Test Failures**
- Ensure DMA channel is available
- Check for DMA conflicts with other drivers
- Try different `CONFIG_PIO_TEST_DMA_CHANNEL` value

**4. Timing Test Inconsistencies**
- Run tests multiple times for averaging
- Check for system load interference
- Use quick mode for faster iteration

### Debug Mode Configuration
```ini
CONFIG_PIO_TEST_VERBOSE_LOGGING=y
CONFIG_LOG_DEFAULT_LEVEL_DEBUG=y
CONFIG_PIO_TEST_TIMEOUT_MS=30000  # Longer timeout for debugging
```

## 🏗️ Integration with Build System

### Build Script Integration
```bash
#!/bin/bash
# Example CI script for sectioned testing

# Quick smoke test
echo "Running basic PIO tests..."
idf.py -D CONFIG_PIO_TEST_GROUP_BASIC=y build flash monitor

# Communication tests
echo "Running communication tests..."
idf.py -D CONFIG_PIO_TEST_GROUP_COMMUNICATION=y build flash monitor

# Full test suite
echo "Running full test suite..."
idf.py build flash monitor
```

### CMake Integration
```cmake
# Add test-specific compiler flags
if(CONFIG_PIO_TEST_VERBOSE_LOGGING)
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE PIO_TEST_VERBOSE=1)
endif()

if(CONFIG_PIO_TEST_STRESS_MODE) 
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE PIO_TEST_STRESS=1)
endif()
```

## 📈 Comparison with I2C Comprehensive Test Structure

The PIO comprehensive test follows the same sectioned approach as the I2C comprehensive test:

| **Aspect** | **I2C Test** | **PIO Test** |
|------------|--------------|--------------|
| **Sections** | 8 configurable sections | 8 configurable sections |
| **Configuration** | Menuconfig + compile flags | Menuconfig + compile flags |
| **Grouping** | Logical test groups | Logical test groups |
| **Reporting** | Section-by-section results | Section-by-section results |
| **CI Integration** | Configurable test combinations | Configurable test combinations |

This ensures consistency across the test infrastructure and allows developers familiar with the I2C test structure to easily use the PIO tests.

## 🎉 Conclusion

The sectioned PIO comprehensive test suite provides:

- **Flexibility**: Test only what you need
- **Efficiency**: Faster iteration during development  
- **Clarity**: Clear section-based organization
- **Scalability**: Easy to add new test sections
- **Integration**: Works well with CI/CD pipelines

Choose the appropriate test sections based on your development phase, debugging needs, or validation requirements!