#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iomanip>

// ASCII Art Generator class for testing
class AsciiArtGenerator {
private:
    std::string canvas;
    int width;
    int height;
    char defaultChar;
    
public:
    AsciiArtGenerator(int w = 80, int h = 24, char defaultCh = ' ') 
        : width(w), height(h), defaultChar(defaultCh) {
        canvas = std::string(width * height, defaultChar);
    }
    
    void setPixel(int x, int y, char ch) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            canvas[y * width + x] = ch;
        }
    }
    
    char getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return canvas[y * width + x];
        }
        return '\0';
    }
    
    void clear() {
        canvas = std::string(width * height, defaultChar);
    }
    
    void fill(char ch) {
        canvas = std::string(width * height, ch);
    }
    
    void drawLine(int x1, int y1, int x2, int y2, char ch) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            setPixel(x1, y1, ch);
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
    
    void drawRectangle(int x1, int y1, int x2, int y2, char ch, bool filled = false) {
        if (filled) {
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    setPixel(x, y, ch);
                }
            }
        } else {
            drawLine(x1, y1, x2, y1, ch);
            drawLine(x2, y1, x2, y2, ch);
            drawLine(x2, y2, x1, y2, ch);
            drawLine(x1, y2, x1, y1, ch);
        }
    }
    
    void drawCircle(int centerX, int centerY, int radius, char ch, bool filled = false) {
        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                int distanceSquared = x * x + y * y;
                if (distanceSquared <= radius * radius) {
                    if (!filled) {
                        // For outline, only draw pixels near the circumference
                        if (distanceSquared >= (radius - 1) * (radius - 1)) {
                            setPixel(centerX + x, centerY + y, ch);
                        }
                    } else {
                        // For filled circle, draw all pixels inside
                        setPixel(centerX + x, centerY + y, ch);
                    }
                }
            }
        }
    }
    
    void drawText(int x, int y, const std::string& text, char ch = '#') {
        for (size_t i = 0; i < text.length(); ++i) {
            setPixel(x + i, y, ch);
        }
    }
    
    std::string render() const {
        std::string result;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                result += canvas[y * width + x];
            }
            if (y < height - 1) result += '\n';
        }
        return result;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    char getDefaultChar() const { return defaultChar; }
    
    void resize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
        canvas = std::string(width * height, defaultChar);
    }
    
    std::string getCanvas() const { return canvas; }
    size_t getCanvasSize() const { return canvas.size(); }
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
    
    void assertEquals(char expected, char actual, const std::string& message) {
        if (expected != actual) {
            addError("Assertion failed: " + message + " (expected: '" + expected + "', got: '" + actual + "')");
        }
    }
};

// Test section implementations
class AsciiBasicFunctionalityTest : public TestSection {
public:
    AsciiBasicFunctionalityTest() : TestSection("Basic Functionality", "Tests basic ASCII art generator creation and configuration") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        // Test 1: Basic generator creation
        AsciiArtGenerator generator(80, 24, ' ');
        assertEquals(80, generator.getWidth(), "Generator width should match constructor parameter");
        assertEquals(24, generator.getHeight(), "Generator height should match constructor parameter");
        assertEquals(' ', generator.getDefaultChar(), "Default character should match constructor parameter");
        assertEquals(static_cast<size_t>(1920), generator.getCanvasSize(), "Canvas size should be width * height");
        
        // Test 2: Default constructor
        AsciiArtGenerator defaultGen;
        assertEquals(80, defaultGen.getWidth(), "Default width should be 80");
        assertEquals(24, defaultGen.getHeight(), "Default height should be 24");
        assertEquals(' ', defaultGen.getDefaultChar(), "Default character should be space");
        
        // Test 3: Resize functionality
        generator.resize(40, 12);
        assertEquals(40, generator.getWidth(), "Width should be resizable");
        assertEquals(12, generator.getHeight(), "Height should be resizable");
        assertEquals(static_cast<size_t>(480), generator.getCanvasSize(), "Canvas size should update after resize");
        
        return passed;
    }
};

class AsciiPixelOperationsTest : public TestSection {
public:
    AsciiPixelOperationsTest() : TestSection("Pixel Operations", "Tests pixel setting, getting, and manipulation") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        AsciiArtGenerator generator(10, 10, '.');
        
        // Test 1: Set and get pixel
        generator.setPixel(5, 5, 'X');
        assertEquals('X', generator.getPixel(5, 5), "Pixel should be settable and retrievable");
        
        // Test 2: Out of bounds handling
        assertEquals('\0', generator.getPixel(15, 15), "Out of bounds get should return null char");
        generator.setPixel(15, 15, 'Y'); // Should not crash
        assertEquals('\0', generator.getPixel(15, 15), "Out of bounds set should not affect canvas");
        
        // Test 3: Multiple pixel operations
        generator.setPixel(0, 0, 'A');
        generator.setPixel(9, 9, 'Z');
        assertEquals('A', generator.getPixel(0, 0), "First pixel should be set correctly");
        assertEquals('Z', generator.getPixel(9, 9), "Last pixel should be set correctly");
        
