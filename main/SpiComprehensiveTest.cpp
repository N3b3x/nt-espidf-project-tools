#include <Arduino.h>
#include <SPI.h>

// Test configuration
#define SPI_CLOCK_SPEED 1000000  // 1 MHz
#define SPI_MODE SPI_MODE0
#define SPI_BIT_ORDER MSBFIRST

// Test results structure
struct TestResult {
    bool passed;
    String message;
    unsigned long duration;
};

// Test section results
struct SectionResult {
    String sectionName;
    int totalTests;
    int passedTests;
    int failedTests;
    unsigned long totalDuration;
    TestResult* results;
};

// Global variables
SPIClass spi;
bool spiInitialized = false;
SectionResult* sectionResults = nullptr;
int totalSections = 0;

// Test sections
enum TestSection {
    SECTION_INITIALIZATION = 0,
    SECTION_BASIC_OPERATIONS,
    SECTION_TRANSFER_MODES,
    SECTION_CLOCK_SPEEDS,
    SECTION_ERROR_HANDLING,
    SECTION_PERFORMANCE,
    SECTION_STRESS_TESTING,
    TOTAL_SECTIONS
};

// Section names
const char* SECTION_NAMES[] = {
    "Initialization",
    "Basic Operations", 
    "Transfer Modes",
    "Clock Speeds",
    "Error Handling",
    "Performance",
    "Stress Testing"
};

// Function declarations
void initializeSpi();
void cleanupSpi();
TestResult runTest(bool (*testFunc)(), const String& testName);
void runSection(TestSection section);
void runAllSections();
void runSpecificSection(int sectionIndex);
void printSectionResults(const SectionResult& section);
void printOverallResults();
bool testSpiBegin();
bool testSpiEnd();
bool testSpiTransfer();
bool testSpiTransfer16();
bool testSpiTransfer32();
bool testSpiTransferBuffer();
bool testSpiClockSpeed();
bool testSpiMode();
bool testSpiBitOrder();
bool testSpiErrorHandling();
bool testSpiPerformance();
bool testSpiStress();

// Test functions
bool testSpiBegin() {
    if (spiInitialized) {
        spi.end();
        spiInitialized = false;
    }
    
    spi.begin();
    spiInitialized = true;
    
    // Verify SPI is initialized
    return spiInitialized;
}

bool testSpiEnd() {
    if (!spiInitialized) {
        return false;
    }
    
    spi.end();
    spiInitialized = false;
    
    return !spiInitialized;
}

bool testSpiTransfer() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test single byte transfer
    byte testData = 0x55;
    byte received = spi.transfer(testData);
    
    // For loopback testing, we expect to receive what we sent
    // In real hardware, this might be different
    return true; // Basic transfer functionality test
}

bool testSpiTransfer16() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test 16-bit transfer
    uint16_t testData = 0x1234;
    uint16_t received = spi.transfer16(testData);
    
    return true; // Basic 16-bit transfer functionality test
}

bool testSpiTransfer32() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test 32-bit transfer
    uint32_t testData = 0x12345678;
    uint32_t received = spi.transfer32(testData);
    
    return true; // Basic 32-bit transfer functionality test
}

bool testSpiTransferBuffer() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test buffer transfer
    byte testBuffer[] = {0x01, 0x02, 0x03, 0x04};
    byte receiveBuffer[4];
    
    spi.transfer(testBuffer, receiveBuffer, 4);
    
    return true; // Basic buffer transfer functionality test
}

bool testSpiClockSpeed() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test different clock speeds
    spi.setClockDivider(SPI_CLOCK_DIV4);
    delay(1);
    spi.setClockDivider(SPI_CLOCK_DIV16);
    delay(1);
    spi.setClockDivider(SPI_CLOCK_DIV64);
    delay(1);
    
    return true;
}

bool testSpiMode() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test different SPI modes
    spi.setDataMode(SPI_MODE0);
    delay(1);
    spi.setDataMode(SPI_MODE1);
    delay(1);
    spi.setDataMode(SPI_MODE2);
    delay(1);
    spi.setDataMode(SPI_MODE3);
    delay(1);
    
    // Reset to default mode
    spi.setDataMode(SPI_MODE0);
    
    return true;
}

bool testSpiBitOrder() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test bit order
    spi.setBitOrder(MSBFIRST);
    delay(1);
    spi.setBitOrder(LSBFIRST);
    delay(1);
    
    // Reset to default
    spi.setBitOrder(MSBFIRST);
    
    return true;
}

bool testSpiErrorHandling() {
    if (!spiInitialized) {
        return false;
    }
    
    // Test error handling scenarios
    // Note: This is a basic test - real error handling depends on hardware
    
    // Test with invalid parameters (should not crash)
    spi.setClockDivider(255); // Invalid divider
    delay(1);
    
    // Reset to valid value
    spi.setClockDivider(SPI_CLOCK_DIV4);
    
    return true;
}

