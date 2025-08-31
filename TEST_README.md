# Logger and ASCII Art Comprehensive Test Suite

This directory contains comprehensive test suites for both Logger and ASCII Art functionality, designed with a sectioned architecture that allows you to test individual components independently.

## 🏗️ Architecture Overview

Both test suites follow the same modular design pattern:

- **TestSection Base Class**: Abstract base class for all test sections
- **Individual Test Sections**: Focused tests for specific functionality areas
- **TestRunner**: Orchestrates test execution and provides command-line interface
- **Comprehensive Coverage**: Tests cover basic functionality, edge cases, and performance

## 📁 Files

- `LoggerComprehensiveTest.cpp` - Logger functionality test suite
- `AsciiArtComprehensiveTest.cpp` - ASCII art generator test suite
- `Makefile` - Build and test automation
- `TEST_README.md` - This documentation

## 🚀 Quick Start

### Building the Tests

```bash
# Build all test executables
make all

# Build specific test
make LoggerComprehensiveTest
make AsciiComprehensiveTest
```

### Running All Tests

```bash
# Run all logger tests
make test-logger

# Run all ASCII tests
make test-ascii

# Run both test suites
make test-all
```

## 🔍 Test Section Management

### Listing Available Sections

```bash
# List logger test sections
make list-logger

# List ASCII test sections
make list-ascii

# Or use the executables directly
./LoggerComprehensiveTest --list
./AsciiComprehensiveTest --list
```

### Running Individual Sections

#### By Section Name
```bash
# Run specific logger section
make run-logger-section SECTION="Basic Functionality"

# Run specific ASCII section
make run-ascii-section SECTION="Pixel Operations"
```

#### By Section Index
```bash
# Run logger section by index
make run-logger-index INDEX=1

# Run ASCII section by index
make run-ascii-index INDEX=2
```

#### Direct Execution
```bash
# Run specific section by name
./LoggerComprehensiveTest --section "Basic Functionality"
./AsciiComprehensiveTest --section "Drawing Operations"

# Run specific section by index
./LoggerComprehensiveTest --index 1
./AsciiComprehensiveTest --index 3
```

## 📋 Logger Test Sections

1. **Basic Functionality** - Logger creation, configuration, and basic operations
2. **Logging Operations** - All log levels, filtering, and message handling
3. **Buffer Management** - Log buffer operations and state management
4. **Performance** - High-volume logging and performance benchmarks
5. **Edge Cases** - Boundary conditions and unusual inputs

## 🎨 ASCII Art Test Sections

1. **Basic Functionality** - Generator creation, dimensions, and configuration
2. **Pixel Operations** - Individual pixel manipulation and bounds checking
3. **Drawing Operations** - Lines, rectangles, circles, and geometric shapes
4. **Text Rendering** - Text drawing and character placement
5. **Canvas Management** - Canvas clearing, filling, and state management
6. **Performance** - High-volume operations and drawing performance
7. **Edge Cases** - Zero dimensions, large canvases, and special characters

## 🛠️ Command Line Options

Both test executables support the same command-line interface:

```bash
--list, -l                    List all test sections
--all, -a                     Run all test sections
--section <name>, -s <name>   Run specific section by name
--index <number>, -i <number> Run specific section by index
--help, -h                    Show help message
```

## 📊 Test Results

Each test section provides:
- ✅/❌ Pass/Fail status
- Detailed error messages for failed tests
- Performance metrics where applicable
- Comprehensive coverage of functionality

## 🔧 Customization

### Adding New Test Sections

1. Create a new class inheriting from `TestSection`
2. Implement the `run()` method with your test logic
3. Add the section to the `TestRunner` constructor
4. Use the existing assertion methods for consistent testing

### Example Test Section Structure

```cpp
class MyCustomTest : public TestSection {
public:
    MyCustomTest() : TestSection("Custom Test", "Description of what this tests") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        
        // Your test logic here
        assertTrue(condition, "Description of assertion");
        assertEquals(expected, actual, "Description of comparison");
        
        return passed;
    }
};
```

## 🧪 Testing Strategies

### Development Workflow
1. **Focus on Specific Areas**: Use section-based testing during development
2. **Quick Validation**: Run individual sections for rapid feedback
3. **Full Regression**: Run all tests before commits
4. **Performance Monitoring**: Use performance tests to catch regressions

### Debugging Failed Tests
1. Run the specific failing section
2. Check the detailed error output
3. Use the assertion messages to identify the issue
4. Verify the expected vs. actual values

## 📈 Performance Considerations

- **Performance Tests**: Include timing benchmarks for critical operations
- **Memory Usage**: Monitor memory allocation in high-volume tests
- **Scalability**: Test with various input sizes and complexity levels

## 🔍 Troubleshooting

### Common Issues

1. **Compilation Errors**: Ensure C++17 support (`-std=c++17`)
2. **Runtime Errors**: Check for missing dependencies or system resources
3. **Test Failures**: Review assertion messages and expected vs. actual values

### Debug Mode

For debugging, you can modify the Makefile to include debug flags:

```bash
# Add debug information
make CXXFLAGS="-std=c++17 -Wall -Wextra -g -O0"
```

## 📚 Best Practices

1. **Test Isolation**: Each section should be independent
2. **Clear Assertions**: Use descriptive assertion messages
3. **Edge Case Coverage**: Include boundary conditions and error cases
4. **Performance Monitoring**: Track performance metrics over time
5. **Documentation**: Keep test descriptions clear and up-to-date

## 🤝 Contributing

When adding new tests:
1. Follow the existing naming conventions
2. Include comprehensive assertions
3. Add appropriate error messages
4. Update this documentation
5. Ensure tests pass consistently

## 📞 Support

For issues or questions about the test suites:
1. Check the command-line help (`--help`)
2. Review the test output for error details
3. Verify your test environment and dependencies
4. Check the Makefile for build issues