        // Test 4: Default character preservation
        assertEquals('.', generator.getPixel(1, 1), "Unset pixels should retain default character");
        
        return passed;
    }
};

class AsciiDrawingOperationsTest : public TestSection {
public:
    AsciiDrawingOperationsTest() : TestSection("Drawing Operations", "Tests line, rectangle, and circle drawing") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        AsciiArtGenerator generator(20, 20, '.');
        
        // Test 1: Horizontal line
        generator.drawLine(0, 10, 19, 10, '-');
        assertEquals('-', generator.getPixel(0, 10), "Horizontal line start should be drawn");
        assertEquals('-', generator.getPixel(19, 10), "Horizontal line end should be drawn");
        assertEquals('-', generator.getPixel(10, 10), "Horizontal line middle should be drawn");
        
        // Test 2: Vertical line
        generator.drawLine(10, 0, 10, 19, '|');
        assertEquals('|', generator.getPixel(10, 0), "Vertical line start should be drawn");
        assertEquals('|', generator.getPixel(10, 19), "Vertical line end should be drawn");
        assertEquals('|', generator.getPixel(10, 10), "Vertical line middle should be drawn");
        
        // Test 3: Rectangle outline
        generator.drawRectangle(2, 2, 17, 17, '+', false);
        assertEquals('+', generator.getPixel(2, 2), "Rectangle top-left corner should be drawn");
        assertEquals('+', generator.getPixel(17, 2), "Rectangle top-right corner should be drawn");
        assertEquals('+', generator.getPixel(2, 17), "Rectangle bottom-left corner should be drawn");
        assertEquals('+', generator.getPixel(17, 17), "Rectangle bottom-right corner should be drawn");
        
        // Test 4: Filled rectangle
        generator.drawRectangle(5, 5, 14, 14, '#', true);
        assertEquals('#', generator.getPixel(5, 5), "Filled rectangle should contain fill character");
        assertEquals('#', generator.getPixel(10, 10), "Filled rectangle center should contain fill character");
        
        // Test 5: Circle (clear canvas first to avoid overlap with previous drawings)
        generator.clear();
        generator.drawCircle(10, 10, 5, 'O', false);
        assertEquals('O', generator.getPixel(10, 5), "Circle top should be drawn");
        assertEquals('O', generator.getPixel(10, 15), "Circle bottom should be drawn");
        assertEquals('O', generator.getPixel(5, 10), "Circle left should be drawn");
        assertEquals('O', generator.getPixel(15, 10), "Circle right should be drawn");
        
        return passed;
    }
};

class AsciiTextRenderingTest : public TestSection {
public:
    AsciiTextRenderingTest() : TestSection("Text Rendering", "Tests text drawing and rendering capabilities") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        AsciiArtGenerator generator(30, 10, ' ');
        
        // Test 1: Basic text rendering
        std::string testText = "Hello";
        generator.drawText(5, 5, testText, 'H');
        assertEquals('H', generator.getPixel(5, 5), "First character should be drawn");
        assertEquals('H', generator.getPixel(9, 5), "Last character should be drawn");
        
        // Test 2: Text with different character
        generator.drawText(10, 7, "World", '*');
        assertEquals('*', generator.getPixel(10, 7), "Text with custom character should be drawn");
        assertEquals('*', generator.getPixel(14, 7), "Last character of custom text should be drawn");
        
        // Test 3: Empty text
        generator.drawText(0, 0, "", 'X');
        assertEquals(' ', generator.getPixel(0, 0), "Empty text should not modify canvas");
        
        // Test 4: Text at boundaries
        generator.drawText(0, 0, "Edge", 'E');
        assertEquals('E', generator.getPixel(0, 0), "Text at origin should be drawn");
        assertEquals('E', generator.getPixel(3, 0), "Text at boundary should be drawn");
        
        // Test 5: Text rendering output
        std::string rendered = generator.render();
        assertTrue(rendered.find("Hello") != std::string::npos || rendered.find("H") != std::string::npos, 
                  "Rendered output should contain drawn text");
        
        return passed;
    }
};

class AsciiCanvasManagementTest : public TestSection {
public:
    AsciiCanvasManagementTest() : TestSection("Canvas Management", "Tests canvas clearing, filling, and state management") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        AsciiArtGenerator generator(10, 10, '.');
        
        // Test 1: Initial canvas state
        assertEquals(static_cast<size_t>(100), generator.getCanvasSize(), "Initial canvas should have correct size");
        assertEquals('.', generator.getPixel(5, 5), "Initial canvas should contain default character");
        
        // Test 2: Fill operation
        generator.fill('X');
        assertEquals('X', generator.getPixel(0, 0), "Fill should set all pixels to specified character");
        assertEquals('X', generator.getPixel(5, 5), "Fill should set middle pixels to specified character");
        assertEquals('X', generator.getPixel(9, 9), "Fill should set last pixels to specified character");
        
        // Test 3: Clear operation
        generator.clear();
        assertEquals('.', generator.getPixel(0, 0), "Clear should restore default character");
        assertEquals('.', generator.getPixel(5, 5), "Clear should restore default character in middle");
        assertEquals('.', generator.getPixel(9, 9), "Clear should restore default character in last position");
        