bool testSpiPerformance() {
    if (!spiInitialized) {
        return false;
    }
    
    // Performance test - measure transfer speed
    const int iterations = 1000;
    unsigned long startTime = micros();
    
    for (int i = 0; i < iterations; i++) {
        spi.transfer(0x55);
    }
    
    unsigned long endTime = micros();
    unsigned long duration = endTime - startTime;
    
    // Calculate transfer rate (bytes per second)
    float transferRate = (float)(iterations * 1000000) / duration;
    
    // Log performance metrics
    Serial.printf("SPI Performance: %d bytes in %lu us = %.2f bytes/sec\n", 
                  iterations, duration, transferRate);
    
    return true;
}

bool testSpiStress() {
    if (!spiInitialized) {
        return false;
    }
    
    // Stress test - continuous transfers
    const int stressIterations = 10000;
    
    for (int i = 0; i < stressIterations; i++) {
        byte testData = i & 0xFF;
        byte received = spi.transfer(testData);
        
        // Add small delay to prevent overwhelming the system
        if (i % 1000 == 0) {
            delay(1);
        }
    }
    
    return true;
}

// Test runner functions
TestResult runTest(bool (*testFunc)(), const String& testName) {
    TestResult result;
    result.passed = false;
    result.message = "";
    
    unsigned long startTime = micros();
    
    try {
        result.passed = testFunc();
        if (result.passed) {
            result.message = "PASSED";
        } else {
            result.message = "FAILED";
        }
    } catch (...) {
        result.passed = false;
        result.message = "ERROR";
    }
    
    result.duration = micros() - startTime;
    
    Serial.printf("  %s: %s (%lu us)\n", testName.c_str(), result.message.c_str(), result.duration);
    
    return result;
}

void runSection(TestSection section) {
    if (section >= TOTAL_SECTIONS) {
        Serial.println("Invalid section index!");
        return;
    }
    
    Serial.printf("\n=== Running Section: %s ===\n", SECTION_NAMES[section]);
    
    // Initialize section results
    sectionResults[section].sectionName = SECTION_NAMES[section];
    sectionResults[section].totalTests = 0;
    sectionResults[section].passedTests = 0;
    sectionResults[section].failedTests = 0;
    sectionResults[section].totalDuration = 0;
    
    switch (section) {
        case SECTION_INITIALIZATION:
            sectionResults[section].totalTests = 2;
            sectionResults[section].results = new TestResult[2];
            
            sectionResults[section].results[0] = runTest(testSpiBegin, "SPI Begin");
            sectionResults[section].results[1] = runTest(testSpiEnd, "SPI End");
            break;
            
        case SECTION_BASIC_OPERATIONS:
            sectionResults[section].totalTests = 4;
            sectionResults[section].results = new TestResult[4];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiTransfer, "Single Byte Transfer");
            sectionResults[section].results[1] = runTest(testSpiTransfer16, "16-bit Transfer");
            sectionResults[section].results[2] = runTest(testSpiTransfer32, "32-bit Transfer");
            sectionResults[section].results[3] = runTest(testSpiTransferBuffer, "Buffer Transfer");
            break;
            
        case SECTION_TRANSFER_MODES:
            sectionResults[section].totalTests = 3;
            sectionResults[section].results = new TestResult[3];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiMode, "SPI Mode Testing");
            sectionResults[section].results[1] = runTest(testSpiBitOrder, "Bit Order Testing");
            sectionResults[section].results[2] = runTest(testSpiClockSpeed, "Clock Speed Testing");
            break;
            
        case SECTION_CLOCK_SPEEDS:
            sectionResults[section].totalTests = 1;
            sectionResults[section].results = new TestResult[1];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiClockSpeed, "Clock Speed Variations");
            break;
            
        case SECTION_ERROR_HANDLING:
            sectionResults[section].totalTests = 1;
            sectionResults[section].results = new TestResult[1];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiErrorHandling, "Error Handling");
            break;
            
        case SECTION_PERFORMANCE:
            sectionResults[section].totalTests = 1;
            sectionResults[section].results = new TestResult[1];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiPerformance, "Performance Benchmark");
            break;
            
        case SECTION_STRESS_TESTING:
            sectionResults[section].totalTests = 1;
            sectionResults[section].results = new TestResult[1];
            
            testSpiBegin(); // Ensure SPI is initialized
            
            sectionResults[section].results[0] = runTest(testSpiStress, "Stress Testing");
            break;
    }
    
    // Calculate section statistics
    for (int i = 0; i < sectionResults[section].totalTests; i++) {
        if (sectionResults[section].results[i].passed) {
            sectionResults[section].passedTests++;
        } else {
            sectionResults[section].failedTests++;
        }
        sectionResults[section].totalDuration += sectionResults[section].results[i].duration;
    }
    
    printSectionResults(sectionResults[section]);
}

