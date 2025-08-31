#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <cassert>
#include <cstring>

// Logger class for testing
class Logger {
private:
    std::string name;
    int logLevel;
    bool enabled;
    std::vector<std::string> logBuffer;
    
public:
    enum LogLevel {
        TRACE = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    
    Logger(const std::string& loggerName = "DefaultLogger") 
        : name(loggerName), logLevel(INFO), enabled(true) {}
    
    void setLogLevel(LogLevel level) { logLevel = level; }
    void setEnabled(bool state) { enabled = state; }
    
    void log(LogLevel level, const std::string& message) {
        if (!enabled || level < logLevel) return;
        
        std::string levelStr;
        switch (level) {
            case TRACE: levelStr = "TRACE"; break;
            case DEBUG: levelStr = "DEBUG"; break;
            case INFO: levelStr = "INFO"; break;
            case WARN: levelStr = "WARN"; break;
            case ERROR: levelStr = "ERROR"; break;
            case FATAL: levelStr = "FATAL"; break;
        }
        
        std::string logEntry = "[" + levelStr + "] " + name + ": " + message;
        logBuffer.push_back(logEntry);
        std::cout << logEntry << std::endl;
    }
    
    void trace(const std::string& message) { log(TRACE, message); }
    void debug(const std::string& message) { log(DEBUG, message); }
    void info(const std::string& message) { log(INFO, message); }
    void warn(const std::string& message) { log(WARN, message); }
    void error(const std::string& message) { log(ERROR, message); }
    void fatal(const std::string& message) { log(FATAL, message); }
    
    const std::vector<std::string>& getLogBuffer() const { return logBuffer; }
    void clearBuffer() { logBuffer.clear(); }
    size_t getBufferSize() const { return logBuffer.size(); }
    
    std::string getName() const { return name; }
    int getLogLevel() const { return logLevel; }
    bool isEnabled() const { return enabled; }
};

// Test section base class
class TestSection {
protected:
    std::string name;
    std::string description;
    bool passed;
    std::vector<std::string> errors;
    
public:
    TestSection(const std::string& sectionName, const std::string& sectionDesc)
        : name(sectionName), description(sectionDesc), passed(true) {}
    
    virtual ~TestSection() = default;
    
    virtual bool run() = 0;
    
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    bool hasPassed() const { return passed; }
    const std::vector<std::string>& getErrors() const { return errors; }
    
protected:
    void addError(const std::string& error) {
        errors.push_back(error);
        passed = false;
    }
    
    void assertTrue(bool condition, const std::string& message) {
        if (!condition) {
            addError("Assertion failed: " + message);
        }
    }
    
    void assertFalse(bool condition, const std::string& message) {
        assertTrue(!condition, message);
    }
    
    void assertEquals(const std::string& expected, const std::string& actual, const std::string& message) {
        if (expected != actual) {
            addError("Assertion failed: " + message + " (expected: " + expected + ", got: " + actual + ")");
        }
    }
    
    void assertEquals(int expected, int actual, const std::string& message) {
        if (expected != actual) {
            addError("Assertion failed: " + message + " (expected: " + std::to_string(expected) + ", got: " + std::to_string(actual) + ")");
        }
    }
    
    void assertEquals(size_t expected, size_t actual, const std::string& message) {
        if (expected != actual) {
            addError("Assertion failed: " + message + " (expected: " + std::to_string(expected) + ", got: " + std::to_string(actual) + ")");
        }
    }
};

// Test section implementations
class LoggerBasicFunctionalityTest : public TestSection {
public:
    LoggerBasicFunctionalityTest() : TestSection("Basic Functionality", "Tests basic logger creation and configuration") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        // Test 1: Basic logger creation
        Logger logger("TestLogger");
        assertTrue(logger.getName() == "TestLogger", "Logger name should match constructor parameter");
        assertTrue(logger.isEnabled(), "Logger should be enabled by default");
        assertTrue(logger.getLogLevel() == Logger::INFO, "Default log level should be INFO");
        