        // Test 4: Partial modification and clear
        generator.setPixel(5, 5, 'Z');
        assertEquals('Z', generator.getPixel(5, 5), "Pixel should be modified after clear");
        generator.clear();
        assertEquals('.', generator.getPixel(5, 5), "Clear should reset all pixels including modified ones");
        
        // Test 5: Canvas content verification
        std::string canvas = generator.getCanvas();
        assertEquals(static_cast<size_t>(100), canvas.length(), "Canvas string should match canvas size");
        assertTrue(std::all_of(canvas.begin(), canvas.end(), [](char c) { return c == '.'; }), 
                  "Canvas should contain only default characters after clear");
        
        return passed;
    }
};

class AsciiPerformanceTest : public TestSection {
public:
    AsciiPerformanceTest() : TestSection("Performance", "Tests ASCII art generator performance under load") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        AsciiArtGenerator generator(100, 50, ' ');
        
        // Test 1: High-volume pixel operations
        const int pixelCount = 5000; // 100x50 canvas = 5000 pixels
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < pixelCount; ++i) {
            int x = i % 100;
            int y = i / 100;
            generator.setPixel(x, y, 'X');
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        assertEquals(static_cast<size_t>(pixelCount), generator.getCanvasSize(), "All pixels should be accessible");
        assertTrue(duration.count() < 100, "Setting 5000 pixels should take less than 100ms");
        
        std::cout << "Performance: " << pixelCount << " pixels set in " << duration.count() << "ms" << std::endl;
        
        // Test 2: Drawing operations performance
        generator.clear();
        start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 100; ++i) {
            generator.drawLine(0, i, 99, i, '-');
        }
        
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        assertTrue(duration.count() < 50, "Drawing 100 lines should take less than 50ms");
        
        std::cout << "Performance: 100 lines drawn in " << duration.count() << "ms" << std::endl;
        
        return passed;
    }
};

class AsciiEdgeCaseTest : public TestSection {
public:
    AsciiEdgeCaseTest() : TestSection("Edge Cases", "Tests ASCII art generator behavior with edge cases") {}
    
    bool run() override {
        std::cout << "\n=== Running " << name << " ===" << std::endl;
        std::cout << description << std::endl;
        
        // Test 1: Zero dimensions
        AsciiArtGenerator zeroGen(0, 0, 'X');
        assertEquals(0, zeroGen.getWidth(), "Zero width should be allowed");
        assertEquals(0, zeroGen.getHeight(), "Zero height should be allowed");
        assertEquals(static_cast<size_t>(0), zeroGen.getCanvasSize(), "Zero dimensions should result in empty canvas");
        
        // Test 2: Very large dimensions
        AsciiArtGenerator largeGen(1000, 1000, 'L');
        assertEquals(1000, largeGen.getWidth(), "Large width should be supported");
        assertEquals(1000, largeGen.getHeight(), "Large height should be supported");
        assertEquals(static_cast<size_t>(1000000), largeGen.getCanvasSize(), "Large canvas should have correct size");
        
        // Test 3: Special characters
        AsciiArtGenerator specialGen(10, 10, '\0');
        assertEquals('\0', specialGen.getDefaultChar(), "Null character should be supported as default");
        specialGen.setPixel(5, 5, '\t');
        assertEquals('\t', specialGen.getPixel(5, 5), "Tab character should be supported");
        
        // Test 4: Negative coordinates
        specialGen.setPixel(-1, -1, 'N');
        assertEquals('\0', specialGen.getPixel(-1, -1), "Negative coordinates should not affect canvas");
        
        // Test 5: Boundary conditions
        AsciiArtGenerator boundaryGen(1, 1, 'B');
        assertEquals(static_cast<size_t>(1), boundaryGen.getCanvasSize(), "1x1 canvas should have size 1");
        boundaryGen.setPixel(0, 0, 'X');
        assertEquals('X', boundaryGen.getPixel(0, 0), "Single pixel canvas should work correctly");
        
        return passed;
    }
};

// Test runner class
class AsciiTestRunner {
private:
    std::vector<std::unique_ptr<TestSection>> testSections;
    std::map<std::string, bool> sectionResults;
    
public:
    AsciiTestRunner() {
        // Initialize all test sections
        testSections.push_back(std::make_unique<AsciiBasicFunctionalityTest>());
        testSections.push_back(std::make_unique<AsciiPixelOperationsTest>());
        testSections.push_back(std::make_unique<AsciiDrawingOperationsTest>());
        testSections.push_back(std::make_unique<AsciiTextRenderingTest>());
        testSections.push_back(std::make_unique<AsciiCanvasManagementTest>());
        testSections.push_back(std::make_unique<AsciiPerformanceTest>());
        testSections.push_back(std::make_unique<AsciiEdgeCaseTest>());
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
    std::cout << "=== ASCII Art Comprehensive Test Suite ===" << std::endl;
    std::cout << "This test suite allows you to run individual sections or all tests at once." << std::endl;
    
    AsciiTestRunner runner;
    
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