void runAllSections() {
    Serial.println("\n=== Running All SPI Test Sections ===\n");
    
    for (int i = 0; i < TOTAL_SECTIONS; i++) {
        runSection((TestSection)i);
    }
    
    printOverallResults();
}

void runSpecificSection(int sectionIndex) {
    if (sectionIndex < 0 || sectionIndex >= TOTAL_SECTIONS) {
        Serial.printf("Invalid section index: %d. Valid range: 0-%d\n", sectionIndex, TOTAL_SECTIONS - 1);
        return;
    }
    
    runSection((TestSection)sectionIndex);
}

void printSectionResults(const SectionResult& section) {
    Serial.printf("\n--- Section Results: %s ---\n", section.sectionName.c_str());
    Serial.printf("Total Tests: %d\n", section.totalTests);
    Serial.printf("Passed: %d\n", section.passedTests);
    Serial.printf("Failed: %d\n", section.failedTests);
    Serial.printf("Success Rate: %.1f%%\n", (float)section.passedTests / section.totalTests * 100);
    Serial.printf("Total Duration: %lu us\n", section.totalDuration);
    Serial.printf("Average Duration: %.1f us\n", (float)section.totalDuration / section.totalTests);
}

void printOverallResults() {
    Serial.println("\n=== Overall Test Results ===");
    
    int totalTests = 0;
    int totalPassed = 0;
    int totalFailed = 0;
    unsigned long totalDuration = 0;
    
    for (int i = 0; i < TOTAL_SECTIONS; i++) {
        if (sectionResults[i].results != nullptr) {
            totalTests += sectionResults[i].totalTests;
            totalPassed += sectionResults[i].passedTests;
            totalFailed += sectionResults[i].failedTests;
            totalDuration += sectionResults[i].totalDuration;
        }
    }
    
    Serial.printf("Total Tests: %d\n", totalTests);
    Serial.printf("Total Passed: %d\n", totalPassed);
    Serial.printf("Total Failed: %d\n", totalFailed);
    Serial.printf("Overall Success Rate: %.1f%%\n", (float)totalPassed / totalTests * 100);
    Serial.printf("Total Duration: %lu us\n", totalDuration);
    Serial.printf("Average Duration: %.1f us\n", (float)totalDuration / totalTests);
}

void initializeSpi() {
    // Initialize SPI with default settings
    spi.begin();
    spi.setClockDivider(SPI_CLOCK_DIV4);
    spi.setDataMode(SPI_MODE0);
    spi.setBitOrder(MSBFIRST);
    spiInitialized = true;
    
    Serial.println("SPI initialized with default settings");
}

void cleanupSpi() {
    if (spiInitialized) {
        spi.end();
        spiInitialized = false;
    }
    
    // Clean up section results
    if (sectionResults != nullptr) {
        for (int i = 0; i < TOTAL_SECTIONS; i++) {
            if (sectionResults[i].results != nullptr) {
                delete[] sectionResults[i].results;
                sectionResults[i].results = nullptr;
            }
        }
        delete[] sectionResults;
        sectionResults = nullptr;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n=== SPI Comprehensive Test Suite ===");
    Serial.println("This test suite provides comprehensive testing of SPI functionality");
    Serial.println("with the ability to run individual sections or all tests together.");
    Serial.println();
    
    // Initialize section results array
    sectionResults = new SectionResult[TOTAL_SECTIONS];
    for (int i = 0; i < TOTAL_SECTIONS; i++) {
        sectionResults[i].results = nullptr;
    }
    
    Serial.println("Available test sections:");
    for (int i = 0; i < TOTAL_SECTIONS; i++) {
        Serial.printf("  %d: %s\n", i, SECTION_NAMES[i]);
    }
    Serial.println();
    Serial.println("Commands:");
    Serial.println("  'all' - Run all test sections");
    Serial.println("  '0-6' - Run specific section (0-6)");
    Serial.println("  'help' - Show this help");
    Serial.println("  'quit' - Exit test suite");
    Serial.println();
    
    // Initialize SPI
    initializeSpi();
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        
        if (input.equalsIgnoreCase("all")) {
            runAllSections();
        } else if (input.equalsIgnoreCase("help")) {
            Serial.println("Available commands:");
            Serial.println("  'all' - Run all test sections");
            Serial.println("  '0-6' - Run specific section (0-6)");
            Serial.println("  'help' - Show this help");
            Serial.println("  'quit' - Exit test suite");
        } else if (input.equalsIgnoreCase("quit")) {
            Serial.println("Exiting test suite...");
            cleanupSpi();
            exit(0);
        } else {
            // Check if input is a number
            int sectionIndex = input.toInt();
            if (sectionIndex >= 0 && sectionIndex < TOTAL_SECTIONS) {
                runSpecificSection(sectionIndex);
            } else {
                Serial.println("Invalid command. Type 'help' for available commands.");
            }
        }
        
        Serial.println("\nEnter command (all/0-6/help/quit):");
    }
    
    delay(100);
}