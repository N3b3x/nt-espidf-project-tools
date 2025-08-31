#!/bin/bash

# SPI Comprehensive Test Build Script
# This script helps compile and upload the SPI comprehensive test to an ESP32 device

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SKETCH_NAME="SpiComprehensiveTest"
SKETCH_PATH="main/${SKETCH_NAME}.cpp"
BOARD_FQBN="esp32:esp32:esp32"
SERIAL_BAUD="115200"

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if arduino-cli is available
check_arduino_cli() {
    if ! command -v arduino-cli &> /dev/null; then
        print_error "arduino-cli is not installed or not in PATH"
        print_status "Please install arduino-cli first:"
        print_status "  https://arduino.github.io/arduino-cli/latest/installation/"
        exit 1
    fi
    
    print_success "arduino-cli found: $(arduino-cli version)"
}

# Function to check if sketch file exists
check_sketch() {
    if [ ! -f "$SKETCH_PATH" ]; then
        print_error "Sketch file not found: $SKETCH_PATH"
        exit 1
    fi
    
    print_success "Sketch file found: $SKETCH_PATH"
}

# Function to detect available ports
detect_ports() {
    print_status "Detecting available ports..."
    
    if command -v arduino-cli &> /dev/null; then
        arduino-cli board list
    else
        print_warning "arduino-cli not available, cannot list boards"
        print_status "Available ports (manual detection):"
        ls /dev/tty* 2>/dev/null | grep -E "(USB|ACM|S)" || print_warning "No serial ports found"
    fi
}

# Function to compile the sketch
compile_sketch() {
    print_status "Compiling sketch: $SKETCH_NAME"
    
    if arduino-cli compile --fqbn "$BOARD_FQBN" "$SKETCH_PATH"; then
        print_success "Compilation successful!"
    else
        print_error "Compilation failed!"
        exit 1
    fi
}

# Function to upload the sketch
upload_sketch() {
    local port="$1"
    
    if [ -z "$port" ]; then
        print_error "No port specified for upload"
        print_status "Usage: $0 upload <port>"
        print_status "Example: $0 upload /dev/ttyUSB0"
        exit 1
    fi
    
    if [ ! -e "$port" ]; then
        print_error "Port $port does not exist"
        detect_ports
        exit 1
    fi
    
    print_status "Uploading sketch to $port..."
    
    if arduino-cli upload --fqbn "$BOARD_FQBN" -p "$port" "$SKETCH_PATH"; then
        print_success "Upload successful!"
    else
        print_error "Upload failed!"
        exit 1
    fi
}

# Function to monitor serial output
monitor_serial() {
    local port="$1"
    
    if [ -z "$port" ]; then
        print_error "No port specified for monitoring"
        print_status "Usage: $0 monitor <port>"
        print_status "Example: $0 monitor /dev/ttyUSB0"
        exit 1
    fi
    
    if [ ! -e "$port" ]; then
        print_error "Port $port does not exist"
        detect_ports
        exit 1
    fi
    
    print_status "Starting serial monitor on $port (baud: $SERIAL_BAUD)"
    print_status "Press Ctrl+C to stop monitoring"
    
    if command -v arduino-cli &> /dev/null; then
        arduino-cli monitor -p "$port" -c baudrate="$SERIAL_BAUD"
    else
        print_warning "arduino-cli not available, using alternative monitor"
        if command -v screen &> /dev/null; then
            screen "$port" "$SERIAL_BAUD"
        elif command -v minicom &> /dev/null; then
            minicom -D "$port" -b "$SERIAL_BAUD"
        else
            print_error "No serial monitor available. Please install screen, minicom, or arduino-cli"
            exit 1
        fi
    fi
}

# Function to run all steps
run_all() {
    local port="$1"
    
    if [ -z "$port" ]; then
        print_error "No port specified"
        print_status "Usage: $0 all <port>"
        print_status "Example: $0 all /dev/ttyUSB0"
        exit 1
    fi
    
    print_status "Running complete build and upload process..."
    
    check_arduino_cli
    check_sketch
    compile_sketch
    upload_sketch "$port"
    
    print_success "Build and upload complete!"
    print_status "Starting serial monitor in 3 seconds..."
    sleep 3
    monitor_serial "$port"
}

# Function to show help
show_help() {
    echo "SPI Comprehensive Test Build Script"
    echo ""
    echo "Usage: $0 <command> [options]"
    echo ""
    echo "Commands:"
    echo "  compile              Compile the SPI test sketch"
    echo "  upload <port>        Upload the compiled sketch to specified port"
    echo "  monitor <port>       Monitor serial output from specified port"
    echo "  all <port>           Run complete build, upload, and monitor process"
    echo "  ports                List available ports"
    echo "  help                 Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 compile"
    echo "  $0 upload /dev/ttyUSB0"
    echo "  $0 monitor /dev/ttyUSB0"
    echo "  $0 all /dev/ttyUSB0"
    echo ""
    echo "Port examples:"
    echo "  Linux:   /dev/ttyUSB0, /dev/ttyACM0"
    echo "  macOS:   /dev/tty.usbserial-*, /dev/tty.usbmodem*"
    echo "  Windows: COM1, COM2, etc."
}

# Main script logic
case "${1:-help}" in
    "compile")
        check_arduino_cli
        check_sketch
        compile_sketch
        ;;
    "upload")
        check_arduino_cli
        check_sketch
        upload_sketch "$2"
        ;;
    "monitor")
        monitor_serial "$2"
        ;;
    "all")
        run_all "$2"
        ;;
    "ports")
        detect_ports
        ;;
    "help"|*)
        show_help
        ;;
esac