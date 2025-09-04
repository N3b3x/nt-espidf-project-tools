#!/bin/bash
# ESP32 Project Setup Script
# This script creates a new ESP32 project with proper structure, configuration, and build system

set -e  # Exit on any error

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source the common setup functions
source "$SCRIPT_DIR/setup_common.sh"

# Default values
DEFAULT_APP_TYPE="ascii_art"
DEFAULT_TARGET="esp32c6"
DEFAULT_IDF_VERSION="release/v5.5"

# Show help if requested
if [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    echo "ESP32 Project Setup Script"
    echo ""
    echo "Usage: ./setup_project.sh [PROJECT_PATH] [OPTIONS]"
    echo ""
    echo "ARGUMENTS:"
    echo "  PROJECT_PATH                - Path where the new project should be created"
    echo "                               If path doesn't exist, it will be created"
    echo "                               If not provided, uses current directory"
    echo ""
    echo "OPTIONS:"
    echo "  --app-type <type>           - Initial app type (default: $DEFAULT_APP_TYPE)"
    echo "  --target <target>           - ESP32 target (default: $DEFAULT_TARGET)"
    echo "  --idf-version <version>     - ESP-IDF version (default: $DEFAULT_IDF_VERSION)"
    echo "  --force                     - Force overwrite existing project"
    echo "  --help, -h                  - Show this help message"
    echo "  --verbose                   - Show detailed output"
    echo ""
    echo "EXAMPLES:"
    echo "  # Create project in current directory"
    echo "  ./setup_project.sh"
    echo ""
    echo "  # Create project in specific directory"
    echo "  ./setup_project.sh /path/to/my_project"
    echo ""
    echo "  # Create project with custom settings"
    echo "  ./setup_project.sh /path/to/my_project --app-type gpio_test --target esp32"
    echo ""
    echo "  # Force overwrite existing project"
    echo "  ./setup_project.sh /path/to/my_project --force"
    echo ""
    echo "WHAT IT CREATES:"
    echo "  • Project directory structure"
    echo "  • app_config.yml with example configuration"
    echo "  • CMakeLists.txt with flexible build system"
    echo "  • main/CMakeLists.txt for component configuration"
    echo "  • scripts/ directory with utility scripts"
    echo "  • Basic source file for the specified app type"
    echo "  • ESP-IDF project initialization"
    echo "  • SDK configuration (sdkconfig)"
    echo ""
    echo "PROJECT STRUCTURE:"
    echo "  project_name/"
    echo "  ├── app_config.yml          # App configuration and metadata"
    echo "  ├── CMakeLists.txt          # Main project CMake configuration"
    echo "  ├── sdkconfig               # ESP-IDF SDK configuration"
    echo "  ├── main/"
    echo "  │   ├── CMakeLists.txt      # Main component CMake configuration"
    echo "  │   └── *.cpp               # Source files for different app types"
    echo "  └── scripts/"
    echo "      └── get_app_info.py     # App information utility"
    echo ""
    echo "REQUIREMENTS:"
    echo "  • ESP-IDF must be installed and available"
    echo "  • Python 3.6+ with PyYAML"
    echo "  • Git (for ESP-IDF project initialization)"
    echo "  • CMake 3.16+"
    echo ""
    echo "POST-SETUP:"
    echo "  • Navigate to project directory"
    echo "  • Run: idf.py set-target <target>"
    echo "  • Run: idf.py build -DAPP_TYPE=<app_type>"
    echo "  • Run: idf.py flash monitor"
    echo ""
    echo "For detailed information, see: docs/README_UTILITY_SCRIPTS.md"
    exit 0
fi

# Parse arguments
PROJECT_PATH=""
APP_TYPE="$DEFAULT_APP_TYPE"
TARGET="$DEFAULT_TARGET"
IDF_VERSION="$DEFAULT_IDF_VERSION"
FORCE_OVERWRITE=false
VERBOSE=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --app-type)
            APP_TYPE="$2"
            shift 2
            ;;
        --target)
            TARGET="$2"
            shift 2
            ;;
        --idf-version)
            IDF_VERSION="$2"
            shift 2
            ;;
        --force)
            FORCE_OVERWRITE=true
            shift
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --help|-h)
            # Help already shown above
            exit 0
            ;;
        -*)
            print_error "Unknown option: $1"
            exit 1
            ;;
        *)
            if [[ -z "$PROJECT_PATH" ]]; then
                PROJECT_PATH="$1"
            else
                print_error "Multiple project paths specified: $PROJECT_PATH and $1"
                exit 1
            fi
            shift
            ;;
    esac
