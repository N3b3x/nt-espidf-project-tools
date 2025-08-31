# SPI Comprehensive Test Suite

This comprehensive SPI test suite provides thorough testing of SPI functionality with the ability to run individual sections or all tests together, similar to the I2C comprehensive test structure.

## Features

- **Sectioned Testing**: Tests are organized into logical sections that can be run individually
- **Grouped Functionality**: Related tests are grouped together for better organization
- **Individual Section Execution**: Run specific test sections without running the entire suite
- **Comprehensive Coverage**: Tests all major SPI functionality including initialization, transfers, modes, and performance
- **Detailed Reporting**: Provides detailed results for each test and section
- **Interactive Interface**: Command-line interface for easy test execution

## Test Sections

The test suite is organized into the following sections:

### 0. Initialization
- SPI Begin/End functionality
- Basic SPI setup and teardown

### 1. Basic Operations
- Single byte transfer
- 16-bit transfer
- 32-bit transfer
- Buffer transfer

### 2. Transfer Modes
- SPI mode testing (MODE0, MODE1, MODE2, MODE3)
- Bit order testing (MSBFIRST, LSBFIRST)
- Clock speed testing

### 3. Clock Speeds
- Various clock divider settings
- Performance at different speeds

### 4. Error Handling
- Invalid parameter handling
- Robustness testing

### 5. Performance
- Transfer rate benchmarking
- Timing measurements

### 6. Stress Testing
- Continuous transfer testing
- Long-term stability testing

## Usage

### Compilation
```bash
# Compile the test suite
arduino-cli compile --fqbn esp32:esp32:esp32 main/SpiComprehensiveTest.cpp

# Or use your preferred Arduino IDE/compiler
```

### Upload and Execution
```bash
# Upload to your ESP32 device
arduino-cli upload --fqbn esp32:esp32:esp32 -p /dev/ttyUSB0 main/SpiComprehensiveTest.cpp

# Monitor the serial output
arduino-cli monitor -p /dev/ttyUSB0
```

### Interactive Commands

Once the test suite is running, you can use the following commands:

- **`all`** - Run all test sections sequentially
- **`0`** - Run only the Initialization section
- **`1`** - Run only the Basic Operations section
- **`2`** - Run only the Transfer Modes section
- **`3`** - Run only the Clock Speeds section
- **`4`** - Run only the Error Handling section
- **`5`** - Run only the Performance section
- **`6`** - Run only the Stress Testing section
- **`help`** - Show available commands
- **`quit`** - Exit the test suite

## Example Output

```
=== SPI Comprehensive Test Suite ===
This test suite provides comprehensive testing of SPI functionality
with the ability to run individual sections or all tests together.

Available test sections:
  0: Initialization
  1: Basic Operations
  2: Transfer Modes
  3: Clock Speeds
  4: Error Handling
  5: Performance
  6: Stress Testing

Commands:
  'all' - Run all test sections
  '0-6' - Run specific section (0-6)
  'help' - Show this help
  'quit' - Exit test suite

SPI initialized with default settings

Enter command (all/0-6/help/quit):
```

## Test Results

Each test section provides detailed results including:

- Total number of tests
- Number of passed/failed tests
- Success rate percentage
- Total and average execution time
- Individual test results with timing

## Hardware Requirements

- ESP32 development board
- SPI-compatible device (optional, for loopback testing)
- USB connection for programming and monitoring

## Configuration

The test suite uses default SPI settings:
- Clock Speed: 1 MHz (SPI_CLOCK_DIV4)
- Mode: SPI_MODE0
- Bit Order: MSBFIRST

You can modify these settings in the `initializeSpi()` function if needed.

## Customization

### Adding New Tests
To add new tests to a section:

1. Create a new test function following the pattern:
   ```cpp
   bool testNewFunctionality() {
       if (!spiInitialized) {
           return false;
       }
       
       // Your test code here
       
       return true; // or false based on test result
   }
   ```

2. Add the function declaration to the function declarations section

3. Add the test to the appropriate section in the `runSection()` function

### Adding New Sections
To add a new test section:

1. Add the section to the `TestSection` enum
2. Add the section name to `SECTION_NAMES[]`
3. Update `TOTAL_SECTIONS`
4. Add the section logic to `runSection()`

## Troubleshooting

### Common Issues

1. **SPI not initializing**: Check your board configuration and SPI pins
2. **Tests failing**: Verify your hardware setup and SPI device compatibility
3. **Serial communication issues**: Ensure correct baud rate (115200) and port selection

### Debug Mode

For debugging, you can add additional Serial output to individual test functions to see exactly where tests are failing.

## Integration with CI/CD

This test suite can be integrated into CI/CD pipelines by:

1. Running specific sections based on test requirements
2. Parsing the output for pass/fail results
3. Using the timing information for performance regression testing
4. Running stress tests for stability validation

## License

This test suite is provided as-is for educational and testing purposes. Modify as needed for your specific requirements.