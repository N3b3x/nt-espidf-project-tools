#ifndef GPIO_COMPREHENSIVE_TEST_H
#define GPIO_COMPREHENSIVE_TEST_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <chrono>

// GPIO Test Section Definitions
enum class GpioTestSection {
    BASIC_GPIO_OPERATIONS,
    GPIO_INTERRUPTS,
    GPIO_PWM_FUNCTIONALITY,
    GPIO_ANALOG_READS,
    GPIO_STRESS_TESTING,
    GPIO_EDGE_CASES,
    ALL_SECTIONS
};

// Test Result Structure
struct TestResult {
    std::string test_name;
    bool passed;
    std::string message;
    double execution_time_ms;
    
    TestResult(const std::string& name, bool pass, const std::string& msg, double time)
        : test_name(name), passed(pass), message(msg), execution_time_ms(time) {}
};

// Test Section Structure
struct TestSection {
    std::string name;
    std::string description;
    std::vector<std::function<TestResult()>> tests;
    bool enabled;
    int timeout_seconds;
    
    TestSection(const std::string& n, const std::string& desc, bool en = true, int timeout = 30)
        : name(n), description(desc), enabled(en), timeout_seconds(timeout) {}
};

// Test Configuration Structure
struct TestConfig {
    bool verbose_output;
    bool stop_on_failure;
    bool generate_report;
    std::string report_filename;
    int default_timeout_seconds;
    
    TestConfig()
        : verbose_output(false), stop_on_failure(false), generate_report(true),
          report_filename("gpio_test_report.txt"), default_timeout_seconds(30) {}
};

class GpioComprehensiveTest {
private:
    std::map<GpioTestSection, TestSection> test_sections;
    std::vector<TestResult> all_results;
    TestConfig config;
    
    void initializeTestSections();
    void populateTestSections();
    void printSectionSummary(const TestSection& section, const std::vector<TestResult>& results);
    void generateTestReport();
    
public:
    GpioComprehensiveTest();
    GpioComprehensiveTest(const TestConfig& cfg);
    
    // Test Execution Methods
    void runSection(GpioTestSection section);
    void runAllSections();
    void runSelectedSections(const std::vector<GpioTestSection>& sections);
    
    // Configuration Methods
    void enableSection(GpioTestSection section, bool enable = true);
    void disableSection(GpioTestSection section);
    void setSectionTimeout(GpioTestSection section, int timeout_seconds);
    void setConfig(const TestConfig& cfg);
    
    // Information Methods
    void listSections() const;
    void listEnabledSections() const;
    void printOverallSummary() const;
    const std::vector<TestResult>& getResults() const { return all_results; }
    
    // Utility Methods
    void clearResults();
    void exportResults(const std::string& filename) const;
    bool hasFailures() const;
    
    // Individual Test Functions (to be implemented in derived classes or separate files)
    static TestResult testGpioPinConfiguration();
    static TestResult testGpioDigitalWrite();
    static TestResult testGpioDigitalRead();
    static TestResult testGpioPinMode();
    static TestResult testGpioPullUpPullDown();
    
    static TestResult testGpioRisingEdgeInterrupt();
    static TestResult testGpioFallingEdgeInterrupt();
    static TestResult testGpioBothEdgeInterrupt();
    static TestResult testGpioInterruptDebouncing();
    static TestResult testGpioInterruptPriority();
    
    static TestResult testGpioPwmFrequency();
    static TestResult testGpioPwmDutyCycle();
    static TestResult testGpioPwmResolution();
    static TestResult testGpioPwmStability();
    
    static TestResult testGpioAdcResolution();
    static TestResult testGpioAdcAccuracy();
    static TestResult testGpioAdcLinearity();
    static TestResult testGpioAdcNoise();
    
    static TestResult testGpioHighFrequencyToggle();
    static TestResult testGpioLoadHandling();
    static TestResult testGpioTemperatureStability();
    static TestResult testGpioLongTermReliability();
    
    static TestResult testGpioInvalidPinNumbers();
    static TestResult testGpioInvalidConfigurations();
    static TestResult testGpioBoundaryValues();
    static TestResult testGpioErrorRecovery();
};

// Utility functions for string conversion
std::string gpioTestSectionToString(GpioTestSection section);
GpioTestSection stringToGpioTestSection(const std::string& str);

#endif // GPIO_COMPREHENSIVE_TEST_H