done

# Set default project path if not provided
if [[ -z "$PROJECT_PATH" ]]; then
    PROJECT_PATH="$(pwd)"
fi

# Convert to absolute path
PROJECT_PATH="$(cd "$(dirname "$PROJECT_PATH")" && pwd)/$(basename "$PROJECT_PATH")"

# Function to create project directory structure
create_project_structure() {
    local project_dir="$1"
    
    print_status "Creating project directory structure..."
    
    # Create main directories
    mkdir -p "$project_dir/main"
    mkdir -p "$project_dir/scripts"
    mkdir -p "$project_dir/docs"
    
    print_success "Project structure created"
}

# Function to create app_config.yml
create_app_config() {
    local project_dir="$1"
    local app_type="$2"
    local target="$3"
    local idf_version="$4"
    
    # Convert app_type to PascalCase for class name
    local class_name=$(echo "$app_type" | sed 's/_\([a-z]\)/\U\1/g' | sed 's/^\([a-z]\)/\U\1/')
    
    print_status "Creating app_config.yml..."
    
    cat > "$project_dir/app_config.yml" << EOF
# ESP32 Project Configuration
# This file centralizes all app definitions, their source files, and metadata
# Used by build scripts, flash scripts, CI workflows, and CMake

version: "1.0"
metadata:
  project: "ESP32 Project"
  default_app: "$app_type"
  target: "$target"
  idf_versions: ["$idf_version"]
  build_types: [["Debug", "Release"]]

apps:
  $app_type:
    description: "Basic $app_type example"
    source_file: "${class_name}Example.cpp"
    category: "example"
    idf_versions: ["$idf_version"]
    build_types: ["Debug", "Release"]
    ci_enabled: true
    featured: true

build_config:
  build_types:
    Debug:
      description: "Debug symbols, verbose logging, assertions enabled"
      cmake_build_type: "Debug"
      optimization: "-O0"
      debug_level: "-g3"
      defines: ["DEBUG"]
    Release:
      description: "Release build optimized for performance and size"
      cmake_build_type: "Release"
      optimization: "-O2"
      debug_level: "-g0"
      defines: ["NDEBUG"]
  
  build_directory_pattern: "build-app-{app_type}-type-{build_type}-target-{target}-idf-{idf_version}"
  project_name_pattern: "esp32_{app_type}_app"

ci_config:
  timeout_minutes: 30
  fail_fast: false
  exclude_combinations: []
  special_apps: []
EOF

    print_success "app_config.yml created"
}