        // Test 2: Log level setting
        logger.setLogLevel(Logger::DEBUG);
        assertEquals(Logger::DEBUG, logger.getLogLevel(), "Log level should be settable");
        
        // Test 3: Enable/disable
        logger.setEnabled(false);
        assertFalse(logger.isEnabled(), "Logger should be disableable");
        logger.setEnabled(true);
        assertTrue(logger.isEnabled(), "Logger should be re-enableable");
        
        return passed;
    }
};

class LoggerLoggingTest : public TestSection {
public:
    LoggerLoggingTest() : TestSection("Logging Operations", "Tests various logging operations and levels") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        Logger logger("LoggingTest");
        logger.clearBuffer();
        
        // Test 1: All log levels (default level is INFO, so TRACE and DEBUG won't be logged)
        logger.trace("Trace message");
        logger.debug("Debug message");
        logger.info("Info message");
        logger.warn("Warning message");
        logger.error("Error message");
        logger.fatal("Fatal message");
        
        assertEquals(static_cast<size_t>(4), logger.getBufferSize(), "INFO and above log levels should be recorded (default level)");
        
        // Test 2: Log level filtering
        logger.clearBuffer();
        logger.setLogLevel(Logger::WARN);
        logger.trace("This should not appear");
        logger.debug("This should not appear");
        logger.info("This should not appear");
        logger.warn("This should appear");
        logger.error("This should appear");
        logger.fatal("This should appear");
        
        assertEquals(static_cast<size_t>(3), logger.getBufferSize(), "Only WARN and above should be logged");
        
        // Test 3: Buffer content verification
        const auto& buffer = logger.getLogBuffer();
        assertTrue(buffer.size() >= 3, "Buffer should contain at least 3 messages");
        if (buffer.size() >= 3) {
            assertTrue(buffer[0].find("WARN") != std::string::npos, "First message should be WARN level");
            assertTrue(buffer[1].find("ERROR") != std::string::npos, "Second message should be ERROR level");
            assertTrue(buffer[2].find("FATAL") != std::string::npos, "Third message should be FATAL level");
        }
        
        return passed;
    }
};

class LoggerBufferManagementTest : public TestSection {
public:
    LoggerBufferManagementTest() : TestSection("Buffer Management", "Tests log buffer operations and management") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        Logger logger("BufferTest");
        
        // Test 1: Initial buffer state
        assertEquals(static_cast<size_t>(0), logger.getBufferSize(), "Initial buffer should be empty");
        
        // Test 2: Buffer population
        logger.info("Message 1");
        logger.info("Message 2");
        logger.info("Message 3");
        assertEquals(static_cast<size_t>(3), logger.getBufferSize(), "Buffer should contain 3 messages");
        
        // Test 3: Buffer clearing
        logger.clearBuffer();
        assertEquals(static_cast<size_t>(0), logger.getBufferSize(), "Buffer should be empty after clearing");
        
        // Test 4: Buffer content preservation
        logger.info("New message");
        assertEquals(static_cast<size_t>(1), logger.getBufferSize(), "Buffer should contain new message");
        const auto& buffer = logger.getLogBuffer();
        assertTrue(buffer[0].find("New message") != std::string::npos, "Buffer should contain correct message");
        
        return passed;
    }
};

class LoggerPerformanceTest : public TestSection {
public:
    LoggerPerformanceTest() : TestSection("Performance", "Tests logger performance under load") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        Logger logger("PerformanceTest");
        logger.clearBuffer();
        
