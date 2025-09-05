# ESP32 HardFOC Interface Wrapper - Centralized Configuration Guide

This document provides comprehensive documentation for the centralized configuration system,
including the `app*config.yml` file structure, configuration loading mechanisms, and integration
with the ESP32 development workflow.

---

**Navigation**: [← Previous: Port Detection](README*PORT*DETECTION.md) | [Back to
Scripts](../README.md) | [Next: Scripts Overview →](README*SCRIPTS*OVERVIEW.md)

---

## 📋 **Table of Contents**

- [📋 Overview](#-overview)
- [🏗️ Architecture and Design](#️-architecture-and-design)
- [⚙️ Configuration File Structure](#️-configuration-file-structure)
- [🔧 Configuration Loading and Validation](#️-configuration-loading-and-validation)
- [🌍 Environment Variable Overrides](#️-environment-variable-overrides)
- [🚀 Usage Examples and Patterns](#️-usage-examples-and-patterns)
- [🔍 Troubleshooting and Debugging](#️-usage-examples-and-patterns)
- [📚 Reference and Examples](#️-reference-and-examples)

## 📋 **Overview**

The centralized configuration system provides a unified approach to managing ESP32 project settings,
application configurations, and build parameters.
The system uses a single `app*config.yml` file to define all project aspects,
enabling consistent configuration management across different scripts and development environments.

### **Core Features**
- **Unified Configuration**: Single YAML file for all project settings
- **Environment Overrides**: Dynamic configuration through environment variables
- **Validation System**: Automatic configuration integrity checking
- **Cross-Platform**: Consistent behavior across different operating systems
- **Integration Ready**: Seamless integration with build and flash systems

### **Key Capabilities**
- Centralized application and build configuration management
- Environment variable-based configuration overrides
- Automatic configuration validation and error reporting
- Integration with CMake and CI/CD systems
- Support for multiple ESP-IDF versions and targets

## 🏗️ **Architecture and Design**

### **System Architecture**
```yaml
Configuration System → YAML Parser → Validation Engine → Script Integration → Environment Overrides
        ↓                ↓              ↓                ↓                ↓
   app*config.yml    yq/grep/sed    Schema Check    Script Loading    Env Var Priority
```text

### **Component Interaction**
- **Configuration File**: Central YAML file with all project settings
- **Loading Engine**: Primary (yq) and fallback (grep/sed) loading mechanisms
- **Validation System**: Configuration integrity and schema validation
- **Script Integration**: Configuration access across all scripts
- **Environment Overrides**: Dynamic configuration modification

### **Design Principles**
- **Single Source of Truth**: All configuration in one file
- **Environment Flexibility**: Dynamic configuration through environment variables
- **Validation First**: Automatic configuration integrity checking
- **Performance Optimized**: Efficient loading and caching mechanisms
- **Integration Ready**: Seamless integration with development workflows

## ⚙️ **Configuration File Structure**

### **File Location and Naming**
```bash
## Configuration file location
examples/esp32/app*config.yml

## Alternative locations (in order of precedence)
./app*config.yml                    # Current directory
../app*config.yml                   # Parent directory
$HOME/.esp32*config.yml            # User home directory
$ESP32*CONFIG*PATH/app*config.yml  # Custom path via environment variable
```text

### **Configuration Schema**

#### **Metadata Section**
```yaml
metadata:
  project*name: "ESP32 HardFOC Interface Wrapper"
  version: "2.0.0"
  description: "Comprehensive ESP32 development framework"
  author: "HardFOC Development Team"
  last*updated: "2025-01-27"
  supported*platforms:
    - "Linux (Ubuntu 20.04+)"
    - "macOS (10.15+)"
    - "Windows (WSL2)"
  requirements:
    esp*idf*min: "v5.0.0"
    esp*idf*recommended: "v5.5"
    python*min: "3.8"
    bash*min: "4.0"
```text

#### **Applications Section**
```yaml
apps:
  gpio*test:
    description: "GPIO peripheral comprehensive testing"
    source*file: "GpioComprehensiveTest.cpp"
    featured: true
    build*types:
      - "Debug"
      - "Release"
    target: "esp32c6"
    dependencies:
      - "ESP-IDF v5.5+"
      - "GPIO hardware interface"
    ci*config:
      enabled: true
      timeout: 300
      retries: 3
    flash*config:
      port*auto*detect: true
      baud*rate: 921600
      flash*mode: "dio"
      flash*freq: "80m"
      flash*size: "4MB"
    monitor*config:
      baud*rate: 115200
      log*output: true
      log*file: "gpio*test*monitor.log"

  adc*test:
    description: "ADC peripheral comprehensive testing"
    source*file: "AdcComprehensiveTest.cpp"
    featured: true
    build*types:
      - "Debug"
      - "Release"
    target: "esp32c6"
    dependencies:
      - "ESP-IDF v5.5+"
      - "ADC hardware interface"
    ci*config:
      enabled: true
      timeout: 300
      retries: 3
    flash*config:
      port*auto*detect: true
      baud*rate: 921600
      flash*mode: "dio"
      flash*freq: "80m"
      flash*size: "4MB"
    monitor*config:
      baud*rate: 115200
      log*output: true
      log*file: "adc*test*monitor.log"
```text

#### **Build Configuration Section**
```yaml
build*config:
  default*build*type: "Release"
  default*target: "esp32c6"
  supported*build*types:
    - "Debug"
    - "Release"
  supported*targets:
    - "esp32"
    - "esp32c3"
    - "esp32c6"
    - "esp32s3"
  
  # ESP-IDF version management
  idf*versions:
    esp32: "v5.0.0"
    esp32c3: "v5.0.0"
    esp32c6: "v5.5"
    esp32s3: "v5.0.0"
  
  # Build optimization
  ccache*enabled: true
  ccache*size: "2G"
  parallel*builds: 4
  incremental*builds: true
  
  # Build validation
  assertions: true
  stack*usage: true
  size*analysis: true
  
  # Debug and development
  logging*level: "INFO"
  sanitizers:
    - "address"
    - "undefined"
  debug*symbols: true
  
  # Performance monitoring
  build*timeout: 1800
  memory*limit: "4G"
  cpu*limit: 4
```text

#### **Flash Configuration Section**
```yaml
flash*config:
  # Port detection and management
  port*auto*detect: true
  port*validation: true
  port*timeout: 30
  
  # Flash parameters
  default*baud*rate: 921600
  default*flash*mode: "dio"
  default*flash*freq: "80m"
  default*flash*size: "4MB"
  
  # Flash validation
  flash*verification: true
  flash*checksum: true
  flash*backup: false
  
  # Error handling
  retry*attempts: 3
  retry*delay: 2
  error*recovery: true
  
  # Logging and monitoring
  flash*logging: true
  flash*log*file: "flash*operation.log"
  flash*verbose: false
```text

#### **System Configuration Section**
```yaml
system*config:
  # Development environment
  development*mode: true
  debug*mode: false
  verbose*output: false
  
  # Logging configuration
  log*directory: "logs"
  log*retention*days: 30
  log*rotation*size: "100M"
  log*compression: true
  
  # Performance settings
  max*parallel*jobs: 4
  memory*limit: "4G"
  cpu*limit: 4
  
  # Security and permissions
  strict*permissions: false
  auto*fix*permissions: true
  user*group*management: true
  
  # Integration settings
  cmake*integration: true
  ci*cd*integration: true
  ide*integration: true
```yaml

## 🔧 **Configuration Loading and Validation**

### **Configuration Loading Process**

#### **Primary Loading Method (yq)**
The system primarily uses `yq` for YAML processing:

```bash
## Primary loading function
load*config*yq() {
    local config*file="$1"
    local query="$2"
    
    if command -v yq &> /dev/null; then
        yq eval "$query" "$config*file" 2>/dev/null
        return $?
    else
        return 1
    fi
}

## Usage examples
load*config*yq "app*config.yml" ".metadata.project*name"
load*config*yq "app*config.yml" ".apps.gpio*test.description"
load*config*yq "app*config.yml" ".build*config.default*build*type"
```text

#### **Fallback Loading Method (grep/sed)**
When `yq` is unavailable, the system falls back to `grep` and `sed`:

```bash
## Fallback loading function
load*config*fallback() {
    local config*file="$1"
    local key="$2"
    
    # Convert YAML path to grep pattern
    local pattern=$(echo "$key" | sed 's/\./\\n/g')
    
    # Extract value using grep and sed
    grep -A1 -B1 "$pattern" "$config*file" | \
    sed -n '/^[[:space:]]*[^[:space:]#]/p' | \
    sed 's/^[[:space:]]*//' | \
    sed 's/[[:space:]]*:[[:space:]]*/:/' | \
    sed 's/^[^:]*://'
}

## Usage examples
load*config*fallback "app*config.yml" "metadata.project*name"
load*config*fallback "app*config.yml" "apps.gpio*test.description"
```text

#### **Configuration Loading Priority**
```bash
## Loading priority order
1. Environment variable override
2. Primary method (yq)
3. Fallback method (grep/sed)
4. Default value
5. Error (if required)

## Implementation
load*config() {
    local key="$1"
    local default*value="$2"
    local required="${3:-false}"
    
    # Check environment variable first
    local env*var=$(echo "$key" | tr '[:lower:]' '[:upper:]' | tr '.' '*')
    if [ -n "${!env*var}" ]; then
        echo "${!env*var}"
        return 0
    fi
    
    # Try primary method
    local value=$(load*config*yq "app*config.yml" ".$key")
    if [ $? -eq 0 ] && [ -n "$value" ]; then
        echo "$value"
        return 0
    fi
    
    # Try fallback method
    value=$(load*config*fallback "app*config.yml" "$key")
    if [ $? -eq 0 ] && [ -n "$value" ]; then
        echo "$value"
        return 0
    fi
    
    # Use default value
    if [ -n "$default*value" ]; then
        echo "$default*value"
        return 0
    fi
    
    # Handle required configuration
    if [ "$required" = "true" ]; then
        echo "ERROR: Required configuration '$key' not found" >&2
        return 1
    fi
    
    return 0
}
```text

### **Configuration Validation**

#### **Schema Validation**
```bash
## Basic schema validation
validate*config*schema() {
    local config*file="$1"
    local errors=0
    
    # Check required sections
    local required*sections=("metadata" "apps" "build*config")
    for section in "${required*sections[@]}"; do
        if ! load*config "$section" "" true >/dev/null 2>&1; then
            echo "ERROR: Missing required section '$section'" >&2
            ((errors++))
        fi
    done
    
    # Check required metadata fields
    local required*metadata=("project*name" "version")
    for field in "${required*metadata[@]}"; do
        if ! load*config "metadata.$field" "" true >/dev/null 2>&1; then
            echo "ERROR: Missing required metadata field '$field'" >&2
            ((errors++))
        fi
    done
    
    # Check application configurations
    local apps=$(load*config "apps" "" false)
    if [ -n "$apps" ]; then
        for app in $(echo "$apps" | tr ' ' '\n'); do
            validate*app*config "$app"
        done
    fi
    
    return $errors
}
```text

#### **Application Configuration Validation**
```bash
## Validate individual application configuration
validate*app*config() {
    local app*name="$1"
    local errors=0
    
    # Check required app fields
    local required*fields=("description" "source*file" "build*types" "target")
    for field in "${required*fields[@]}"; do
        if ! load*config "apps.$app*name.$field" "" true >/dev/null 2>&1; then
            echo "ERROR: App '$app*name' missing required field '$field'" >&2
            ((errors++))
        fi
    done
    
    # Validate source file existence
    local source*file=$(load*config "apps.$app*name.source*file" "" false)
    if [ -n "$source*file" ]; then
        local full*path="examples/esp32/main/$source*file"
        if [ ! -f "$full*path" ]; then
            echo "ERROR: App '$app*name' source file not found: $full*path" >&2
            ((errors++))
        fi
    fi
    
    # Validate build types
    local build*types=$(load*config "apps.$app*name.build*types" "" false)
    if [ -n "$build*types" ]; then
        for build*type in $(echo "$build*types" | tr ' ' '\n'); do
            if [[ ! "$build*type" =~ ^(Debug|Release)$ ]]; then
                echo "ERROR: App '$app*name' invalid build type: $build*type" >&2
                ((errors++))
            fi
        done
    fi
    
    return $errors
}
```text

#### **Configuration Integrity Checking**
```bash
## Comprehensive configuration validation
validate*config*integrity() {
    local config*file="$1"
    local errors=0
    
    echo "Validating configuration file: $config*file"
    
    # Check file existence
    if [ ! -f "$config*file" ]; then
        echo "ERROR: Configuration file not found: $config*file" >&2
        return 1
    fi
    
    # Check YAML syntax (if yq available)
    if command -v yq &> /dev/null; then
        if ! yq eval "." "$config*file" >/dev/null 2>&1; then
            echo "ERROR: Invalid YAML syntax in configuration file" >&2
            ((errors++))
        fi
    fi
    
    # Validate schema
    if ! validate*config*schema "$config*file"; then
        ((errors++))
    fi
    
    # Check for configuration conflicts
    if ! check*config*conflicts "$config*file"; then
        ((errors++))
    fi
    
    if [ $errors -eq 0 ]; then
        echo "✓ Configuration validation passed"
        return 0
    else
        echo "✗ Configuration validation failed with $errors error(s)"
        return 1
    fi
}
```text

## 🌍 **Environment Variable Overrides**

### **Override Priority System**

#### **Priority Order**
```bash
## Configuration override priority (highest to lowest)
1. Command line arguments
2. Environment variables
3. Configuration file values
4. Default values
5. System defaults

## Implementation
get*config*value() {
    local key="$1"
    local default*value="$2"
    
    # Check command line arguments first
    local cmd*arg=$(get*cmd*line*arg "$key")
    if [ -n "$cmd*arg" ]; then
        echo "$cmd*arg"
        return 0
    fi
    
    # Check environment variables
    local env*var=$(get*env*var "$key")
    if [ -n "$env*var" ]; then
        echo "$env*var"
        return 0
    fi
    
    # Load from configuration file
    local config*value=$(load*config "$key" "" false)
    if [ -n "$config*value" ]; then
        echo "$config*value"
        return 0
    fi
    
    # Use default value
    echo "$default*value"
}
```text

#### **Environment Variable Naming Convention**
```bash
## Environment variable naming pattern
## Convert YAML path to environment variable name
## Format: ESP32*<SECTION>*<SUBSECTION>*<FIELD>

## Examples:
ESP32*METADATA*PROJECT*NAME="My Project"
ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE="Debug"
ESP32*APPS*GPIO*TEST*TARGET="esp32s3"
ESP32*FLASH*CONFIG*PORT*AUTO*DETECT="false"
ESP32*SYSTEM*CONFIG*DEBUG*MODE="true"

## Conversion function
yaml*path*to*env*var() {
    local yaml*path="$1"
    echo "$yaml*path" | \
    tr '[:lower:]' '[:upper:]' | \
    tr '.' '*' | \
    sed 's/^/ESP32*/'
}

## Usage
env*var=$(yaml*path*to*env*var "build*config.default*build*type")
## Result: ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE
```text

### **Supported Environment Variables**

#### **Build Configuration Overrides**
```bash
## Build type and target
export ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE="Debug"
export ESP32*BUILD*CONFIG*DEFAULT*TARGET="esp32s3"
export ESP32*BUILD*CONFIG*CCACHE*ENABLED="true"
export ESP32*BUILD*CONFIG*PARALLEL*BUILDS="8"
export ESP32*BUILD*CONFIG*BUILD*TIMEOUT="3600"

## ESP-IDF version overrides
export ESP32*BUILD*CONFIG*IDF*VERSIONS*ESP32="v5.0.0"
export ESP32*BUILD*CONFIG*IDF*VERSIONS*ESP32C6="v5.5"
export ESP32*BUILD*CONFIG*IDF*VERSIONS*ESP32S3="v5.0.0"

## Build optimization
export ESP32*BUILD*CONFIG*CCACHE*SIZE="4G"
export ESP32*BUILD*CONFIG*INCREMENTAL*BUILDS="true"
export ESP32*BUILD*CONFIG*DEBUG*SYMBOLS="true"
```text

#### **Application Configuration Overrides**
```bash
## Application-specific overrides
export ESP32*APPS*GPIO*TEST*TARGET="esp32s3"
export ESP32*APPS*GPIO*TEST*BUILD*TYPES="Debug Release"
export ESP32*APPS*ADC*TEST*FLASH*CONFIG*BAUD*RATE="460800"
export ESP32*APPS*UART*TEST*MONITOR*CONFIG*BAUD*RATE="230400"

## Application dependencies
export ESP32*APPS*GPIO*TEST*DEPENDENCIES="ESP-IDF v5.5+,GPIO hardware"
export ESP32*APPS*ADC*TEST*DEPENDENCIES="ESP-IDF v5.5+,ADC hardware"
```text

#### **Flash Configuration Overrides**
```bash
## Flash parameters
export ESP32*FLASH*CONFIG*PORT*AUTO*DETECT="false"
export ESP32*FLASH*CONFIG*DEFAULT*BAUD*RATE="460800"
export ESP32*FLASH*CONFIG*DEFAULT*FLASH*MODE="qio"
export ESP32*FLASH*CONFIG*DEFAULT*FLASH*FREQ="40m"
export ESP32*FLASH*CONFIG*DEFAULT*FLASH*SIZE="8MB"

## Flash validation
export ESP32*FLASH*CONFIG*FLASH*VERIFICATION="true"
export ESP32*FLASH*CONFIG*FLASH*CHECKSUM="true"
export ESP32*FLASH*CONFIG*RETRY*ATTEMPTS="5"
```text

#### **System Configuration Overrides**
```bash
## System settings
export ESP32*SYSTEM*CONFIG*DEVELOPMENT*MODE="true"
export ESP32*SYSTEM*CONFIG*DEBUG*MODE="true"
export ESP32*SYSTEM*CONFIG*VERBOSE*OUTPUT="true"

## Logging configuration
export ESP32*SYSTEM*CONFIG*LOG*DIRECTORY="/tmp/esp32*logs"
export ESP32*SYSTEM*CONFIG*LOG*RETENTION*DAYS="7"
export ESP32*SYSTEM*CONFIG*LOG*ROTATION*SIZE="50M"

## Performance settings
export ESP32*SYSTEM*CONFIG*MAX*PARALLEL*JOBS="8"
export ESP32*SYSTEM*CONFIG*MEMORY*LIMIT="8G"
export ESP32*SYSTEM*CONFIG*CPU*LIMIT="8"
```text

### **Dynamic Configuration Updates**

#### **Runtime Configuration Modification**
```bash
## Update configuration at runtime
update*config*runtime() {
    local key="$1"
    local value="$2"
    
    # Set environment variable for immediate effect
    local env*var=$(yaml*path*to*env*var "$key")
    export "$env*var"="$value"
    
    echo "Updated configuration: $key = $value"
}

## Usage examples
update*config*runtime "build*config.default*build*type" "Debug"
update*config*runtime "flash*config.port*auto*detect" "false"
update*config*runtime "system*config.debug*mode" "true"
```text

#### **Configuration Reloading**
```bash
## Reload configuration from file
reload*config() {
    local config*file="$1"
    
    echo "Reloading configuration from: $config*file"
    
    # Validate configuration file
    if ! validate*config*integrity "$config*file"; then
        echo "ERROR: Configuration validation failed" >&2
        return 1
    fi
    
    # Clear cached configuration
    unset CONFIG*CACHE
    
    # Reload configuration
    source*config*file "$config*file"
    
    echo "Configuration reloaded successfully"
}
```text

## 🚀 **Usage Examples and Patterns**

### **Basic Configuration Usage**

#### **1. Loading Configuration Values**
```bash
## Load basic configuration values
project*name=$(load*config "metadata.project*name" "Unknown Project")
version=$(load*config "metadata.version" "1.0.0")
default*build*type=$(load*config "build*config.default*build*type" "Release")

## Load application configuration
gpio*description=$(load*config "apps.gpio*test.description" "")
gpio*target=$(load*config "apps.gpio*test.target" "esp32c6")
gpio*build*types=$(load*config "apps.gpio*test.build*types" "Debug Release")
```text

#### **2. Configuration Validation**
```bash
## Validate configuration file
if ! validate*config*integrity "app*config.yml"; then
    echo "Configuration validation failed"
    exit 1
fi

## Validate specific application
if ! validate*app*config "gpio*test"; then
    echo "GPIO test configuration validation failed"
    exit 1
fi
```text

#### **3. Environment Variable Overrides**
```bash
## Override build configuration
export ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE="Debug"
export ESP32*BUILD*CONFIG*DEFAULT*TARGET="esp32s3"

## Override application configuration
export ESP32*APPS*GPIO*TEST*TARGET="esp32s3"
export ESP32*APPS*GPIO*TEST*BUILD*TYPES="Debug"

## Override flash configuration
export ESP32*FLASH*CONFIG*PORT*AUTO*DETECT="false"
export ESP32*FLASH*CONFIG*DEFAULT*BAUD*RATE="460800"
```text

### **Advanced Configuration Patterns**

#### **1. Configuration-Driven Scripts**
```bash
#!/bin/bash
## Configuration-driven build script

## Load configuration
source ./scripts/config*loader.sh

## Get build parameters
build*type=$(load*config "build*config.default*build*type" "Release")
target=$(load*config "build*config.default*target" "esp32c6")
parallel*jobs=$(load*config "build*config.parallel*builds" "4")

## Validate configuration
if ! validate*config*integrity "app*config.yml"; then
    echo "Configuration validation failed"
    exit 1
fi

## Execute build with configuration
idf.py set-target "$target"
idf.py build -b "$build*type" -j "$parallel*jobs"
```text

#### **2. Dynamic Configuration Updates**
```bash
#!/bin/bash
## Dynamic configuration management

## Update configuration based on environment
if [ "$CI" = "true" ]; then
    update*config*runtime "system*config.debug*mode" "false"
    update*config*runtime "build*config.parallel*builds" "8"
    update*config*runtime "flash*config.port*auto*detect" "false"
fi

## Update configuration based on user preferences
if [ "$USER*PREFERS*DEBUG" = "true" ]; then
    update*config*runtime "build*config.default*build*type" "Debug"
    update*config*runtime "system*config.verbose*output" "true"
fi
```text

#### **3. Configuration Testing and Validation**
```bash
#!/bin/bash
## Configuration testing script

## Test configuration loading
echo "Testing configuration loading..."

## Test metadata loading
project*name=$(load*config "metadata.project*name" "")
if [ -n "$project*name" ]; then
    echo "✓ Project name loaded: $project*name"
else
    echo "✗ Failed to load project name"
fi

## Test application loading
apps=$(load*config "apps" "")
if [ -n "$apps" ]; then
    echo "✓ Applications loaded: $apps"
else
    echo "✗ Failed to load applications"
fi

## Test build configuration
build*type=$(load*config "build*config.default*build*type" "")
if [ -n "$build*type" ]; then
    echo "✓ Build type loaded: $build*type"
else
    echo "✗ Failed to load build type"
fi
```text

### **Integration Examples**

#### **1. CMake Integration**
```cmake
## CMakeLists.txt configuration integration
cmake*minimum*required(VERSION 3.16)

## Load configuration values
execute*process(
    COMMAND bash -c "source ${CMAKE*SOURCE*DIR}/scripts/config*loader.sh && load*config 'build*config.default*target' 'esp32c6'"
    OUTPUT*VARIABLE ESP32*TARGET
    OUTPUT*STRIP*TRAILING*WHITESPACE
)

execute*process(
    COMMAND bash -c "source ${CMAKE*SOURCE*DIR}/scripts/config*loader.sh && load*config 'build*config.default*build*type' 'Release'"
    OUTPUT*VARIABLE ESP32*BUILD*TYPE
    OUTPUT*STRIP*TRAILING*WHITESPACE
)

## Use configuration values
set(IDF*TARGET ${ESP32*TARGET})
set(CONFIG*DEFAULT*BUILD*TYPE ${ESP32*BUILD*TYPE})

## Configuration validation
add*custom*target(validate*config
    COMMAND bash -c "cd ${CMAKE*SOURCE*DIR} && source scripts/config*loader.sh && validate*config*integrity app*config.yml"
    COMMENT "Validating configuration"
)
```text

#### **2. CI/CD Integration**
```yaml
## GitHub Actions configuration integration
- name: Validate Configuration
  run: |
    cd examples/esp32
    source scripts/config*loader.sh
    validate*config*integrity app*config.yml

- name: Load Build Configuration
  run: |
    cd examples/esp32
    source scripts/config*loader.sh
    echo "BUILD*TYPE=$(load*config 'build*config.default*build*type' 'Release')" >> $GITHUB*ENV
    echo "TARGET=$(load*config 'build*config.default*target' 'esp32c6')" >> $GITHUB*ENV

- name: Build Application
  run: |
    cd examples/esp32
    idf.py set-target ${{ env.TARGET }}
    idf.py build -b ${{ env.BUILD*TYPE }}
```text

#### **3. Development Environment Setup**
```bash
#!/bin/bash
## Development environment configuration

## Load configuration
source ./scripts/config*loader.sh

## Validate configuration
if ! validate*config*integrity "app*config.yml"; then
    echo "Configuration validation failed"
    exit 1
fi

## Setup environment based on configuration
target=$(load*config "build*config.default*target" "esp32c6")
build*type=$(load*config "build*config.default*build*type" "Release")
debug*mode=$(load*config "system*config.debug*mode" "false")

## Configure ESP-IDF
idf.py set-target "$target"

## Set environment variables
export IDF*TARGET="$target"
export CONFIG*DEFAULT*BUILD*TYPE="$build*type"

if [ "$debug*mode" = "true" ]; then
    export ESP32*DEBUG*MODE="true"
    export ESP32*VERBOSE*OUTPUT="true"
fi

echo "Development environment configured:"
echo "  Target: $target"
echo "  Build Type: $build*type"
echo "  Debug Mode: $debug*mode"
```text

## 🔍 **Troubleshooting and Debugging**

### **Common Configuration Issues**

#### **1. Configuration File Not Found**
**Problem**: Configuration file not found or inaccessible
**Symptoms**: "Configuration file not found" or "File not accessible" errors
**Solutions**:
```bash
## Check file existence
ls -la app*config.yml
ls -la examples/esp32/app*config.yml

## Check file permissions
ls -la app*config.yml
chmod 644 app*config.yml

## Check file path
pwd
find . -name "app*config.yml" -type f

## Verify file location
## Configuration file should be in examples/esp32/ directory
```yaml

#### **2. YAML Syntax Errors**
**Problem**: Invalid YAML syntax in configuration file
**Symptoms**: "Invalid YAML syntax" or parsing errors
**Solutions**:
```bash
## Validate YAML syntax with yq
yq eval "." app*config.yml

## Check for common YAML issues
## - Proper indentation (spaces, not tabs)
## - Valid key-value pairs
## - Proper list formatting
## - Valid string escaping

## Use YAML linter
yamllint app*config.yml

## Check for hidden characters
cat -A app*config.yml
```text

#### **3. Configuration Validation Failures**
**Problem**: Configuration validation fails during loading
**Symptoms**: "Configuration validation failed" or missing required fields
**Solutions**:
```bash
## Run validation manually
source ./scripts/config*loader.sh
validate*config*integrity app*config.yml

## Check specific validation errors
validate*config*schema app*config.yml
validate*app*config "gpio*test"

## Verify required fields
load*config "metadata.project*name" "" true
load*config "apps.gpio*test.description" "" true
load*config "build*config.default*build*type" "" true
```text

#### **4. Environment Variable Conflicts**
**Problem**: Environment variables conflict with configuration file values
**Symptoms**: Unexpected configuration values or override issues
**Solutions**:
```bash
## Check environment variables
env | grep ESP32

## Clear conflicting environment variables
unset ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE
unset ESP32*APPS*GPIO*TEST*TARGET

## Verify configuration loading priority
load*config "build*config.default*build*type" "Release"
update*config*runtime "build*config.default*build*type" "Debug"
load*config "build*config.default*build*type" "Release"
```text

### **Debug and Verbose Mode**

#### **Enabling Debug Output**
```bash
## Enable debug mode
export DEBUG=1
export VERBOSE=1
export CONFIG*DEBUG=1

## Run with debug output
source ./scripts/config*loader.sh
load*config "metadata.project*name" "" true

## Debug information available
- Configuration file loading process
- Environment variable processing
- Configuration validation details
- Error context and resolution
```text

#### **Configuration Testing**
```bash
## Test configuration loading
test*config*loading() {
    echo "Testing configuration loading..."
    
    # Test metadata
    local project*name=$(load*config "metadata.project*name" "" false)
    echo "Project name: $project*name"
    
    # Test applications
    local apps=$(load*config "apps" "" false)
    echo "Applications: $apps"
    
    # Test build configuration
    local build*type=$(load*config "build*config.default*build*type" "" false)
    echo "Build type: $build*type"
    
    # Test environment variable overrides
    export ESP32*BUILD*CONFIG*DEFAULT*BUILD*TYPE="Debug"
    local override*value=$(load*config "build*config.default*build*type" "" false)
    echo "Override value: $override*value"
}

## Run configuration test
test*config*loading
```text

## 📚 **Reference and Examples**

### **Configuration Function Reference**

#### **Core Loading Functions**
```bash
## Primary configuration loading
load*config <key> [default*value] [required]

## YAML-based loading
load*config*yq <config*file> <query>

## Fallback loading
load*config*fallback <config*file> <key>

## Configuration validation
validate*config*integrity <config*file>
validate*config*schema <config*file>
validate*app*config <app*name>
```text

#### **Environment Variable Functions**
```bash
## Environment variable management
yaml*path*to*env*var <yaml*path>
get*env*var <key>
update*config*runtime <key> <value>

## Configuration reloading
reload*config <config*file>
source*config*file <config*file>
```text

#### **Utility Functions**
```bash
## Configuration utilities
get*config*value <key> [default*value]
check*config*conflicts <config*file>
get*cmd*line*arg <key>
```text

### **Configuration Examples**

#### **Minimal Configuration**
```yaml
metadata:
  project*name: "ESP32 Example"
  version: "1.0.0"

apps:
  basic*test:
    description: "Basic ESP32 test"
    source*file: "BasicTest.cpp"
    build*types: ["Debug", "Release"]
    target: "esp32c6"

build*config:
  default*build*type: "Release"
  default*target: "esp32c6"
  supported*build*types: ["Debug", "Release"]
  supported*targets: ["esp32", "esp32c6"]
```text

#### **Standard Configuration**
```yaml
metadata:
  project*name: "ESP32 HardFOC Interface Wrapper"
  version: "2.0.0"
  description: "Comprehensive ESP32 development framework"
  author: "HardFOC Development Team"
  last*updated: "2025-01-27"
  supported*platforms: ["Linux (Ubuntu 20.04+)", "macOS (10.15+)", "Windows (WSL2)"]
  requirements:
    esp*idf*min: "v5.0.0"
    esp*idf*recommended: "v5.5.0"
    python*min: "3.8"
    bash*min: "4.0"

apps:
  gpio*test:
    description: "GPIO peripheral comprehensive testing"
    source*file: "GpioComprehensiveTest.cpp"
    featured: true
    build*types: ["Debug", "Release"]
    target: "esp32c6"
    dependencies: ["ESP-IDF v5.5+", "GPIO hardware interface"]
    ci*config:
      enabled: true
      timeout: 300
      retries: 3
    flash*config:
      port*auto*detect: true
      baud*rate: 921600
      flash*mode: "dio"
      flash*freq: "80m"
      flash*size: "4MB"
    monitor*config:
      baud*rate: 115200
      log*output: true
      log*file: "gpio*test*monitor.log"

build*config:
  default*build*type: "Release"
  default*target: "esp32c6"
  supported*build*types: ["Debug", "Release"]
  supported*targets: ["esp32", "esp32c3", "esp32c6", "esp32s3"]
  idf*versions:
    esp32: "v5.0.0"
    esp32c3: "v5.0.0"
    esp32c6: "v5.5"
    esp32s3: "v5.0.0"
  ccache*enabled: true
  ccache*size: "2G"
  parallel*builds: 4
  incremental*builds: true
  assertions: true
  stack*usage: true
  size*analysis: true
  logging*level: "INFO"
  sanitizers: ["address", "undefined"]
  debug*symbols: true
  build*timeout: 1800
  memory*limit: "4G"
  cpu*limit: 4

flash*config:
  port*auto*detect: true
  port*validation: true
  port*timeout: 30
  default*baud*rate: 921600
  default*flash*mode: "dio"
  default*flash*freq: "80m"
  default*flash*size: "4MB"
  flash*verification: true
  flash*checksum: true
  flash*backup: false
  retry*attempts: 3
  retry*delay: 2
  error*recovery: true
  flash*logging: true
  flash*log*file: "flash*operation.log"
  flash*verbose: false

system*config:
  development*mode: true
  debug*mode: false
  verbose*output: false
  log*directory: "logs"
  log*retention*days: 30
  log*rotation*size: "100M"
  log*compression: true
  max*parallel*jobs: 4
  memory*limit: "4G"
  cpu*limit: 4
  strict*permissions: false
  auto*fix*permissions: true
  user*group*management: true
  cmake*integration: true
  ci*cd*integration: true
  ide*integration: true
```text

#### **Advanced Configuration**
```yaml
metadata:
  project*name: "ESP32 Advanced Development Framework"
  version: "3.0.0"
  description: "Advanced ESP32 development framework with CI/CD integration"
  author: "Advanced Development Team"
  last*updated: "2025-01-27"
  supported*platforms: ["Linux (Ubuntu 20.04+)", "macOS (10.15+)", "Docker"]
  requirements:
    esp*idf*min: "v5.0.0"
    esp*idf*recommended: "v5.5"
    python*min: "3.8"
    bash*min: "4.0"
    docker*min: "20.10.0"
  ci*cd:
    github*actions: true
    gitlab*ci: true
    jenkins: true
    docker: true

apps:
  comprehensive*test:
    description: "Comprehensive ESP32 testing suite"
    source*file: "ComprehensiveTest.cpp"
    featured: true
    build*types: ["Debug", "Release", "Profiling"]
    target: "esp32c6"
    dependencies: ["ESP-IDF v5.5+", "All hardware interfaces", "Testing framework"]
    ci*config:
      enabled: true
      timeout: 600
      retries: 5
      parallel*execution: true
      resource*requirements:
        memory: "8G"
        cpu: "8"
        storage: "20G"
    flash*config:
      port*auto*detect: true
      baud*rate: 921600
      flash*mode: "dio"
      flash*freq: "80m"
      flash*size: "8MB"
      flash*verification: true
      flash*checksum: true
      flash*backup: true
      backup*directory: "flash*backups"
    monitor*config:
      baud*rate: 115200
      log*output: true
      log*file: "comprehensive*test*monitor.log"
      log*rotation: true
      log*compression: true
    testing*config:
      unit*tests: true
      integration*tests: true
      performance*tests: true
      stress*tests: true
      test*timeout: 300
      test*retries: 3

build*config:
  default*build*type: "Release"
  default*target: "esp32c6"
  supported*build*types: ["Debug", "Release", "Profiling", "Production"]
  supported*targets: ["esp32", "esp32c3", "esp32c6", "esp32s3", "esp32h2"]
  idf*versions:
    esp32: "v5.0.0"
    esp32c3: "v5.0.0"
    esp32c6: "v5.5"
    esp32s3: "v5.0.0"
    esp32h2: "v5.5"
  ccache*enabled: true
  ccache*size: "4G"
  ccache*compression: true
  parallel*builds: 8
  incremental*builds: true
  distributed*builds: true
  build*farm:
    enabled: true
    nodes: 4
    load*balancing: true
  assertions: true
  stack*usage: true
  size*analysis: true
  memory*analysis: true
  performance*analysis: true
  logging*level: "DEBUG"
  sanitizers: ["address", "undefined", "thread", "memory"]
  debug*symbols: true
  optimization*level: "O2"
  build*timeout: 3600
  memory*limit: "8G"
  cpu*limit: 8
  storage*limit: "50G"
  network*access: true
  build*caching: true
  artifact*storage: true

flash*config:
  port*auto*detect: true
  port*validation: true
  port*timeout: 60
  port*retry*attempts: 5
  default*baud*rate: 921600
  default*flash*mode: "dio"
  default*flash*freq: "80m"
  default*flash*size: "8MB"
  flash*verification: true
  flash*checksum: true
  flash*backup: true
  flash*encryption: false
  flash*secure*boot: false
  retry*attempts: 5
  retry*delay: 3
  error*recovery: true
  flash*logging: true
  flash*log*file: "flash*operation.log"
  flash*verbose: true
  flash*progress: true
  flash*validation: true
  flash*optimization: true

system*config:
  development*mode: true
  debug*mode: true
  verbose*output: true
  profiling*mode: false
  production*mode: false
  log*directory: "logs"
  log*retention*days: 90
  log*rotation*size: "500M"
  log*compression: true
  log*encryption: false
  log*archiving: true
  log*analysis: true
  max*parallel*jobs: 8
  memory*limit: "8G"
  cpu*limit: 8
  storage*limit: "100G"
  network*limit: "1Gbps"
  strict*permissions: false
  auto*fix*permissions: true
  user*group*management: true
  security*scanning: true
  vulnerability*checking: true
  cmake*integration: true
  ci*cd*integration: true
  ide*integration: true
  docker*integration: true
  cloud*integration: true
  monitoring: true
  alerting: true
  reporting: true
  analytics: true
```text

### **Best Practices**

#### **1. Configuration Management**
- Use a single configuration file for all settings
- Implement proper validation and error checking
- Use environment variables for dynamic overrides
- Regular configuration validation and testing
- Version control configuration files

#### **2. Environment Variable Usage**
- Use consistent naming conventions
- Document all supported environment variables
- Implement proper override priority
- Validate environment variable values
- Clear documentation of override behavior

#### **3. Configuration Validation**
- Implement comprehensive schema validation
- Check for configuration conflicts
- Validate file paths and dependencies
- Regular configuration integrity checks
- Automated validation in CI/CD

#### **4. Integration and Automation**
- Integrate with build systems (CMake, Make)
- Support CI/CD pipeline integration
- Implement configuration reloading
- Support dynamic configuration updates
- Provide configuration testing tools

---

**Navigation**: [← Previous: Port Detection](README*PORT*DETECTION.md) | [Back to
Scripts](../README.md) | [Next: Scripts Overview →](README_SCRIPTS_OVERVIEW.md)

