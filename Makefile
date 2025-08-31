# Makefile for Logger and ASCII Comprehensive Tests
# This Makefile provides targets to compile and run the test suites

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGETS = LoggerComprehensiveTest AsciiComprehensiveTest

# Default target
all: $(TARGETS)

# Logger test compilation
LoggerComprehensiveTest: LoggerComprehensiveTest.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# ASCII test compilation
AsciiComprehensiveTest: AsciiArtComprehensiveTest.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Test targets
test-logger: LoggerComprehensiveTest
	@echo "=== Testing Logger Comprehensive Test ==="
	./LoggerComprehensiveTest --all

test-ascii: AsciiComprehensiveTest
	@echo "=== Testing ASCII Art Comprehensive Test ==="
	./AsciiComprehensiveTest --all

test-all: test-logger test-ascii

# List sections for each test
list-logger: LoggerComprehensiveTest
	@echo "=== Logger Test Sections ==="
	./LoggerComprehensiveTest --list

list-ascii: AsciiComprehensiveTest
	@echo "=== ASCII Test Sections ==="
	./AsciiComprehensiveTest --list

# Run specific sections
# Usage: make run-logger-section SECTION="Basic Functionality"
run-logger-section: LoggerComprehensiveTest
	@if [ -z "$(SECTION)" ]; then \
		echo "Usage: make run-logger-section SECTION=\"<section_name>\""; \
		./LoggerComprehensiveTest --list; \
	else \
		./LoggerComprehensiveTest --section "$(SECTION)"; \
	fi

# Usage: make run-ascii-section SECTION="Basic Functionality"
run-ascii-section: AsciiComprehensiveTest
	@if [ -z "$(SECTION)" ]; then \
		echo "Usage: make run-ascii-section SECTION=\"<section_name>\""; \
		./AsciiComprehensiveTest --list; \
	else \
		./AsciiComprehensiveTest --section "$(SECTION)"; \
	fi

# Run by index
# Usage: make run-logger-index INDEX=1
run-logger-index: LoggerComprehensiveTest
	@if [ -z "$(INDEX)" ]; then \
		echo "Usage: make run-logger-index INDEX=<number>"; \
		./LoggerComprehensiveTest --list; \
	else \
		./LoggerComprehensiveTest --index $(INDEX); \
	fi

# Usage: make run-ascii-index INDEX=1
run-ascii-index: AsciiComprehensiveTest
	@if [ -z "$(INDEX)" ]; then \
		echo "Usage: make run-ascii-index INDEX=<number>"; \
		./AsciiComprehensiveTest --list; \
	else \
		./AsciiComprehensiveTest --index $(INDEX); \
	fi

# Clean build artifacts
clean:
	rm -f $(TARGETS) *.o

# Help target
help:
	@echo "Available targets:"
	@echo "  all                    - Build all test executables"
	@echo "  LoggerComprehensiveTest - Build logger test executable"
	@echo "  AsciiComprehensiveTest  - Build ASCII test executable"
	@echo "  test-logger            - Run all logger tests"
	@echo "  test-ascii             - Run all ASCII tests"
	@echo "  test-all               - Run all tests"
	@echo "  list-logger            - List logger test sections"
	@echo "  list-ascii             - List ASCII test sections"
	@echo "  run-logger-section SECTION=\"<name>\"" - Run specific logger section"
	@echo "  run-ascii-section SECTION=\"<name>\"" - Run specific ASCII section"
	@echo "  run-logger-index INDEX=<number>" - Run logger section by index"
	@echo "  run-ascii-index INDEX=<number>" - Run ASCII section by index"
	@echo "  clean                  - Remove build artifacts"
	@echo "  help                   - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make run-logger-section SECTION=\"Basic Functionality\""
	@echo "  make run-ascii-section SECTION=\"Pixel Operations\""
	@echo "  make run-logger-index INDEX=1"
	@echo "  make run-ascii-index INDEX=2"

.PHONY: all test-logger test-ascii test-all list-logger list-ascii \
        run-logger-section run-ascii-section run-logger-index run-ascii-index \
        clean help