# ESP32 HardFOC Interface Wrapper - Configuration System Guide

This document provides comprehensive documentation for the ESP32 configuration system, including
YAML configuration management, validation, and integration with all scripts.

---

**Navigation**: [← Previous: Flash System](README*FLASH*SYSTEM.md) | [Back to Scripts](../README.md)
| [Next: Logging System →](README*LOGGING*SYSTEM.md)

---

## 📋 **Table of Contents**

- [📋 Overview](#-overview)
- [🏗️ Architecture and Design](#️-architecture-and-design)
- [⚙️ Configuration File Structure](#️-configuration-file-structure)
- [🔧 Configuration Loading and Validation](#️-configuration-loading-and-validation)
- [🔄 Environment Variable Overrides](#️-environment-variable-overrides)
- [🚀 Usage Examples and Patterns](#️-usage-examples-and-patterns)
- [🔍 Troubleshooting and Debugging](#️-troubleshooting-and-debugging)
- [📚 Reference and Examples](#️-reference-and-examples)

## 📋 **Overview**

The ESP32 configuration system provides centralized,
intelligent configuration management for all scripts in the HardFOC Interface Wrapper project.
It features YAML-based configuration, automatic validation, intelligent fallbacks,
and cross-platform compatibility.

### **Core Features**
- **Centralized Configuration**: Single YAML file manages all script behavior
- **Enhanced Validation**: Smart combination validation and error prevention
- **Smart Defaults**: Automatic ESP-IDF version selection based on app and build type
- **Smart Fallbacks**: Graceful degradation when configuration is incomplete
- **Cross-Platform**: Consistent behavior across Linux and macOS
- **Environment Integration**: Environment variable overrides and customization

### **Key Capabilities**
- YAML configuration parsing with `yq` and fallback methods
- **Smart combination validation** - Prevents invalid app + build type + IDF version combinations
- **Automatic ESP-IDF version selection** - Chooses the right version when not specified
- Application and build type validation
- ESP-IDF version compatibility checking
- Environment variable override support
- Configuration integrity validation
- Cross-script configuration sharing

## 🏗️ **Architecture and Design**

### **System Architecture**
```yaml
app*config.yml → config*loader.sh → Script Functions → Environment Variables
     ↓              ↓                    ↓                    ↓
Configuration    Parsing &        Validation &      Script
Definitions      Validation       Fallbacks         Execution
```text

### **Component Interaction**
- **`app*config.yml`**: Centralized configuration source
- **`config*loader.sh`**: Configuration parsing and validation engine
- **Script Functions**: Configuration access and validation functions
- **Environment Variables**: Runtime configuration overrides

### **Design Principles**
- **Single Source of Truth**: All configuration in one YAML file
- **Fail-Fast Validation**: Configuration errors caught early with clear messages
- **Intelligent Defaults**: Sensible fallbacks when configuration is incomplete
- **Cross-Platform Consistency**: Uniform behavior across operating systems
- **Performance Optimization**: Efficient parsing and caching mechanisms

---

## 🛡️ **Enhanced Validation System**

The configuration system now includes comprehensive validation to prevent invalid build combinations
and provide clear guidance to users.

### **Validation Features**

- **🔍 Combination Validation** - Validates app + build type + IDF version combinations
- **🚫 Invalid Build Prevention** - Blocks builds with unsupported combinations
- **💡 Smart Error Messages** - Clear guidance on what combinations are allowed
- **🧠 Smart Defaults** - Automatic ESP-IDF version selection when not specified

### **Validation Functions**

The configuration system provides several new validation functions:

#### **Combination Validation**
```bash
## Check if a build combination is valid
is*valid*combination() {
    local app*type="$1"
    local build*type="$2"
    local idf*version="$3"
    
    # Validate app type exists
    if ! is*valid*app*type "$app*type"; then return 1; fi
    
    # Validate build type is supported
    if ! is*valid*build*type "$build*type"; then return 1; fi
    
    # Check if app supports this IDF version
    local app*idf*versions*array=$(get*app*idf*versions*array "$app*type")
    if ! echo "$app*idf*versions*array" | grep -q "$idf*version"; then return 1; fi
    
    # Check if app supports this build type for this IDF version
    local app*build*types=$(get*build*types "$app*type")
    local clean*build*types=$(echo "$app*build*types" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' ' ')
    
    if [[ "$clean*build*types" == *"$build*type"* ]]; then return 0; fi
    
    return 1
}
```text

#### **Smart Default Selection**
```bash
## Enhanced IDF version selection with comprehensive validation
## Now handled by enhanced get*idf*version() and is*valid*combination()

## Get app-specific IDF version with fallback
idf*version=$(get*idf*version "gpio*test")

## Comprehensive combination validation
if is*valid*combination "gpio*test" "Release" "release/v5.5"; then
    echo "Valid combination for CI pipeline"
fi

## Enhanced build type validation with app overrides
if is*valid*build*type "Release" "gpio*test" "release/v5.5"; then
    echo "Valid build type for app and IDF version"
fi
```text

### **Validation Flow**

```text
┌─────────────────────────────────────────────────────────────────────────────┐
│                           CONFIGURATION LOADING                             │
│  app*config.yml → config*loader.sh → Validation Functions                   │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                        BASIC VALIDATION FIRST                               │
│  • Validate app type exists                                                 │
│  • Validate build type is supported                                         │
│  • Fail fast if basic validation fails                                      │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                        SMART DEFAULT SELECTION                              │
│  • Only if basic validation passes                                          │
│  • Check app-specific IDF versions                                          │
│  • Find first version supporting requested build type                       │
│  • Fallback to global defaults if needed                                    │
│  • Result example: release/v5.5                                                     │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                        FINAL COMBINATION VALIDATION                         │
│  • Single comprehensive check (no redundant individual validations)         │
│  • Functions remain standalone-safe for independent sourcing                │
│  • Check combination constraints                                            │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           VALIDATION RESULT                                 │
│  ✅ VALID: gpio*test + Release + release/v5.5                               │
│  → Proceed with build                                                       │
│                                                                             │
│  ❌ INVALID: gpio*test + Release + release/v5.4                             │
│  → Show error with valid combinations                                       │
│  → Provide helpful next steps                                               │
└─────────────────────────────────────────────────────────────────────────────┘
```text

## ⚙️ **Configuration File Structure**

### **Configuration File Location**
The configuration system reads from:
```text
examples/esp32/app*config.yml
```text

### **Configuration Schema**

#### **Metadata Section**
```yaml
## Global metadata and defaults
metadata:
  default*app: "ascii*art"            # Default application to build
  default*build*type: "Release"       # Default build configuration
  target: "esp32c6"                   # Target MCU architecture
  idf*versions: ["release/v5.5"]      # Supported ESP-IDF versions
  description: "ESP32 HardFOC Interface Wrapper Configuration"
  version: "2.1.0"
  last*updated: "2025-01-15"
```text

#### **Applications Section**
```yaml
## Application definitions and configurations
apps:
  ascii*art:
    description: "ASCII art generator application"
    source*file: "AsciiArtComprehensiveTest.cpp"
    category: "utility"
    build*types: ["Debug", "Release"]
    idf*versions: ["release/v5.5"]
    ci*enabled: true
    featured: true
    dependencies: []
    tags: ["demo", "utility", "ascii"]

  gpio*test:
    description: "GPIO peripheral testing application"
    source*file: "GpioComprehensiveTest.cpp"
    category: "peripheral"
    build*types: ["Debug", "Release"]
    idf*versions: ["release/v5.5"]
    ci*enabled: true
    featured: true
    dependencies: ["gpio*driver"]
    tags: ["peripheral", "gpio", "testing"]

  adc*test:
    description: "ADC peripheral testing application"
    source*file: "AdcComprehensiveTest.cpp"
    category: "peripheral"
    build*types: ["Debug", "Release"]
    idf*versions: ["release/v5.5"]
    ci*enabled: true
    featured: false
    dependencies: ["adc*driver", "gpio*driver"]
    tags: ["peripheral", "adc", "analog"]
```text

#### **Build Configuration Section**
```yaml
## Build system configuration
build*config:
  build*types:
    Debug:
      description: "Debug build with symbols and verbose logging"
      cmake*build*type: "Debug"
      optimization: "-O0"
      debug*level: "-g3"
      defines: ["DEBUG", "VERBOSE*LOGGING"]
      assertions: true
      logging*level: "DEBUG"
      stack*usage: true
      
    Release:
      description: "Optimized build for production deployment"
      cmake*build*type: "Release"
      optimization: "-O2"
      debug*level: "-g"
      defines: ["NDEBUG"]
      assertions: false
      logging*level: "INFO"
      stack*usage: false
      
    RelWithDebInfo:
      description: "Release build with debug information"
      cmake*build*type: "RelWithDebInfo"
      optimization: "-O2"
      debug*level: "-g"
      defines: ["NDEBUG"]
      assertions: false
      logging*level: "INFO"
      stack*usage: false
      
    MinSizeRel:
      description: "Release build optimized for size"
      cmake*build*type: "MinSizeRel"
      optimization: "-Os"
      debug*level: "-g"
      defines: ["NDEBUG", "MINIMAL*LOGGING"]
      assertions: false
      logging*level: "WARN"
      stack*usage: false

  # Build system patterns
  build*directory*pattern: "build*{app*type}*{build*type}"
  project*name*pattern: "esp32*project*{app*type}*app"
  
  # Build optimization
  ccache*enabled: true
  parallel*builds: true
  incremental*builds: true
  
  # Build validation
  size*analysis: true
  dependency*checking: true
  warning*as*errors: false
```text

#### **Flash Configuration Section**
```yaml
## Flash system configuration
flash*config:
  # Port detection
  auto*detect*ports: true
  port*scan*timeout: 5
  port*test*timeout: 3
  
  # Flash parameters
  flash*mode: "dio"
  flash*freq: "80m"
  flash*size: "4MB"
  
  # Monitor settings
  monitor*baud: 115200
  monitor*data*bits: 8
  monitor*parity: "none"
  monitor*stop*bits: 1
  
  # Logging
  auto*logging: true
  log*rotation: true
  max*log*files: 50
  log*retention*days: 30
```text

#### **System Configuration Section**
```yaml
## System and environment configuration
system*config:
  # Operating system support
  platforms:
    - "linux"
    - "macos"
    
  
  # Shell requirements
  shell: "bash"
  min*bash*version: "4.0"
  
  # Python requirements
  python*version: "3.6+"
  required*packages: ["PyYAML"]
  
  # Tool requirements
  required*tools: ["git", "cmake", "ninja", "ccache"]
  optional*tools: ["yq", "screen", "tmux"]
  
  # Cache configuration
  cache*directories:
    - "$HOME/.ccache"
    - "$HOME/.espressif"
    - "$HOME/.cache/pip"
```yaml

### **Configuration File Validation**

#### **YAML Schema Validation**
The configuration system validates:
- **Syntax**: Valid YAML format and structure
- **Required Fields**: Essential configuration elements
- **Data Types**: Correct data types for each field
- **Relationships**: Valid references between sections
- **Constraints**: Value ranges and valid options

#### **Configuration Integrity Checks**
```yaml
## Validation rules
validation:
  # App validation
  apps:
    - source*file must exist
    - build*types must be valid
    - idf*versions must be supported
    
  # Build type validation
  build*types:
    - cmake*build*type must be valid
    - optimization flags must be valid
    - defines must be strings
    
  # Metadata validation
  metadata:
    - default*app must exist in apps
    - default*build*type must be valid
    - target must be supported
```text

## 🔧 **Configuration Loading and Validation**

### **Configuration Loading Process**

#### **1. Primary Loading Method (yq)**
```bash
## Check for yq availability
check*yq() {
    if command -v yq &> /dev/null; then
        # Detect yq version and set syntax
        local yq*version=$(yq --version | grep -oE '[0-9]+\.[0-9]+' | head -1)
        if [[ "$(echo "$yq*version" | cut -d. -f1)" -ge 4 ]]; then
            export YQ*SYNTAX="eval"
        else
            export YQ*SYNTAX="direct"
        fi
        return 0
    fi
    return 1
}

## Load configuration with yq
load*config*yq() {
    if ! check*yq; then
        return 1
    fi
    
    # Export configuration as environment variables
    export CONFIG*DEFAULT*APP=$(run*yq '.metadata.default*app' -r)
    export CONFIG*DEFAULT*BUILD*TYPE=$(run*yq '.metadata.default*build*type' -r)
    export CONFIG*TARGET=$(run*yq '.metadata.target' -r)
    export CONFIG*DEFAULT*IDF*VERSION=$(run*yq '.metadata.idf*versions[0]' -r)
    
    return 0
}
```text

#### **2. Fallback Loading Method (grep/sed)**
```bash
## Fallback parsing without yq
load*config*basic() {
    # Extract basic configuration using grep and sed
    export CONFIG*DEFAULT*APP=$(grep -A 10 "metadata:" "$CONFIG*FILE" | \
        grep "default*app:" | sed 's/.*default*app: *"*\([^"]*\)"*.*/\1/')
    
    export CONFIG*DEFAULT*BUILD*TYPE=$(grep -A 10 "metadata:" "$CONFIG*FILE" | \
        grep "default*build*type:" | sed 's/.*default*build*type: *"*\([^"]*\)"*.*/\1/')
    
    export CONFIG*TARGET=$(grep -A 10 "metadata:" "$CONFIG*FILE" | \
        grep "target:" | sed 's/.*target: *"*\([^"]*\)"*.*/\1/')
    
    export CONFIG*DEFAULT*IDF*VERSION=$(grep -A 10 "metadata:" "$CONFIG*FILE" | \
        grep "idf*versions:" | sed 's/.*idf*versions: *\[*"*\([^"]*\)"*.*/\1/' | head -1)
}
```text

### **Configuration Validation Functions**

#### **Application Validation**
```bash
## Validate application type
validate*app*type() {
    local app*type="$1"
    if is*valid*app*type "$app*type"; then
        return 0
    else
        echo "ERROR: Invalid app type: $app*type" >&2
        echo "Available types: $(get*app*types)" >&2
        return 1
    fi
}

## Check if app type is valid
is*valid*app*type() {
    local app*type="$1"
    local app*types=$(get*app*types)
    [[ " $app*types " =~ " $app*type " ]]
}

## Get all available app types
get*app*types() {
    if check*yq; then
        run*yq '.apps | keys | .[]' -r | tr '\n' ' '
    else
        # Fallback: extract from apps section
        grep -A 20 "apps:" "$CONFIG*FILE" | \
            grep -E "^  [a-zA-Z*][a-zA-Z0-9*]*:" | \
            sed 's/^  \([^:]*\):.*/\1/' | tr '\n' ' '
    fi
}
```text

#### **Build Type Validation**
```bash
## Validate build type
validate*build*type() {
    local build*type="$1"
    if is*valid*build*type "$build*type"; then
        return 0
    else
        echo "ERROR: Invalid build type: $build*type" >&2
        echo "Available types: $(get*build*types)" >&2
        return 1
    fi
}

## Check if build type is valid
is*valid*build*type() {
    local build*type="$1"
    local build*types=$(get*build*types)
    [[ " $build*types " =~ " $build*type " ]]
}

## Get all available build types
get*build*types() {
    if check*yq; then
        run*yq '.build*config.build*types | keys | .[]' -r | tr '\n' ' '
    else
        # Fallback: extract from build*config section
        grep -A 20 "build*config:" "$CONFIG*FILE" | \
            grep -A 10 "build*types:" | \
            grep -E "^    [A-Za-z][A-Za-z0-9]*:" | \
            sed 's/^    \([^:]*\):.*/\1/' | tr '\n' ' '
    fi
}
```text

#### **ESP-IDF Version Validation**
```bash
## Validate ESP-IDF version compatibility
validate*app*idf*version() {
    local app*type="$1"
    local idf*version="$2"
    
    if is*valid*app*idf*version "$app*type" "$idf*version"; then
        return 0
    else
        echo "ERROR: App '$app*type' does not support ESP-IDF version '$idf*version'" >&2
        echo "Supported versions for '$app*type': $(get*app*idf*versions "$app*type")" >&2
        return 1
    fi
}

## Check if app supports ESP-IDF version
is*valid*app*idf*version() {
    local app*type="$1"
    local idf*version="$2"
    
    # Check app-specific versions first
    local app*versions=$(get*app*idf*versions "$app*type")
    if [[ -n "$app*versions" ]]; then
        [[ " $app*versions " =~ " $idf*version " ]]
        return $?
    fi
    
    # Fall back to global versions
    local global*versions=$(get*idf*versions)
    [[ " $global*versions " =~ " $idf*version " ]]
}

## Get ESP-IDF versions supported by app
get*app*idf*versions() {
    local app*type="$1"
    
    if check*yq; then
        run*yq ".apps.$app*type.idf*versions[]" -r 2>/dev/null | tr '\n' ' '
    else
        # Fallback: extract app-specific versions
        grep -A 20 "apps:" "$CONFIG*FILE" | \
            grep -A 10 "  $app*type:" | \
            grep "idf*versions:" | \
            sed 's/.*idf*versions: *\[*"*\([^"]*\)"*.*/\1/' | tr '\n' ' '
    fi
}
```text

### **Configuration Access Functions**

#### **Application Information**
```bash
## Get app description
get*app*description() {
    local app*type="$1"
    
    if check*yq; then
        run*yq ".apps.$app*type.description" -r 2>/dev/null
    else
        # Fallback: extract description
        grep -A 20 "apps:" "$CONFIG*FILE" | \
            grep -A 10 "  $app*type:" | \
            grep "description:" | \
            sed 's/.*description: *"*\([^"]*\)"*.*/\1/'
    fi
}

## Get app source file
get*app*source*file() {
    local app*type="$1"
    
    if check*yq; then
        run*yq ".apps.$app*type.source*file" -r 2>/dev/null
    else
        # Fallback: extract source file
        grep -A 20 "apps:" "$CONFIG*FILE" | \
            grep -A 10 "  $app*type:" | \
            grep "source*file:" | \
            sed 's/.*source*file: *"*\([^"]*\)"*.*/\1/'
    fi
}

## Get app category
get*app*category() {
    local app*type="$1"
    
    if check*yq; then
        run*yq ".apps.$app*type.category" -r 2>/dev/null
    else
        # Fallback: extract category
        grep -A 20 "apps:" "$CONFIG*FILE" | \
            grep -A 10 "  $app*type:" | \
            grep "category:" | \
            sed 's/.*category: *"*\([^"]*\)"*.*/\1/'
    fi
}
```text

#### **Build Configuration**
```bash
## Get build directory
get*build*directory() {
    local app*type="$1"
    local build*type="$2"
    
    local pattern=$(get*build*directory*pattern)
    echo "$pattern" | sed "s/{app*type}/$app*type/g" | sed "s/{build*type}/$build*type/g"
}

## Get project name
get*project*name() {
    local app*type="$1"
    
    local pattern=$(get*project*name*pattern)
    echo "$pattern" | sed "s/{app*type}/$app*type/g"
}

## Get build directory pattern
get*build*directory*pattern() {
    if check*yq; then
        run*yq '.build*config.build*directory*pattern' -r 2>/dev/null || echo "build*{app*type}*{build*type}"
    else
        # Fallback: extract pattern
        grep -A 20 "build*config:" "$CONFIG*FILE" | \
            grep "build*directory*pattern:" | \
            sed 's/.*build*directory*pattern: *"*\([^"]*\)"*.*/\1/' || echo "build*{app*type}*{build*type}"
    fi
}
```text

## 🔄 **Environment Variable Overrides**

### **Configuration Override System**

#### **Environment Variable Priority**
The configuration system uses a priority-based override system:

```bash
## Priority order (highest to lowest)
1. Command line arguments
2. Environment variables
3. Configuration file defaults
4. Hard-coded fallbacks
```text

#### **Supported Environment Variables**
```bash
## Project path configuration (for portable scripts)
export PROJECT*PATH="/path/to/project"  # Override project directory location

## Application configuration overrides
export CONFIG*DEFAULT*APP="gpio*test"
export CONFIG*DEFAULT*BUILD*TYPE="Debug"
export CONFIG*DEFAULT*IDF*VERSION="release/v5.4"
export CONFIG*TARGET="esp32c6"

## Build system overrides
export CLEAN=1                    # Force clean builds
export USE*CCACHE=0               # Disable ccache
export BUILD*VERBOSE=1            # Enable verbose build output

## Flash system overrides
export ESPPORT="/dev/ttyUSB0"     # Override port detection
export MONITOR*BAUD=230400        # Override monitor baud rate
export FLASH*MODE="dio"           # Override flash mode

## Debug and verbose overrides
export DEBUG=1                    # Enable debug mode
export IDF*VERBOSE=1              # Enable ESP-IDF verbose output
export CONFIG*VERBOSE=1           # Enable configuration verbose output
```text

### **Portable Configuration**

The configuration system supports portable scripts through the `PROJECT*PATH` environment variable
and `--project-path` command-line flag.

#### **Project Path Resolution**
```bash
## Priority order for project path resolution:
1. --project-path command-line flag
2. PROJECT*PATH environment variable  
3. Default: ../ relative to script location
```text

#### **Portable Usage Examples**
```bash
## Using --project-path flag
./build*app.sh --project-path /path/to/project gpio*test Release
./flash*app.sh --project-path ../project flash*monitor adc*test
./manage*idf.sh --project-path /opt/esp32-project list

## Using PROJECT*PATH environment variable
export PROJECT*PATH=/path/to/project
./build*app.sh gpio*test Release
./flash*app.sh flash*monitor adc*test

## Python scripts
python3 get*app*info.py list --project-path /path/to/project
python3 generate*matrix.py --project-path /path/to/project
```yaml

#### **Configuration File Discovery**
When using portable scripts, the system automatically:
1. Resolves the project directory path (absolute or relative)
2. Looks for `app*config.yml` in the project directory
3. Validates that the configuration file exists
4. Loads and parses the configuration

#### **Error Handling**
```bash
## Clear error messages for missing project or config
ERROR: PROJECT*PATH specified but app*config.yml not found: /path/to/project/app*config.yml
Please check the project path or unset PROJECT*PATH to use default location.
```text

### **Dynamic Configuration Updates**

#### **Runtime Configuration Changes**
```bash
## Update configuration at runtime
update*config() {
    local key="$1"
    local value="$2"
    
    case "$key" in
        "default*app")
            export CONFIG*DEFAULT*APP="$value"
            ;;
        "default*build*type")
            export CONFIG*DEFAULT*BUILD*TYPE="$value"
            ;;
        "default*idf*version")
            export CONFIG*DEFAULT*IDF*VERSION="$value"
            ;;
        "target")
            export CONFIG*TARGET="$value"
            ;;
        *)
            echo "Unknown configuration key: $key" >&2
            return 1
            ;;
    esac
}

## Usage example
update*config "default*app" "adc*test"
update*config "default*build*type" "Debug"
```text

#### **Configuration Validation at Runtime**
```bash
## Validate current configuration
validate*current*config() {
    local errors=0
    
    # Validate default app
    if ! is*valid*app*type "$CONFIG*DEFAULT*APP"; then
        echo "ERROR: Invalid default app: $CONFIG*DEFAULT*APP" >&2
        ((errors++))
    fi
    
    # Validate default build type
    if ! is*valid*build*type "$CONFIG*DEFAULT*BUILD*TYPE"; then
        echo "ERROR: Invalid default build type: $CONFIG*DEFAULT*BUILD*TYPE" >&2
        ((errors++))
    fi
    
    # Validate ESP-IDF version
    if ! is*valid*idf*version "$CONFIG*DEFAULT*IDF*VERSION"; then
        echo "ERROR: Invalid ESP-IDF version: $CONFIG*DEFAULT*IDF*VERSION" >&2
        ((errors++))
    fi
    
    return $errors
}
```text

## 🚀 **Usage Examples and Patterns**

### **Basic Configuration Usage**

#### **1. Load and Validate Configuration**
```bash
## Source configuration loader
source ./scripts/config*loader.sh

## Initialize configuration
init*config

## Validate configuration
if ! validate*current*config; then
    echo "Configuration validation failed"
    exit 1
fi

## Use configuration
echo "Default app: $CONFIG*DEFAULT*APP"
echo "Default build type: $CONFIG*DEFAULT*BUILD*TYPE"
echo "Target: $CONFIG*TARGET"
```text

#### **2. Application Configuration Access**
```bash
## Get application information
app*types=$(get*app*types)
echo "Available apps: $app*types"

## Get specific app details
description=$(get*app*description "gpio*test")
source*file=$(get*app*source*file "gpio*test")
category=$(get*app*category "gpio*test")

echo "GPIO Test: $description"
echo "Source: $source*file"
echo "Category: $category"
```text

#### **3. Build Configuration Access**
```bash
## Get build configuration
build*types=$(get*build*types)
echo "Available build types: $build*types"

## Get build directory
build*dir=$(get*build*directory "gpio*test" "Release")
echo "Build directory: $build*dir"

## Get project name
project*name=$(get*project*name "gpio*test")
echo "Project name: $project*name"
```text

### **Advanced Configuration Patterns**

#### **1. Configuration-Driven Scripts**
```bash
#!/bin/bash
## Example: Configuration-driven build script

source ./scripts/config*loader.sh
init*config

## Validate parameters
APP*TYPE=${1:-$CONFIG*DEFAULT*APP}
BUILD*TYPE=${2:-$CONFIG*DEFAULT*BUILD*TYPE}

## Validate configuration
if ! validate*app*type "$APP*TYPE"; then
    exit 1
fi

if ! validate*build*type "$BUILD*TYPE"; then
    exit 1
fi

## Get configuration
SOURCE*FILE=$(get*app*source*file "$APP*TYPE")
BUILD*DIR=$(get*build*directory "$APP*TYPE" "$BUILD*TYPE")
PROJECT*NAME=$(get*project*name "$APP*TYPE")

echo "Building $APP*TYPE ($BUILD*TYPE)"
echo "Source: $SOURCE*FILE"
echo "Build dir: $BUILD*DIR"
echo "Project: $PROJECT*NAME"
```text

#### **2. Dynamic Configuration Updates**
```bash
#!/bin/bash
## Example: Dynamic configuration management

source ./scripts/config*loader.sh
init*config

## Function to update configuration
update*app*config() {
    local app*type="$1"
    local key="$2"
    local value="$3"
    
    # Validate app type
    if ! is*valid*app*type "$app*type"; then
        echo "Invalid app type: $app*type" >&2
        return 1
    fi
    
    # Update configuration (this would modify the YAML file)
    echo "Updating $app*type.$key = $value"
    # Implementation would use yq or similar tool
}

## Usage
update*app*config "gpio*test" "featured" "true"
update*app*config "adc*test" "ci*enabled" "false"
```text

#### **3. Configuration Validation Scripts**
```bash
#!/bin/bash
## Example: Configuration validation script

source ./scripts/config*loader.sh
init*config

echo "Validating configuration..."

## Check all applications
errors=0
for app in $(get*app*types); do
    echo "Checking app: $app"
    
    # Validate source file exists
    source*file=$(get*app*source*file "$app")
    if [[ ! -f "$source*file" ]]; then
        echo "ERROR: Source file not found: $source*file" >&2
        ((errors++))
    fi
    
    # Validate build types
    app*build*types=$(get*build*types "$app")
    for build*type in $app*build*types; do
        if ! is*valid*build*type "$build*type"; then
            echo "ERROR: Invalid build type for $app: $build*type" >&2
            ((errors++))
        fi
    done
    
    # Validate ESP-IDF versions
    app*idf*versions=$(get*app*idf*versions "$app")
    for idf*version in $app*idf*versions; do
        if ! is*valid*idf*version "$idf*version"; then
            echo "ERROR: Invalid ESP-IDF version for $app: $idf*version" >&2
            ((errors++))
        fi
    done
done

if [[ $errors -eq 0 ]]; then
    echo "Configuration validation passed"
    exit 0
else
    echo "Configuration validation failed with $errors errors"
    exit 1
fi
```text

### **Integration Examples**

#### **1. CMake Integration**
```cmake
## CMakeLists.txt configuration integration
cmake*minimum*required(VERSION 3.16)

## Get app information from configuration
execute*process(
    COMMAND bash -c "source ${CMAKE*SOURCE*DIR}/scripts/config*loader.sh && init*config && echo \$CONFIG*DEFAULT*APP"
    OUTPUT*VARIABLE DEFAULT*APP
    OUTPUT*STRIP*TRAILING*WHITESPACE
)

## Get build type from configuration
execute*process(
    COMMAND bash -c "source ${CMAKE*SOURCE*DIR}/scripts/config*loader.sh && init*config && echo \$CONFIG*DEFAULT*BUILD*TYPE"
    OUTPUT*VARIABLE DEFAULT*BUILD*TYPE
    OUTPUT*STRIP*TRAILING*WHITESPACE
)

## Use configuration values
project(esp32*${DEFAULT*APP}*app)

## Set build type
set(CMAKE*BUILD*TYPE ${DEFAULT*BUILD*TYPE})
```text

#### **2. CI/CD Integration**
```yaml
## GitHub Actions configuration integration
- name: Validate Configuration
  run: |
    cd examples/esp32
    source ./scripts/config*loader.sh
    init*config
    if ! validate*current*config; then
      echo "Configuration validation failed"
      exit 1
    fi

- name: Build All CI Apps
  run: |
    cd examples/esp32
    source ./scripts/config*loader.sh
    init*config
    
    # Get CI-enabled apps
    for app in $(get*app*types); do
      if is*ci*enabled "$app"; then
        echo "Building $app"
        ./scripts/build*app.sh "$app" Release
      fi
    done
```yaml

## 🔍 **Troubleshooting and Debugging**

### **Common Configuration Issues**

#### **1. Configuration File Not Found**
**Problem**: Scripts cannot find `app*config.yml`
**Symptoms**: "Configuration file not found" or "app*config.yml not found" errors
**Solutions**:
```bash
## Check file existence
ls -la app*config.yml

## Check file permissions
ls -la app*config.yml

## Verify working directory
pwd
ls -la

## Check file path
find . -name "app*config.yml"
```yaml

#### **2. YAML Syntax Errors**
**Problem**: Invalid YAML syntax in configuration file
**Symptoms**: "YAML parse error" or "Invalid YAML" errors
**Solutions**:
```bash
## Validate YAML syntax with yq
yq eval app*config.yml

## Check for common syntax issues
grep -n ":" app*config.yml | grep -v "^\s*#"

## Use online YAML validator
## https://www.yamllint.com/
```text

#### **3. Configuration Validation Failures**
**Problem**: Configuration validation fails
**Symptoms**: "Configuration validation failed" or "Invalid configuration" errors
**Solutions**:
```bash
## Enable verbose configuration output
export CONFIG*VERBOSE=1

## Check configuration manually
source ./scripts/config*loader.sh
init*config
echo "Default app: $CONFIG*DEFAULT*APP"
echo "Default build type: $CONFIG*DEFAULT*BUILD*TYPE"

## Validate specific sections
validate*app*type "gpio*test"
validate*build*type "Release"
```text

#### **4. Environment Variable Conflicts**
**Problem**: Environment variables override configuration incorrectly
**Symptoms**: Unexpected behavior or wrong defaults
**Solutions**:
```bash
## Check environment variables
env | grep CONFIG*
env | grep ESP

## Clear conflicting variables
unset CONFIG*DEFAULT*APP
unset CONFIG*DEFAULT*BUILD*TYPE

## Reload configuration
source ./scripts/config*loader.sh
init*config
```text

### **Debug and Verbose Mode**

#### **Enabling Configuration Debug Output**
```bash
## Enable configuration debug mode
export CONFIG*DEBUG=1
export CONFIG*VERBOSE=1

## Source configuration loader
source ./scripts/config*loader.sh
init*config

## Debug information available
- Configuration file loading details
- YAML parsing results
- Validation process information
- Environment variable overrides
- Fallback mechanism details
```text

#### **Configuration Debugging Functions**
```bash
## Debug configuration loading
debug*config*loading() {
    echo "=== Configuration Loading Debug ==="
    echo "Config file: $CONFIG*FILE"
    echo "YQ available: $(check*yq && echo "Yes" || echo "No")"
    echo "YQ syntax: $YQ*SYNTAX"
    echo "Config file exists: $([ -f "$CONFIG*FILE" ] && echo "Yes" || echo "No")"
    echo "Config file readable: $([ -r "$CONFIG*FILE" ] && echo "Yes" || echo "No")"
}

## Debug current configuration
debug*current*config() {
    echo "=== Current Configuration Debug ==="
    echo "Default app: $CONFIG*DEFAULT*APP"
    echo "Default build type: $CONFIG*DEFAULT*BUILD*TYPE"
    echo "Target: $CONFIG*TARGET"
    echo "Default IDF version: $CONFIG*DEFAULT*IDF*VERSION"
    echo "Available apps: $(get*app*types)"
    echo "Available build types: $(get*build*types)"
}
```text

### **Configuration Testing**

#### **Configuration Test Scripts**
```bash
#!/bin/bash
## Example: Configuration testing script

source ./scripts/config*loader.sh

echo "Testing configuration system..."

## Test 1: Basic loading
echo "Test 1: Basic configuration loading"
if init*config; then
    echo "✓ Configuration loaded successfully"
else
    echo "✗ Configuration loading failed"
    exit 1
fi

## Test 2: Validation
echo "Test 2: Configuration validation"
if validate*current*config; then
    echo "✓ Configuration validation passed"
else
    echo "✗ Configuration validation failed"
    exit 1
fi

## Test 3: App access
echo "Test 3: Application access"
app*types=$(get*app*types)
if [[ -n "$app*types" ]]; then
    echo "✓ Available apps: $app*types"
else
    echo "✗ No apps found"
    exit 1
fi

## Test 4: Build type access
echo "Test 4: Build type access"
build*types=$(get*build*types)
if [[ -n "$build*types" ]]; then
    echo "✓ Available build types: $build*types"
else
    echo "✗ No build types found"
    exit 1
fi

echo "All configuration tests passed!"
```text

## 📚 **Reference and Examples**

### **Configuration Function Reference**

#### **Core Functions**
```bash
## Configuration initialization
init*config                    # Initialize configuration system
load*config*yq                 # Load configuration using yq
load*config*basic              # Load configuration using fallback method

## Configuration validation
validate*current*config        # Validate current configuration
validate*app*type             # Validate application type
validate*build*type           # Validate build type

## Configuration access
get*app*types                 # Get all available application types
get*app*description           # Get application description
get*app*source*file           # Get application source file
get*app*category              # Get application category
get*build*types               # Get all available build types
get*idf*versions              # Get supported ESP-IDF versions
get*app*idf*versions          # Get ESP-IDF versions for specific app
get*build*types               # Get build types (with app override support)

## Configuration utilities
get*build*directory           # Get build directory path
get*project*name              # Get project name
get*build*directory*pattern   # Get build directory pattern
get*project*name*pattern      # Get project name pattern
```text

#### **Helper Functions**
```bash
## YAML processing
check*yq                      # Check if yq is available
run*yq                        # Execute yq with appropriate syntax
detect*yq*version             # Detect yq version and set syntax

## Validation helpers
is*valid*app*type             # Check if app type is valid
is*valid*build*type           # Check if build type is valid
is*valid*idf*version          # Check if ESP-IDF version is valid
is*valid*combination           # Check if app + build type + IDF version combination is valid

## Configuration utilities
get*featured*app*types        # Get featured application types
is*ci*enabled                # Check if app is CI-enabled
is*featured                  # Check if app is featured
```text

### **Configuration Examples**

#### **Minimal Configuration**
```yaml
## Minimal app*config.yml
metadata:
  default*app: "gpio*test"
  default*build*type: "Release"
  target: "esp32c6"

apps:
  gpio*test:
    source*file: "GpioComprehensiveTest.cpp"
    build*types: ["Debug", "Release"]
```text

#### **Standard Configuration**
```yaml
## Standard app*config.yml
metadata:
  default*app: "gpio*test"
  default*build*type: "Release"
  target: "esp32c6"
  idf*versions: ["release/v5.5"]
  description: "ESP32 HardFOC Interface Wrapper"
  version: "2.1.0"

apps:
  gpio*test:
    description: "GPIO testing application"
    source*file: "GpioComprehensiveTest.cpp"
    category: "peripheral"
    build*types: ["Debug", "Release"]
    idf*versions: ["release/v5.5"]
    ci*enabled: true
    featured: true

build*config:
  build*types:
    Debug:
      cmake*build*type: "Debug"
      optimization: "-O0"
      debug*level: "-g3"
    Release:
      cmake*build*type: "Release"
      optimization: "-O2"
      debug*level: "-g"
```text

#### **Advanced Configuration**
```yaml
## Advanced app*config.yml with all features
metadata:
  default*app: "gpio*test"
  default*build*type: "Release"
  target: "esp32c6"
  idf*versions: ["release/v5.5", "release/v5.4"]
  description: "ESP32 HardFOC Interface Wrapper - Advanced Configuration"
  version: "2.1.0"
  last*updated: "2025-01-15"
  maintainer: "HardFOC Team"
  repository: "https://github.com/example/hf-internal-interface-wrap"

apps:
  gpio*test:
    description: "Comprehensive GPIO testing and validation application"
    source*file: "GpioComprehensiveTest.cpp"
    category: "peripheral"
    build*types: ["Debug", "Release", "RelWithDebInfo"]
    idf*versions: ["release/v5.5", "release/v5.4"]
    ci*enabled: true
    featured: true
    dependencies: ["gpio*driver", "common*utils"]
    tags: ["peripheral", "gpio", "testing", "validation"]
    test*timeout: 300
    memory*requirements: "2MB"
    
  adc*test:
    description: "ADC peripheral testing and calibration application"
    source*file: "AdcComprehensiveTest.cpp"
    category: "peripheral"
    build*types: ["Debug", "Release"]
    idf*versions: ["release/v5.5"]
    ci*enabled: true
    featured: false
    dependencies: ["adc*driver", "gpio*driver", "calibration"]
    tags: ["peripheral", "adc", "analog", "calibration"]
    test*timeout: 600
    memory*requirements: "3MB"

build*config:
  build*types:
    Debug:
      description: "Debug build with maximum debugging information"
      cmake*build*type: "Debug"
      optimization: "-O0"
      debug*level: "-g3"
      defines: ["DEBUG", "VERBOSE*LOGGING", "ASSERTIONS*ENABLED"]
      assertions: true
      logging*level: "DEBUG"
      stack*usage: true
      sanitizers: ["address", "undefined"]
      
    Release:
      description: "Production-ready optimized build"
      cmake*build*type: "Release"
      optimization: "-O2"
      debug*level: "-g"
      defines: ["NDEBUG", "PRODUCTION*BUILD"]
      assertions: false
      logging*level: "INFO"
      stack*usage: false
      sanitizers: []
      
    RelWithDebInfo:
      description: "Release build with debug information for profiling"
      cmake*build*type: "RelWithDebInfo"
      optimization: "-O2"
      debug*level: "-g"
      defines: ["NDEBUG", "PROFILING*ENABLED"]
      assertions: false
      logging*level: "INFO"
      stack*usage: false
      sanitizers: []

  build*directory*pattern: "build*{app*type}*{build*type}*{idf*version}"
  project*name*pattern: "esp32*{app*type}*app"
  
  ccache*enabled: true
  parallel*builds: true
  incremental*builds: true
  size*analysis: true
  dependency*checking: true
  warning*as*errors: false

flash*config:
  auto*detect*ports: true
  port*scan*timeout: 5
  port*test*timeout: 3
  flash*mode: "dio"
  flash*freq: "80m"
  flash*size: "4MB"
  monitor*baud: 115200
  auto*logging: true
  log*rotation: true
  max*log*files: 50
  log*retention*days: 30

system*config:
  platforms: ["linux", "macos"]
  shell: "bash"
  min*bash*version: "4.0"
  python*version: "3.6+"
  required*packages: ["PyYAML"]
  required*tools: ["git", "cmake", "ninja", "ccache"]
  optional*tools: ["yq", "screen", "tmux"]
  cache*directories: ["$HOME/.ccache", "$HOME/.espressif", "$HOME/.cache/pip"]
```text

## 🚀 **Enhanced Functionality**

#### **App-Specific Overrides**
- **Smart Build Type Handling**: `get*build*types(app*type)` now checks app-specific overrides first
- **Version-Aware Validation**: Functions now understand the relationship between IDF versions and build types
- **Intelligent Fallbacks**: When app overrides aren't specified, functions fall back to metadata defaults

#### **🆕 Enhanced Validation Functions**
- **`is*valid*build*type(build*type, app*type, idf*version)`**: Comprehensive validation with app and version context
- **`is*valid*combination(app*type, build*type, idf*version)`**: Single function for complete combination validation
- **`get*app*build*types*for*idf*version(app*type, idf*version)`**: Get build types for specific app-IDF combinations

#### **CI Pipeline Optimization**
- **Robust Combination Validation**: Prevents invalid app + build type + IDF version combinations
- **Smart Error Messages**: Clear guidance on what combinations are allowed
- **Fallback Parsing**: Works reliably even when `yq` is not available in CI environments

### **Enhanced Function Examples**

#### **Smart Build Type Retrieval**
```bash
## Before: Only global build types
build*types=$(get*build*types)  # Returns: Debug Release

## After: App-specific with fallback
build*types=$(get*build*types)                    # Global: Debug Release
app*build*types=$(get*build*types "gpio*test")   # App-specific: Debug Release
```text

#### **Comprehensive Validation**
```bash
## Before: Separate validation functions
validate*app*type "gpio*test"
validate*build*type "Release"
validate*app*idf*version "gpio*test" "release/v5.5"

## After: Single comprehensive validation
if is*valid*combination "gpio*test" "Release" "release/v5.5"; then
    echo "Valid combination for CI pipeline"
fi
```text

#### **Version-Aware Build Type Validation**
```bash
## Enhanced validation with context
if is*valid*build*type "Release" "gpio*test" "release/v5.5"; then
    echo "Valid build type for app and IDF version"
fi

## Get version-specific build types
version*build*types=$(get*build*types*for*idf*version "release/v5.5")
echo "Build types for v5.5: $version*build*types"
```text

### **Migration Guide**

#### **Functions Removed in Version 2.0**
- `get*app*build*types()` → Use `get*build*types(app*type)` instead
- `validate*app*build*type()` → Use `is*valid*build*type(build*type, app*type)` instead
- `validate*app*idf*version()` → Use `is*valid*combination(app*type, build*type, idf*version)` instead
- `get*idf*version*smart()` → Use `get*idf*version(app*type)` + `is*valid*combination()` instead

#### **Updated Function Signatures**
```bash
## Before
get*build*types()                    # Only global
is*valid*build*type(build*type)      # Basic validation

## After
get*build*types([app*type])          # Global or app-specific
is*valid*build*type(build*type, [app*type], [idf*version])  # Comprehensive validation
```text

---

### **Best Practices**

#### **1. Configuration Structure**
- Use consistent naming conventions
- Group related configuration items
- Provide clear descriptions for all items
- Use appropriate data types and constraints

#### **2. Validation and Error Handling**
- Always validate configuration before use
- Provide clear error messages
- Implement graceful fallbacks
- Test configuration with various scenarios

#### **3. Documentation and Maintenance**
- Document all configuration options
- Keep configuration examples updated
- Version control configuration changes
- Regular configuration validation

#### **4. Performance and Optimization**
- Use efficient parsing methods
- Implement configuration caching
- Minimize configuration file size
- Optimize validation processes

---

**Navigation**: [← Previous: Flash System](README*FLASH*SYSTEM.md) | [Back to Scripts](../README.md)
| [Next: Logging System →](README*LOGGING*SYSTEM.md)
