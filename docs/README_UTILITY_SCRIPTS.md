# ESP32 Interface Wrapper - Utility Scripts Guide

This document provides comprehensive documentation for the ESP32 utility scripts, including port
detection, setup automation, and helper tools for development workflow management.

---

**Navigation**: [← Previous: Logging System](README*LOGGING*SYSTEM.md) | [Back to
Scripts](../README.md) | [Next: Port Detection →](README*PORT*DETECTION.md)

---

## 📋 **Table of Contents**

- [📋 Overview](#-overview)
- [🏗️ Architecture and Design](#️-architecture-and-design)
- [🔌 Port Detection and Troubleshooting](#️-port-detection-and-troubleshooting)
- [⚙️ Environment Setup and Automation](#️-environment-setup-and-automation)
- [🔧 Configuration and Information Tools](#️-configuration-and-information-tools)
- [🚀 Usage Examples and Patterns](#️-usage-examples-and-patterns)
- [🔍 Troubleshooting and Debugging](#️-troubleshooting-and-debugging)
- [📚 Reference and Examples](#️-reference-and-examples)

## 📋 **Overview**

The ESP32 utility scripts provide essential tools for development environment setup,
port detection, troubleshooting, and workflow automation.
These scripts ensure consistent development environments across different platforms and provide
intelligent automation for common development tasks.

### **Core Features**
- **Cross-Platform Port Detection**: Automatic ESP32 device identification
- **Environment Separation**: Clear separation between local development and CI environments
- **Intelligent Troubleshooting**: Automated problem detection and resolution
- **Configuration Management**: Centralized configuration and information access
- **CI/CD Integration**: Optimized for automated environments with minimal dependencies

### **Key Capabilities**
- Automatic ESP32 device detection across platforms
- **Dual Environment Setup**: Local development (complete) and CI (minimal)
- Intelligent dependency management and installation
- Cross-platform compatibility and optimization
- Automated troubleshooting and problem resolution
- **CI-Specific Optimizations**: Cache management and build directory preparation

## 🏗️ **Architecture and Design**

### **New System Architecture**
```text
Utility Scripts → Environment Detection → Setup Selection → Tool Installation → Validation
      ↓              ↓                    ↓                ↓                ↓
Port Detection   Local vs CI        setup*repo.sh      Dependency Mgmt   Environment
& Troubleshooting  Environment       vs ESP-IDF CI      & Installation    Verification
```text

### **Environment Setup Architecture**

```python
┌─────────────────────────────────────────────────────────────────────────────┐
│                           SETUP COMMON FUNCTIONS                            │
│                    (setup*common.sh - shared utilities)                     │
├─────────────────────────────────────────────────────────────────────────────┤
│  • System dependency installation                                           │
│  • Clang toolchain setup                                                    │
│  • ESP-IDF installation and management                                      │
│  • Python dependency management                                             │
│  • Cross-platform compatibility functions                                   │
│  • Cache optimization and management                                        │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                    ENVIRONMENT-SPECIFIC SETUP                               │
├─────────────────────────────────────────────────────────────────────────────┤
│  🏠 LOCAL DEVELOPMENT (setup*repo.sh)        🏭 CI/CD (Direct ESP-IDF)      │
│  • Full development environment              • ESP-IDF CI action handles    │
│  • Interactive user setup                    • Direct project building      │
│  • Complete tool installation                • Cache-aware installation     │
│  • Environment variables setup               • Build directory preparation  │
│  • Development aliases                       • CI-specific optimizations    │
│  • ESP-IDF auto-installation                 • ESP-IDF handled by CI action │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           BUILD SYSTEM INTEGRATION                          │
├─────────────────────────────────────────────────────────────────────────────┤
│  • build*app.sh uses environment from setup                                 │
│  • flash*app.sh integrates with setup                                       │
│  • CI workflows use ESP-IDF CI action directly                              │
│  • Local development uses setup*repo.sh for environment                     │
└─────────────────────────────────────────────────────────────────────────────┘
```text

### **Component Interaction**
- **Port Detection**: Cross-platform ESP32 device identification
- **Environment Setup**: Automated dependency and tool installation with environment separation
- **Configuration Tools**: Centralized configuration management
- **Troubleshooting**: Automated problem detection and resolution
- **Platform Adaptation**: Cross-platform compatibility and optimization

### **Design Principles**
- **Cross-Platform**: Consistent behavior across Linux, macOS, and Windows (WSL2)
- **Automated Operation**: Minimal user intervention required
- **Intelligent Fallbacks**: Graceful degradation when tools unavailable
- **Performance Optimized**: Efficient execution and resource usage
- **User Experience**: Clear feedback and error handling
- **Environment Separation**: Clear distinction between local development and CI needs

## 🔌 **Port Detection and Troubleshooting**

### **Cross-Platform Port Detection**

#### **Linux Port Detection**
The system automatically detects ESP32 devices on Linux:

```bash
## USB serial device patterns
/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyACM0

## ESP32-specific USB identifiers
CP210x: Silicon Labs CP210x USB to UART Bridge
CH340: WCH CH340 USB to Serial
FTDI: FTDI FT232R USB UART
CDC ACM: USB CDC ACM devices

## Automatic detection
./detect*ports.sh --verbose
```text

#### **macOS Port Detection**
The system adapts to macOS-specific device patterns:

```bash
## macOS device patterns
/dev/cu.usbserial-*, /dev/cu.SLAB*USBtoUART*
/dev/cu.usbmodem*, /dev/cu.usbserial*

## System information
system*profiler SPUSBDataType | grep -i esp

## Automatic detection
./detect*ports.sh --verbose
```text

#### **Windows (WSL2) Port Detection**
The system provides WSL2 compatibility:

```bash
## WSL2 port mapping
/dev/ttyS* (COM port equivalents)

## USB device detection
lsusb for device identification

## Port accessibility testing
./detect*ports.sh --test-connection
```text

### **Port Validation and Testing**

#### **Connectivity Testing**
```bash
## Test port connectivity
./detect*ports.sh --test-connection

## Verify port accessibility
./detect*ports.sh --verbose

## Check port permissions and status
./detect*ports.sh --verbose --test-connection
```text

#### **Permission Management**
The system handles common permission issues:

```bash
## Linux udev rules for ESP32 devices
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", MODE="0666"

## User group membership
sudo usermod -a -G dialout $USER
sudo usermod -a -G tty $USER

## Permission verification
ls -la /dev/ttyUSB*
groups $USER
```text

### **Troubleshooting Capabilities**

#### **Automatic Problem Detection**
```bash
## Common issue detection
- USB driver availability
- Port permission problems
- Device enumeration issues
- Connection stability problems
```text

#### **Problem Resolution**
```bash
## Automatic resolution attempts
- Permission fixing
- Driver installation guidance
- Port configuration
- Connection testing
```text

## ⚙️ **Environment Setup and Automation**

### **Dual Environment Setup System**

The system now provides two distinct setup approaches optimized for different use cases:

#### **Local Development Setup (setup*repo.sh)**
```bash
## Complete development environment setup
./setup*repo.sh

## What it installs
- System dependencies (build tools, libraries)
- Clang-20 toolchain (compiler, formatter, analyzer)
- ESP-IDF v5.5 (ESP32 development framework)
- Python dependencies (PyYAML)
- yq (YAML processor)
- Development aliases and environment variables
- Complete development toolchain
```text

#### **Direct ESP-IDF CI Action - Simplified CI/CD**
```bash
## CI builds use ESP-IDF CI action directly
## ESP-IDF CI action handles everything

## What it provides
- Essential build tools (clang-20, clang-format, clang-tidy)
- Python dependencies (PyYAML, yq)
- CI build directory structure
- Minimal dependencies for CI builds
- ESP-IDF handled by ESP-IDF CI action

## NEW: Required environment variables
- ESP32*PROJECT*PATH: Path to ESP32 project directory (e.g., 'examples/esp32')
```text

### **Environment-Specific Features**

#### **Local Development Features**
```bash
## Interactive setup process
- OS detection and adaptation
- Dependency verification
- Installation confirmation
- Progress feedback
- Completion verification
- User guidance and troubleshooting
```text

#### **CI/CD Environment Features**
```bash
## CI optimizations
- Cache-aware installation
- Minimal dependency installation
- Non-interactive operation
- Cache statistics and reporting
- Build directory preparation
- ESP-IDF integration with CI actions
- Reliable tool installation (no caching issues)
```text

### **Setup Script Selection Guide**

| Use Case | Setup Script | Key Benefits |

|----------|--------------|--------------|

| **Local Development** | `setup*repo.sh` | Complete toolchain, interactive setup, ESP-IDF auto-installation |

| **CI/CD Pipelines** | ESP-IDF CI action | Direct building, no setup needed, official action |

| **New Developer Setup** | `setup*repo.sh` | Full environment, user guidance, complete toolchain |

| **Automated Builds** | ESP-IDF CI action | Direct building, no setup needed, official action |

### **Cross-Platform Compatibility**

#### **Operating System Support**
```bash
## Supported platforms
- Linux (Ubuntu 20.04+, Fedora, CentOS)
- macOS (10.15+)
- Windows (WSL2)

## Platform-specific adaptations
- Package manager detection
- Tool installation methods
- Path handling
- Permission management
```text

#### **Dependency Management**
```bash
## Automatic dependency detection
- Required tools identification
- Version compatibility checking
- Installation method selection
- Fallback mechanisms
- Environment-specific optimization
```text

## 🔧 **Configuration and Information Tools**

### **Configuration Management**

#### **Centralized Configuration Access**
```bash
## Configuration information
./get*app*info.py list                    # List all applications
./get*app*info.py source*file gpio*test   # Get source file path
./get*app*info.py validate adc*test       # Validate application

## Configuration validation
- Application existence verification
- Source file path validation
- Configuration integrity checking
```text

#### **Configuration Integration**
```bash
## CMake integration
execute*process(
    COMMAND python3 get*app*info.py source*file ${APP*TYPE}
    OUTPUT*VARIABLE APP*SOURCE*FILE
    OUTPUT*STRIP*TRAILING*WHITESPACE
)

## Build system integration
- Source file resolution
- Configuration validation
- Build parameter extraction
```text

### **Information Extraction**

#### **Application Information**
```bash
## Available information
- Application descriptions
- Source file paths
- Build type support
- ESP-IDF version compatibility
- CI/CD configuration
- Featured status
```text

#### **System Information**
```bash
## System details
- Operating system detection
- Tool availability
- Version information
- Path configurations
- Environment variables
```text

## 🚀 **Usage Examples and Patterns**

### **Environment Setup Workflows**

#### **1. Local Development Setup**
```bash
## Complete local setup
./setup*repo.sh

## Setup process
1. OS detection and adaptation
2. Dependency verification
3. Tool installation
4. Environment configuration
5. Verification and testing
6. User guidance and troubleshooting
```text

#### **2. CI/CD Environment Setup**
```bash
## CI builds use ESP-IDF CI action directly

## CI process
1. Cache-aware installation
2. Minimal dependency setup
3. Environment optimization
4. Cache statistics
5. Build directory preparation
6. ESP-IDF CI action integration
```text

#### **3. Environment Verification**
```bash
## Verify setup
./setup*repo.sh --verify

## Verification checks
- Tool availability
- Version compatibility
- Path configuration
- Environment variables
- Functionality testing
```text

### **Port Detection Workflows**

#### **1. Basic Port Detection**
```bash
## Quick port detection
./detect*ports.sh

## Expected output
- Available ESP32 devices
- Port accessibility status
- Basic device information
```text

#### **2. Detailed Port Analysis**
```bash
## Comprehensive port analysis
./detect*ports.sh --verbose

## Expected output
- Detailed device information
- USB device details
- Permission status
- Driver information
```text

#### **3. Port Connectivity Testing**
```bash
## Test port connectivity
./detect*ports.sh --test-connection

## Expected output
- Port accessibility verification
- Connection stability testing
- Error detection and reporting
```text

### **Configuration Management Workflows**

#### **1. Application Information Access**
```bash
## Get application information
./get*app*info.py list
./get*app*info.py source*file gpio*test
./get*app*info.py validate adc*test

## Information usage
- Build system integration
- Configuration validation
- Documentation generation
- CI/CD pipeline configuration
```text

#### **2. Configuration Validation**
```bash
## Validate configuration
./get*app*info.py validate gpio*test
./get*app*info.py validate adc*test

## Validation process
- Application existence check
- Source file verification
- Configuration integrity
- Dependency validation
```text

### **Troubleshooting Workflows**

#### **1. Port Problem Resolution**
```bash
## Port troubleshooting
./detect*ports.sh --verbose --test-connection

## Problem resolution
1. Issue identification
2. Automatic resolution attempts
3. Manual resolution guidance
4. Verification testing
```text

#### **2. Environment Problem Resolution**
```bash
## Environment troubleshooting
./setup*repo.sh --troubleshoot

## Resolution process
1. Problem diagnosis
2. Dependency verification
3. Tool reinstallation
4. Configuration repair
5. Verification testing
```text

#### **3. Environment-Specific Troubleshooting**
```bash
## Local development issues
./setup*repo.sh --debug

## Environment verification
./setup*repo.sh --verify  # Local
```text

## 🔍 **Troubleshooting and Debugging**

### **Common Port Issues**

#### **1. No ESP32 Devices Detected**
**Problem**: No ESP32 devices found
**Symptoms**: "No ports detected" or "No devices found" messages
**Solutions**:
```bash
## Check device connections
./detect*ports.sh --verbose

## Verify USB drivers
lsusb | grep -i esp
system*profiler SPUSBDataType | grep -i esp

## Check device enumeration
dmesg | grep -i usb
```text

#### **2. Port Permission Issues**
**Problem**: Port access denied
**Symptoms**: "Permission denied" or "Access denied" errors
**Solutions**:
```bash
## Check user permissions
ls -la /dev/ttyUSB*
groups $USER

## Add user to required groups
sudo usermod -a -G dialout,tty $USER

## Create udev rules
sudo nano /etc/udev/rules.d/99-esp32.rules
```text

#### **3. Port Connectivity Issues**
**Problem**: Port not accessible or unstable
**Symptoms**: "Port not accessible" or connection failures
**Solutions**:
```bash
## Test port connectivity
./detect*ports.sh --test-connection

## Check port stability
./detect*ports.sh --verbose --test-connection

## Verify device mode
## Check for bootloader mode
## Reset device if necessary
```text

### **Environment Setup Issues**

#### **1. Setup Script Selection Issues**
**Problem**: Wrong setup script for environment
**Symptoms**: Setup errors or missing functionality
**Solutions**:
```bash
## For local development (complete environment)
./setup*repo.sh

## For CI/CD (ESP-IDF CI action handles everything)

## Check script help for details
./setup*repo.sh --help
```text

#### **2. Dependency Installation Failures**
**Problem**: Required tools not installed
**Symptoms**: "Command not found" or installation errors
**Solutions**:
```bash
## Check tool availability
which git cmake ninja ccache

## Manual installation
sudo apt-get install git cmake ninja-build ccache

## Verify installation
./setup*repo.sh --verify  # Local
```text

#### **3. ESP-IDF Installation Issues**
**Problem**: ESP-IDF not properly installed
**Symptoms**: "ESP-IDF not found" or environment errors
**Solutions**:
```bash
## Local development
./setup*repo.sh --reinstall-esp-idf

## CI environment
## Ensure ESP-IDF CI action is properly configured

## Verify environment
source ~/esp/esp-idf/export.sh
idf.py --version
```text

#### **4. Permission and Path Issues**
**Problem**: Insufficient permissions or incorrect paths
**Symptoms**: "Permission denied" or "Path not found" errors
**Solutions**:
```bash
## Check permissions
ls -la ~/esp/
ls -la ~/.espressif/

## Fix permissions
chmod -R 755 ~/esp/
chmod -R 755 ~/.espressif/

## Verify paths
echo $IDF*PATH
echo $PATH
```text

### **Debug and Verbose Mode**

#### **Enabling Debug Output**
```bash
## Enable debug mode
export DEBUG=1
export VERBOSE=1

## Run with debug output
./detect*ports.sh --verbose
./setup*repo.sh --debug
./get*app*info.py --verbose
```text

#### **Debug Information Available**
```bash
## Debug information
- Port detection process details
- Device enumeration information
- Permission checking details
- Installation process information
- Configuration loading details
- Error context and resolution
- Environment setup process details
- CI vs local environment differences
```text

## 📚 **Reference and Examples**

### **Command Reference**

#### **Port Detection Commands**
```bash
./detect*ports.sh [options]

## Options:
##   --verbose              - Show detailed device information
##   --test-connection     - Test port connectivity
##   --help, -h           - Show usage information
```text

#### **Setup Commands**
```bash
./setup*repo.sh [options]     # Local development setup (complete environment)

## Common options:
##   --help, -h           - Show usage information
##   --verify             - Verify installation
##   --debug             - Enable debug output
##   --reinstall-esp-idf - Reinstall ESP-IDF (setup*repo.sh only)

## Environment-specific options:
## setup*repo.sh:
##   --interactive        - Interactive setup mode
##   --non-interactive   - Non-interactive setup mode

```text

#### **Configuration Commands**
```bash
./get*app*info.py <command> [args...]

## Commands:
##   list                    - List all available applications
##   source*file <app*type>  - Get source file path for application
##   validate <app*type>     - Validate application configuration
##   --help, -h             - Show usage information
```text

### **Environment Variables**

#### **Port Detection Variables**
```bash
## Port detection configuration
export PORT*DETECTION*VERBOSE=1    # Enable verbose output
export PORT*TEST*TIMEOUT=5         # Set connection test timeout
export PORT*SCAN*TIMEOUT=3         # Set port scan timeout
```text

#### **Setup Configuration Variables**
```bash
## Setup configuration
export SETUP*MODE="local"          # Set setup mode (local/ci)
export ESP*IDF*VERSION="v5.5"      # Set ESP-IDF version
export CLANG*VERSION="20"          # Set Clang version
export PYTHON*VERSION="3.9"        # Set Python version

## Environment-specific variables
export ESP32*PROJECT*PATH="examples/esp32"  # Project path
export IDF*TARGET="esp32c6"        # Target MCU
export BUILD*TYPE="Release"        # Build type
export APP*TYPE="gpio*test"        # Application type
```text

#### **Debug Configuration Variables**
```bash
## Debug configuration
export DEBUG=1                     # Enable debug mode
export VERBOSE=1                   # Enable verbose output
export SETUP*DEBUG=1               # Enable setup debug mode
export PORT*DEBUG=1                # Enable port debug mode
```text

### **Configuration Examples**

#### **Minimal Port Detection Configuration**
```bash
## Basic port detection
./detect*ports.sh

## Expected behavior
- Automatic ESP32 device detection
- Basic port information display
- Error reporting for issues
```text

#### **Advanced Port Detection Configuration**
```bash
## Comprehensive port analysis
./detect*ports.sh --verbose --test-connection

## Expected behavior
- Detailed device information
- Port connectivity testing
- Permission verification
- Troubleshooting guidance
```text

#### **Environment Setup Configuration**

#### **Local Development Setup**
```bash
## Complete environment setup
./setup*repo.sh

## Expected behavior
- OS detection and adaptation
- Complete dependency installation
- Tool configuration
- Environment verification
- Interactive user guidance
```text

#### **CI/CD Environment Setup**
```bash
## CI builds use ESP-IDF CI action directly
## No setup needed

## Expected behavior
- Minimal dependency installation
- Cache-aware setup
- Build directory preparation
- Non-interactive operation
- CI-specific optimizations
```text

### **Integration Examples**

#### **CMake Integration**
```cmake
## CMakeLists.txt utility integration
cmake*minimum*required(VERSION 3.16)

## Port detection integration
add*custom*target(detect*ports
    COMMAND ${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh --verbose
    COMMENT "Detecting ESP32 ports"
)

## Configuration validation
add*custom*target(validate*config
    COMMAND python3 ${CMAKE*SOURCE*DIR}/scripts/get*app*info.py validate ${APP*TYPE}
    COMMENT "Validating application configuration"
)
```text

#### **CI/CD Integration**

#### **GitHub Actions Integration**
```yaml
## GitHub Actions utility integration
- name: Setup ESP32 Environment
  run: |
    cd examples/esp32
    # CI builds use ESP-IDF CI action directly

- name: Detect ESP32 Ports
  run: |
    cd examples/esp32
    ./scripts/detect*ports.sh --verbose

- name: Validate Configuration
  run: |
    cd examples/esp32
    python3 ./scripts/get*app*info.py validate gpio*test
```text

#### **GitLab CI Integration**
```yaml
## GitLab CI utility integration
setup*environment:
  script:
    - cd examples/esp32
    # CI builds use ESP-IDF CI action directly
  artifacts:
    paths:
      - examples/esp32/build*/
```text

#### **Jenkins Pipeline Integration**
```groovy
// Jenkins Pipeline utility integration
pipeline {
  agent any
  stages {
    stage('Setup Environment') {
      steps {
        script {
          sh 'cd examples/esp32 && echo "CI builds use ESP-IDF CI action directly"'
        }
      }
    }
    stage('Detect Ports') {
      steps {
        script {
          sh 'cd examples/esp32 && ./scripts/detect*ports.sh --verbose'
        }
      }
    }
  }
}
```text

#### **Automation Scripts**
```bash
#!/bin/bash
## Automated development environment setup

cd examples/esp32

## Choose setup based on environment
if [[ "$CI" == "true" ]]; then
    echo "CI builds use ESP-IDF CI action directly"
else
    echo "Setting up local development environment..."
    ./setup*repo.sh
fi

## Verify setup
echo "Verifying setup..."
if [[ "$CI" == "true" ]]; then
    echo "CI builds use ESP-IDF CI action directly"
else
    ./setup*repo.sh --verify
fi

## Detect available ports
echo "Detecting ESP32 ports..."
./detect*ports.sh --verbose

## Validate configuration
echo "Validating configuration..."
python3 ./get*app*info.py validate gpio*test

echo "Setup complete!"
```text

### **Best Practices**

#### **1. Port Detection**
- Always use verbose mode for troubleshooting
- Test port connectivity before operations
- Verify permissions and user group membership
- Use automatic detection when possible

#### **2. Environment Setup**
- **Local Development**: Use `setup*repo.sh` for complete development environment
- **CI/CD**: Use ESP-IDF CI action directly
- Verify installation after setup
- Monitor cache usage and optimization
- Regular environment verification

#### **3. Configuration Management**
- Validate configuration before use
- Use centralized configuration access
- Monitor configuration changes
- Regular configuration verification

#### **4. Troubleshooting**
- Enable debug mode for detailed information
- Use systematic problem resolution approach
- Document solutions for future reference
- Regular system health checks
- Use environment-specific troubleshooting approaches

---

**Navigation**: [← Previous: Logging System](README*LOGGING*SYSTEM.md) | [Back to
Scripts](../README.md) | [Next: Port Detection →](README_PORT_DETECTION.md)