        // Test 1: High-volume logging
        const int messageCount = 1000;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < messageCount; ++i) {
            logger.info("Performance test message " + std::to_string(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        assertEquals(static_cast<size_t>(messageCount), logger.getBufferSize(), "All messages should be logged");
        assertTrue(duration.count() < 1000, "Logging 1000 messages should take less than 1 second");
        
        std::cout << "Performance: " << messageCount << " messages in " << duration.count() << "ms" << std::endl;
        
        return passed;
    }
};

class LoggerEdgeCaseTest : public TestSection {
public:
    LoggerEdgeCaseTest() : TestSection("Edge Cases", "Tests logger behavior with edge cases") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        // Test 1: Empty logger name
        Logger emptyLogger("");
        assertTrue(emptyLogger.getName() == "", "Empty logger name should be allowed");
        
        // Test 2: Very long logger name
        std::string longName(1000, 'A');
        Logger longNameLogger(longName);
        assertEquals(longName, longNameLogger.getName(), "Very long logger name should be supported");
        
        // Test 3: Empty messages
        Logger messageLogger("MessageTest");
        messageLogger.clearBuffer();
        messageLogger.info("");
        assertEquals(static_cast<size_t>(1), messageLogger.getBufferSize(), "Empty message should be logged");
        
        // Test 4: Special characters in messages
        messageLogger.clearBuffer();
        std::string specialChars = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
        messageLogger.info(specialChars);
        assertEquals(static_cast<size_t>(1), messageLogger.getBufferSize(), "Special characters should be handled");
        
        // Test 5: Unicode characters
        messageLogger.clearBuffer();
        std::string unicodeMsg = "Unicode: 你好世界 🌍";
        messageLogger.info(unicodeMsg);
        assertEquals(static_cast<size_t>(1), messageLogger.getBufferSize(), "Unicode characters should be handled");
        
        return passed;
    }
};

// Test runner class
class LoggerTestRunner {
private:
    std::vector<std::unique_ptr<TestSection>> testSections;
    std::map<std::string, bool> sectionResults;
    
public:
    LoggerTestRunner() {
        // Initialize all test sections
        testSections.push_back(std::make_unique<LoggerBasicFunctionalityTest>());
        testSections.push_back(std::make_unique<LoggerLoggingTest>());
        testSections.push_back(std::make_unique<LoggerBufferManagementTest>());
        testSections.push_back(std::make_unique<LoggerPerformanceTest>());
        testSections.push_back(std::make_unique<LoggerEdgeCaseTest>());
    }
    
    void listSections() {
        std::cout << "\n=== Available Test Sections ===" << std::endl;
        for (size_t i = 0; i < testSections.size(); ++i) {
            std::cout << i + 1 << ". " << testSections[i]->getName() << " - " 
                      << testSections[i]->getDescription() << std::endl;
        }
    }
    
    bool runSection(const std::string& sectionName) {
        for (auto& section : testSections) {
            if (section->getName() == sectionName) {
                bool result = section->run();
                sectionResults[sectionName] = result;
                return result;
            }
        }
        std::cout << "Section '" << sectionName << "' not found!" << std::endl;
        return false;
    }
    
    bool runSection(int sectionIndex) {
        if (sectionIndex < 1 || sectionIndex > static_cast<int>(testSections.size())) {
            std::cout << "Invalid section index: " << sectionIndex << std::endl;
            return false;
        }
        
        auto& section = testSections[sectionIndex - 1];
        bool result = section->run();
        sectionResults[section->getName()] = result;
        return result;
    }
    
    bool runAllSections() {
        std::cout << "\n=== Running All Test Sections ===" << std::endl;
        bool allPassed = true;
        
        for (auto& section : testSections) {
            bool result = section->run();
            sectionResults[section->getName()] = result;
            if (!result) allPassed = false;
        }
        
        return allPassed;
    }
    