# Function to create main CMakeLists.txt
create_main_cmake() {
    local project_dir="$1"
    
    print_status "Creating main CMakeLists.txt..."
    
    cat > "$project_dir/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.16)

# CRITICAL: Set variables BEFORE any other processing
# This ensures they are available during component configuration
if(DEFINED APP_TYPE)
    message(STATUS "APP_TYPE from command line: ${APP_TYPE}")
else()
    set(APP_TYPE "ascii_art")
    message(STATUS "APP_TYPE defaulting to: ${APP_TYPE}")
endif()

if(DEFINED BUILD_TYPE)
    message(STATUS "BUILD_TYPE from command line: ${BUILD_TYPE}")
else()
    set(BUILD_TYPE "Release")
    message(STATUS "BUILD_TYPE defaulting to: ${BUILD_TYPE}")
endif()

# Flexible build system for different app types
# Usage: idf.py build -DAPP_TYPE=<type> -DBUILD_TYPE=<type>
# 
# App types are dynamically read from app_config.yml (single source of truth)
# To see available app types, run: python3 scripts/get_app_info.py list
# To see app details, run: python3 scripts/get_app_info.py info <app_type>
#
# Build types: Debug, Release (default: Release)

# Validate app type by reading from app_config.yml (single source of truth)
execute_process(
    COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/scripts/get_app_info.py" list
    OUTPUT_VARIABLE VALID_APP_TYPES_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE APP_LIST_RESULT
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(NOT APP_LIST_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to read valid app types from app_config.yml")
endif()

# Convert space-separated string to CMake list
string(REPLACE " " ";" VALID_APP_TYPES "${VALID_APP_TYPES_STRING}")

if(NOT APP_TYPE IN_LIST VALID_APP_TYPES)
    message(FATAL_ERROR "Invalid APP_TYPE: ${APP_TYPE}. Valid types: ${VALID_APP_TYPES}")
endif()

# Validate build type
set(VALID_BUILD_TYPES "Debug;Release")
if(NOT BUILD_TYPE IN_LIST VALID_BUILD_TYPES)
    message(FATAL_ERROR "Invalid BUILD_TYPE: ${BUILD_TYPE}. Valid types: ${VALID_BUILD_TYPES}")
endif()

message(STATUS "Building app type: ${APP_TYPE}")
message(STATUS "Build type: ${BUILD_TYPE}")
message(STATUS "CMake source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMake current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# CRITICAL: Set these as global variables for all components
set(APP_TYPE "${APP_TYPE}" CACHE STRING "App type to build" FORCE)
set(BUILD_TYPE "${BUILD_TYPE}" CACHE STRING "Build type (Debug/Release)" FORCE)

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set project name based on app type
project(esp32_${APP_TYPE}_app)
EOF

    print_success "Main CMakeLists.txt created"
}

# Function to create main component CMakeLists.txt
create_main_component_cmake() {
    local project_dir="$1"
    
    print_status "Creating main component CMakeLists.txt..."
    
    cat > "$project_dir/main/CMakeLists.txt" << 'EOF'
# Flexible main component for different app types
# Determine source file based on APP_TYPE

# Get app type from parent
if(NOT DEFINED APP_TYPE)
    set(APP_TYPE "ascii_art")
endif()

# Debug information
message(STATUS "=== CMake Debug Information ===")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "APP_TYPE: ${APP_TYPE}")
message(STATUS "===============================")

# Get source file from centralized configuration
execute_process(
    COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/get_app_info.py" validate "${APP_TYPE}"
    RESULT_VARIABLE VALIDATE_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
)

if(NOT VALIDATE_RESULT EQUAL 0)
    message(FATAL_ERROR "Unknown APP_TYPE: ${APP_TYPE}")
endif()

execute_process(
    COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/get_app_info.py" source_file "${APP_TYPE}"
    OUTPUT_VARIABLE MAIN_SOURCE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE SOURCE_RESULT
)

if(NOT SOURCE_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to get source file for APP_TYPE: ${APP_TYPE}")
endif()

message(STATUS "Using source file from configuration: ${MAIN_SOURCE}")

# Check if source file exists
set(SOURCE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SOURCE}")
message(STATUS "Looking for source file: ${SOURCE_FILE_PATH}")

# List files in current directory for debugging
file(GLOB DEBUG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*")
message(STATUS "Files in source directory:")
foreach(DEBUG_FILE ${DEBUG_FILES})
    message(STATUS "  ${DEBUG_FILE}")
endforeach()

if(NOT EXISTS "${SOURCE_FILE_PATH}")
    # Try alternative paths for CI environment
    set(SOURCE_FILE_PATH "${CMAKE_CURRENT_LIST_DIR}/${MAIN_SOURCE}")
    message(STATUS "Trying alternative path: ${SOURCE_FILE_PATH}")
    if(NOT EXISTS "${SOURCE_FILE_PATH}")
        message(FATAL_ERROR "Source file not found: ${MAIN_SOURCE}\n"
                            "Tried paths:\n"
                            "  ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SOURCE}\n"
                            "  ${CMAKE_CURRENT_LIST_DIR}/${MAIN_SOURCE}\n"
                            "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}\n"
                            "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
    endif()
endif()

message(STATUS "Using source file: ${SOURCE_FILE_PATH}")

# Basic component registration
idf_component_register(
    SRCS "${MAIN_SOURCE}"
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer freertos
)

# Set C++ standard
target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)

# Set compiler flags based on build type
if(BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${COMPONENT_LIB} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O0
        -g3
        -DDEBUG
    )
else()
    target_compile_options(${COMPONENT_LIB} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O2
        -g
        -DNDEBUG
    )
endif()

# Add compile definitions for each example type
target_compile_definitions(${COMPONENT_LIB} PRIVATE 
    "EXAMPLE_TYPE_${APP_TYPE}=1"
)
EOF

    print_success "Main component CMakeLists.txt created"
}

# Function to create get_app_info.py script
create_get_app_info_script() {
    local project_dir="$1"
    
    print_status "Creating get_app_info.py script..."
    
    cat > "$project_dir/scripts/get_app_info.py" << 'EOF'
#!/usr/bin/env python3
"""
ESP32 App Information Script
This script provides information about ESP32 apps from configuration
"""

import sys
import yaml
import argparse
from pathlib import Path

def show_help():
    """Show comprehensive help information."""
    print("ESP32 App Information Script")
    print("")
    print("Usage: python3 get_app_info.py [COMMAND] [ARGUMENTS]")
    print("")
    print("COMMANDS:")
    print("  source_file <app_type>      - Get source file path for app")
    print("  list                        - List all available apps")
    print("  validate <app_type>         - Check if app type is valid")
    print("  info <app_type>             - Show detailed information for app")
    print("  build_types <app_type>      - Show supported build types for app")
    print("  idf_versions <app_type>     - Show supported ESP-IDF versions for app")
    print("")
    print("OPTIONS:")
    print("  --help, -h                  - Show this help message")
    print("  --verbose                    - Show detailed output")
    print("  --format <format>           - Output format: text, json, yaml (default: text)")
    print("  --project-path <path>       - Path to project directory containing app_config.yml")
    print("")
    print("EXAMPLES:")
    print("  # Basic information")
    print("  python3 get_app_info.py list")
    print("  python3 get_app_info.py source_file ascii_art")
    print("  python3 get_app_info.py validate ascii_art")
    print("")
    print("  # Detailed app information")
    print("  python3 get_app_info.py info ascii_art")
    print("  python3 get_app_info.py build_types ascii_art")
    print("  python3 get_app_info.py idf_versions ascii_art")
    print("")
    sys.exit(0)

def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description="Get information about ESP32 apps from configuration",
        add_help=False  # We'll handle help manually
    )
    
    parser.add_argument("command", nargs="?", help="Command to execute")
    parser.add_argument("app_type", nargs="?", help="Application type")
    parser.add_argument("--help", "-h", action="store_true", help="Show help message")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    parser.add_argument("--format", "-f", choices=["text", "json", "yaml"], default="text", help="Output format")
    parser.add_argument("--project-path", "-p", help="Path to project directory containing app_config.yml")
    
    args = parser.parse_args()
    
    if args.help or not args.command:
        show_help()
    
    return args

def load_config(project_path=None):
    """Load the apps configuration file."""
    if project_path:
        # Use provided project path
        project_dir = Path(project_path).resolve()
        config_file = project_dir / "app_config.yml"
    else:
        # Default behavior: assume script is in project/scripts/
        config_file = Path(__file__).parent.parent / "app_config.yml"
    
    if not config_file.exists():
        print(f"Error: Configuration file not found: {config_file}", file=sys.stderr)
        if project_path:
            print(f"Please check the project path: {project_path}", file=sys.stderr)
        sys.exit(1)
    
    try:
        with open(config_file, 'r') as f:
            return yaml.safe_load(f)
    except Exception as e:
        print(f"Error loading configuration: {e}", file=sys.stderr)
        sys.exit(1)

def get_app_source_file(app_type, project_path=None):
    """Get source file for an app type."""
    config = load_config(project_path)
    
    if app_type not in config['apps']:
        print(f"Error: Unknown app type: {app_type}", file=sys.stderr)
        sys.exit(1)
    
    return config['apps'][app_type]['source_file']

def list_apps(project_path=None):
    """List all available apps."""
    config = load_config(project_path)
    apps = list(config['apps'].keys())
    return apps

def validate_app(app_type, project_path=None):
    """Validate if app type exists."""
    config = load_config(project_path)
    return app_type in config['apps']

def main():
    """Main function."""
    args = parse_arguments()
    
    command = args.command
    app_type = args.app_type
    verbose = args.verbose
    format_output = args.format
    project_path = args.project_path

    if command == "source_file":
        if not app_type:
            print("Usage: get_app_info.py source_file <app_type>", file=sys.stderr)
            sys.exit(1)
        source_file = get_app_source_file(app_type, project_path)
        print(source_file)
    
    elif command == "list":
        apps = list_apps(project_path)
        print(" ".join(apps))
    
    elif command == "validate":
        if not app_type:
            print("Usage: get_app_info.py validate <app_type>", file=sys.stderr)
            sys.exit(1)
        is_valid = validate_app(app_type, project_path)
        print("true" if is_valid else "false")
        if not is_valid:
            sys.exit(1)
    
    else:
        print(f"Error: Unknown command: {command}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
EOF

    # Make the script executable
    chmod +x "$project_dir/scripts/get_app_info.py"
    
    print_success "get_app_info.py script created"
}

# Function to create basic source file
create_basic_source_file() {
    local project_dir="$1"
    local app_type="$2"
    
    print_status "Creating basic source file for $app_type..."
    
    # Convert app_type to PascalCase for class name
    local class_name=$(echo "$app_type" | sed 's/_\([a-z]\)/\U\1/g' | sed 's/^\([a-z]\)/\U\1/')
    local source_file="${class_name}Example.cpp"
    
    cat > "$project_dir/main/$source_file" << EOF
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

static const char *TAG = "${app_type^^}";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting ${class_name} Example");
    
    // Initialize your application here
    ESP_LOGI(TAG, "Application initialized successfully");
    
    // Main application loop
    while (1) {
        ESP_LOGI(TAG, "Hello from ${class_name} Example!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
EOF

    print_success "Basic source file created: $source_file"
}

# Function to initialize ESP-IDF project
initialize_esp_idf_project() {
    local project_dir="$1"
    local target="$2"
    
    print_status "Initializing ESP-IDF project..."
    
    # Check if ESP-IDF is available
    if ! command_exists idf.py; then
        print_warning "ESP-IDF not found. Skipping ESP-IDF initialization."
        print_status "You can initialize ESP-IDF later by running:"
        print_status "  cd $project_dir"
        print_status "  idf.py set-target $target"
        print_status "  idf.py menuconfig"
        return 0
    fi
    
    # Change to project directory
    cd "$project_dir"
    
    # Initialize ESP-IDF project
    if ! idf.py set-target "$target" > /dev/null 2>&1; then
        print_warning "Failed to set ESP-IDF target to $target"
        print_status "You can set the target manually later:"
        print_status "  cd $project_dir"
        print_status "  idf.py set-target $target"
        return 0
    fi
    
    # Create basic sdkconfig if it doesn't exist
    if [[ ! -f "sdkconfig" ]]; then
        print_status "Creating basic sdkconfig..."
        idf.py menuconfig --non-interactive > /dev/null 2>&1 || true
    fi
    
    print_success "ESP-IDF project initialized for target $target"
}

# Function to verify project setup
verify_project_setup() {
    local project_dir="$1"
    local app_type="$2"
    
    print_status "Verifying project setup..."
    
    # Check required files
    local required_files=(
        "app_config.yml"
        "CMakeLists.txt"
        "main/CMakeLists.txt"
        "scripts/get_app_info.py"
    )
    
    # Check optional files
    local optional_files=(
        "sdkconfig"
    )
    
    for file in "${required_files[@]}"; do
        if [[ ! -f "$project_dir/$file" ]]; then
            print_error "Required file missing: $file"
            return 1
        fi
    done
    
    # Check optional files
    for file in "${optional_files[@]}"; do
        if [[ ! -f "$project_dir/$file" ]]; then
            print_warning "Optional file missing: $file (will be created when ESP-IDF is initialized)"
        fi
    done
    
    # Check if source file exists
    local source_file=$(python3 "$project_dir/scripts/get_app_info.py" source_file "$app_type" --project-path "$project_dir")
    if [[ ! -f "$project_dir/main/$source_file" ]]; then
        print_error "Source file missing: main/$source_file"
        return 1
    fi
    
    # Test get_app_info.py script
    if ! python3 "$project_dir/scripts/get_app_info.py" validate "$app_type" --project-path "$project_dir" > /dev/null 2>&1; then
        print_error "get_app_info.py script validation failed"
        return 1
    fi
    
    print_success "Project setup verification passed"
}

# Main function
main() {
    echo "================================================================"
    echo "🚀 ESP32 Project Setup"
    echo "================================================================"
    echo ""
    
    print_status "Project path: $PROJECT_PATH"
    print_status "App type: $APP_TYPE"
    print_status "Target: $TARGET"
    print_status "ESP-IDF version: $IDF_VERSION"
    echo ""
    
    # Check if project directory already exists
    if [[ -d "$PROJECT_PATH" ]] && [[ "$FORCE_OVERWRITE" != true ]]; then
        print_error "Project directory already exists: $PROJECT_PATH"
        print_status "Use --force to overwrite existing project"
        exit 1
    fi
    
    # Create project directory if it doesn't exist
    if [[ ! -d "$PROJECT_PATH" ]]; then
        print_status "Creating project directory: $PROJECT_PATH"
        mkdir -p "$PROJECT_PATH"
    elif [[ "$FORCE_OVERWRITE" == true ]]; then
        print_warning "Overwriting existing project directory: $PROJECT_PATH"
    fi
    
    # Create project structure
    create_project_structure "$PROJECT_PATH"
    
    # Create configuration files
    create_app_config "$PROJECT_PATH" "$APP_TYPE" "$TARGET" "$IDF_VERSION"
    create_main_cmake "$PROJECT_PATH"
    create_main_component_cmake "$PROJECT_PATH"
    create_get_app_info_script "$PROJECT_PATH"
    
    # Create basic source file
    create_basic_source_file "$PROJECT_PATH" "$APP_TYPE"
    
    # Initialize ESP-IDF project
    initialize_esp_idf_project "$PROJECT_PATH" "$TARGET"
    
    # Verify setup
    if ! verify_project_setup "$PROJECT_PATH" "$APP_TYPE"; then
        print_error "Project setup verification failed"
        exit 1
    fi
    
    echo ""
    echo "================================================================"
    print_success "🎉 ESP32 project setup complete!"
    echo "================================================================"
    echo ""
    print_status "Project created at: $PROJECT_PATH"
    print_status "Initial app type: $APP_TYPE"
    print_status "Target: $TARGET"
    echo ""
    print_status "Next steps:"
    print_status "1. Navigate to project directory: cd $PROJECT_PATH"
    print_status "2. Build the project: idf.py build -DAPP_TYPE=$APP_TYPE"
    print_status "3. Flash to device: idf.py flash monitor"
    print_status "4. List available apps: python3 scripts/get_app_info.py list"
    echo ""
    print_status "To add more apps:"
    print_status "1. Edit app_config.yml to add new app definitions"
    print_status "2. Create corresponding source files in main/"
    print_status "3. Build with: idf.py build -DAPP_TYPE=<new_app_type>"
    echo ""
    print_status "Happy coding! 🚀"
}

# Run main function
main "$@"