    void printResults() {
        std::cout << "\n=== Test Results Summary ===" << std::endl;
        int totalSections = 0;
        int passedSections = 0;
        
        for (const auto& result : sectionResults) {
            totalSections++;
            if (result.second) passedSections++;
            
            std::cout << (result.second ? "✅" : "❌") << " " << result.first << std::endl;
        }
        
        std::cout << "\nOverall Result: " << passedSections << "/" << totalSections << " sections passed" << std::endl;
        
        if (passedSections == totalSections) {
            std::cout << "🎉 All tests passed!" << std::endl;
        } else {
            std::cout << "⚠️  Some tests failed. Check individual section results above." << std::endl;
        }
    }
    
    void printSectionDetails(const std::string& sectionName) {
        for (auto& section : testSections) {
            if (section->getName() == sectionName) {
                std::cout << "\n=== Section Details: " << section->getName() << " ===" << std::endl;
                std::cout << "Description: " << section->getDescription() << std::endl;
                std::cout << "Status: " << (sectionResults[sectionName] ? "PASSED" : "FAILED") << std::endl;
                
                if (!sectionResults[sectionName]) {
                    std::cout << "Errors:" << std::endl;
                    for (const auto& error : section->getErrors()) {
                        std::cout << "  - " << error << std::endl;
                    }
                }
                return;
            }
        }
        std::cout << "Section '" << sectionName << "' not found!" << std::endl;
    }
};

// Main function
int main(int argc, char* argv[]) {
    std::cout << "=== Logger Comprehensive Test Suite ===" << std::endl;
    std::cout << "This test suite allows you to run individual sections or all tests at once." << std::endl;
    
    LoggerTestRunner runner;
    
    if (argc > 1) {
        std::string arg = argv[1];
        
        if (arg == "--list" || arg == "-l") {
            runner.listSections();
            return 0;
        } else if (arg == "--all" || arg == "-a") {
            bool allPassed = runner.runAllSections();
            runner.printResults();
            return allPassed ? 0 : 1;
        } else if (arg == "--section" || arg == "-s") {
            if (argc > 2) {
                std::string sectionName = argv[2];
                bool passed = runner.runSection(sectionName);
                runner.printSectionDetails(sectionName);
                return passed ? 0 : 1;
            } else {
                std::cout << "Usage: " << argv[0] << " --section <section_name>" << std::endl;
                runner.listSections();
                return 1;
            }
        } else if (arg == "--index" || arg == "-i") {
            if (argc > 2) {
                int sectionIndex = std::stoi(argv[2]);
                bool passed = runner.runSection(sectionIndex);
                runner.printResults();
                return passed ? 0 : 1;
            } else {
                std::cout << "Usage: " << argv[0] << " --index <section_number>" << std::endl;
                runner.listSections();
                return 1;
            }
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage:" << std::endl;
            std::cout << "  " << argv[0] << " --list, -l                    List all test sections" << std::endl;
            std::cout << "  " << argv[0] << " --all, -a                     Run all test sections" << std::endl;
            std::cout << "  " << argv[0] << " --section <name>, -s <name>   Run specific section by name" << std::endl;
            std::cout << "  " << argv[0] << " --index <number>, -i <number> Run specific section by index" << std::endl;
            std::cout << "  " << argv[0] << " --help, -h                    Show this help message" << std::endl;
            std::cout << std::endl;
            std::cout << "Examples:" << std::endl;
            std::cout << "  " << argv[0] << " --section \"Basic Functionality\"" << std::endl;
            std::cout << "  " << argv[0] << " --index 1" << std::endl;
            std::cout << "  " << argv[0] << " --all" << std::endl;
            return 0;
        } else {
            std::cout << "Unknown argument: " << arg << std::endl;
            std::cout << "Use --help for usage information." << std::endl;
            return 1;
        }
    } else {
        // Interactive mode
        std::cout << "\nNo arguments provided. Running in interactive mode." << std::endl;
        std::cout << "Use --help to see command line options." << std::endl;
        
        runner.listSections();
        std::cout << "\nRunning all sections by default..." << std::endl;
        
        bool allPassed = runner.runAllSections();
        runner.printResults();
        return allPassed ? 0 : 1;
    }
}