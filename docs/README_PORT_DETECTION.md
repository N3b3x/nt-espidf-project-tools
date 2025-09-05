# ESP32 HardFOC Interface Wrapper - Port Detection Guide

This document provides comprehensive documentation for the ESP32 port detection system, including
cross-platform device identification, troubleshooting, and integration with the development
workflow.

---

**Navigation**: [← Previous: Utility Scripts](README*UTILITY*SCRIPTS.md) | [Back to
Scripts](../README.md) | [Next: Centralized Config →](README*CENTRALIZED*CONFIG.md)

---

## 📋 **Table of Contents**

- [📋 Overview](#-overview)
- [🏗️ Architecture and Design](#️-architecture-and-design)
- [🔌 Cross-Platform Detection](#️-cross-platform-detection)
- [🔍 Port Validation and Testing](#️-port-validation-and-testing)
- [⚙️ Configuration and Customization](#️-configuration-and-customization)
- [🚀 Usage Examples and Patterns](#️-usage-examples-and-patterns)
- [🔧 Troubleshooting and Debugging](#️-troubleshooting-and-debugging)
- [📚 Reference and Examples](#️-reference-and-examples)

## 📋 **Overview**

The ESP32 port detection system provides intelligent,
cross-platform device identification and troubleshooting capabilities.
It automatically detects ESP32 devices across different operating systems,
validates port accessibility,
and provides comprehensive troubleshooting guidance for common connectivity issues.

### **Core Features**
- **Cross-Platform Detection**: Automatic ESP32 device identification on Linux, macOS, and Windows (WSL2)
- **Intelligent Troubleshooting**: Automated problem detection and resolution guidance
- **Port Validation**: Comprehensive connectivity and permission testing
- **Integration Ready**: Seamless integration with build and flash systems
- **Performance Optimized**: Fast detection with intelligent caching

### **Key Capabilities**
- Automatic ESP32 device enumeration across platforms
- USB device identification and driver status reporting
- Port permission verification and automatic resolution
- Connection stability testing and validation
- Comprehensive troubleshooting and diagnostic information

## 🏗️ **Architecture and Design**

### **System Architecture**
```text
Port Detection → Platform Detection → Device Enumeration → Port Validation → Status Reporting
      ↓              ↓                    ↓                ↓                ↓
Cross-Platform   OS Adaptation      USB Device Scan    Connectivity    Troubleshooting
Detection        & Optimization     & Identification   Testing         & Guidance
```text

### **Component Interaction**
- **Platform Detection**: Operating system identification and adaptation
- **Device Enumeration**: USB device scanning and ESP32 identification
- **Port Validation**: Connectivity testing and permission verification
- **Status Reporting**: Comprehensive device and port information
- **Troubleshooting**: Problem detection and resolution guidance

### **Design Principles**
- **Cross-Platform**: Consistent behavior across different operating systems
- **Intelligent Detection**: Automatic ESP32 device identification patterns
- **Performance Optimized**: Fast detection with minimal system impact
- **User Experience**: Clear feedback and actionable troubleshooting guidance
- **Integration Ready**: Seamless integration with development workflows

## 🔌 **Cross-Platform Detection**

### **Linux Port Detection**

#### **Device Detection Patterns**
The system automatically identifies ESP32 devices on Linux using multiple detection methods:

```bash
## USB serial device patterns
/dev/ttyUSB0, /dev/ttyUSB1, /dev/ttyACM0, /dev/ttyACM1

## ESP32-specific USB identifiers
CP210x: Silicon Labs CP210x USB to UART Bridge (10c4:ea60)
CH340: WCH CH340 USB to Serial (1a86:7523)
FTDI: FTDI FT232R USB UART (0403:6001)
CDC ACM: USB CDC ACM devices (various vendors)

## Automatic detection command
./detect*ports.sh --verbose
```text

#### **Linux-Specific Features**
```bash
## Kernel message monitoring
dmesg | grep -i usb
dmesg | grep -i tty

## USB device enumeration
lsusb -v | grep -i esp
lsusb -v | grep -A5 -B5 "10c4:ea60"

## Device file system information
ls -la /sys/class/tty/
ls -la /sys/bus/usb/devices/
```text

#### **Permission Management**
```bash
## User group requirements
sudo usermod -a -G dialout $USER
sudo usermod -a -G tty $USER

## udev rules for ESP32 devices
sudo nano /etc/udev/rules.d/99-esp32.rules

## Example udev rules
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", MODE="0666"

## Reload udev rules
sudo udevadm control --reload-rules
sudo udevadm trigger
```text

### **macOS Port Detection**

#### **Device Detection Patterns**
The system adapts to macOS-specific device patterns and system information:

```bash
## macOS device patterns
/dev/cu.usbserial-*, /dev/cu.SLAB*USBtoUART*
/dev/cu.usbmodem*, /dev/cu.usbserial*
/dev/cu.wchusbserial*, /dev/cu.FTDI*

## System information commands
system*profiler SPUSBDataType | grep -i esp
system*profiler SPUSBDataType | grep -A10 -B5 "CP210x"
system*profiler SPUSBDataType | grep -A10 -B5 "CH340"

## Device enumeration
ls /dev/cu.* | grep -E "(usbserial|SLAB|usbmodem|wchusb|FTDI)"
```text

#### **macOS-Specific Features**
```bash
## USB device information
ioreg -p IOUSB -l -w 0 | grep -i esp
ioreg -p IOUSB -l -w 0 | grep -A5 -B5 "CP210x"

## Port validation
## macOS distinguishes between callout (cu.*) and terminal (tty.*) devices
## Use callout devices for ESP32 communication

## Permission handling
## macOS typically doesn't require special permissions for USB devices
## Check System Preferences > Security & Privacy > Privacy > Full Disk Access
```text

#### **macOS Troubleshooting**
```bash
## Check USB device status
system*profiler SPUSBDataType

## Verify device enumeration
ls -la /dev/cu.*

## Test port accessibility
./detect*ports.sh --test-connection

## Check for driver issues
kextstat | grep -i usb
kextstat | grep -i serial
```text

### **Windows (WSL2) Port Detection**

#### **WSL2 Compatibility**
The system provides comprehensive WSL2 support for Windows users:

```bash
## WSL2 port mapping
/dev/ttyS* (COM port equivalents)
/dev/ttyUSB* (USB device mapping)

## USB device detection
lsusb for device identification
## Note: lsusb may not work in all WSL2 configurations

## Port accessibility testing
./detect*ports.sh --test-connection

## Windows COM port mapping
## COM1 → /dev/ttyS0
## COM2 → /dev/ttyS1
## USB devices → /dev/ttyUSB*
```text

#### **WSL2-Specific Features**
```bash
## Windows device detection
## Use Windows Device Manager to identify COM ports
## Map COM ports to WSL2 device files

## USB device access
## Ensure USB devices are accessible from WSL2
## Check Windows USB device sharing settings

## Port validation
./detect*ports.sh --verbose --test-connection
```text

#### **WSL2 Troubleshooting**
```bash
## Check WSL2 USB support
lsusb

## Verify port accessibility
ls /dev/ttyS*
ls /dev/ttyUSB*

## Test basic connectivity
./detect*ports.sh --verbose

## Check Windows device sharing
## Ensure USB devices are shared with WSL2
```text

## 🔍 **Port Validation and Testing**

### **Connectivity Testing**

#### **Basic Port Testing**
```bash
## Test port connectivity
./detect*ports.sh --test-connection

## What it tests
- Port file existence and accessibility
- Read and write permissions
- Basic serial communication capability
- Device responsiveness
- Connection stability
```text

#### **Advanced Port Testing**
```bash
## Comprehensive port analysis
./detect*ports.sh --verbose --test-connection

## Advanced tests include
- USB device enumeration verification
- Driver status and compatibility
- Permission level validation
- Connection stability testing
- Error condition simulation
```text

#### **Test Output Examples**
```bash
## Successful port test
Port: /dev/ttyACM0
  Basic access: ✓ Available
  Read permissions: ✓ Readable
  Write permissions: ✓ Writable
  Serial communication: ✓ Testable
  Status: READY

## Failed port test
Port: /dev/ttyUSB0
  Basic access: ✓ Available
  Read permissions: ✗ Permission denied
  Write permissions: ✗ Permission denied
  Serial communication: ✗ Not testable
  Status: PERMISSION*ERROR
  Solution: Add user to dialout group
```text

### **Permission Verification**

#### **Permission Level Analysis**
```bash
## Permission checking
ls -la /dev/ttyUSB*
ls -la /dev/ttyACM*

## Expected permissions
- Owner: root
- Group: dialout or tty
- Mode: 666 (readable/writable by all)

## User group verification
groups $USER
id $USER
```text

#### **Permission Resolution**
```bash
## Automatic permission fixing
./detect*ports.sh --fix-permissions

## Manual permission resolution
sudo chmod 666 /dev/ttyUSB0
sudo chmod 666 /dev/ttyACM0

## User group management
sudo usermod -a -G dialout,tty $USER
newgrp dialout
```text

### **Device Identification**

#### **USB Device Information**
```bash
## Detailed USB device information
./detect*ports.sh --verbose

## Information provided
- USB bus and device numbers
- Vendor and product IDs
- Device descriptions
- Driver information
- Connection status
```text

#### **Device Type Recognition**
```bash
## ESP32 device patterns
- ESP32: Various USB-to-UART bridges
- ESP32-C3: CP210x, CH340, CDC ACM
- ESP32-C6: CP210x, CH340, CDC ACM
- ESP32-S3: CP210x, CH340, CDC ACM

## Device compatibility
- CP210x: Silicon Labs (most reliable)
- CH340: WCH (good compatibility)
- FTDI: FTDI (excellent compatibility)
- CDC ACM: Native USB (modern devices)
```text

## ⚙️ **Configuration and Customization**

### **Environment Variables**

#### **Detection Configuration**
```bash
## Port detection configuration
export PORT*DETECTION*VERBOSE=1        # Enable verbose output
export PORT*TEST*TIMEOUT=5             # Set connection test timeout (seconds)
export PORT*SCAN*TIMEOUT=3             # Set port scan timeout (seconds)
export PORT*MAX*RETRIES=3              # Set maximum retry attempts
export PORT*DEBUG=1                    # Enable debug mode
```text

#### **Platform-Specific Configuration**
```bash
## Linux configuration
export LINUX*DEVICE*PATTERNS="/dev/ttyUSB*,/dev/ttyACM*"
export LINUX*GROUPS="dialout,tty"
export LINUX*UDEV*RULES="/etc/udev/rules.d/99-esp32.rules"

## macOS configuration
export MACOS*DEVICE*PATTERNS="/dev/cu.usbserial*,/dev/cu.SLAB*"
export MACOS*SYSTEM*PROFILER=1
export MACOS*IOREG=1

## WSL2 configuration
export WSL2*PORT*MAPPING="/dev/ttyS*,/dev/ttyUSB*"
export WSL2*USB*ACCESS=1
export WSL2*COM*MAPPING=1
```text

#### **Troubleshooting Configuration**
```bash
## Troubleshooting options
export PORT*AUTO*FIX=1                 # Enable automatic problem resolution
export PORT*VERBOSE*ERRORS=1           # Show detailed error information
export PORT*SUGGEST*SOLUTIONS=1        # Provide solution suggestions
export PORT*LOG*LEVEL="INFO"           # Set logging level (DEBUG, INFO, WARN, ERROR)
```text

### **Custom Detection Patterns**

#### **User-Defined Device Patterns**
```bash
## Custom device patterns
export CUSTOM*DEVICE*PATTERNS="/dev/ttyCustom*,/dev/ttyESP*"
export CUSTOM*VENDOR*IDS="1234:5678,abcd:efgh"
export CUSTOM*PRODUCT*NAMES="Custom ESP32,My Device"

## Pattern format
- Device files: /dev/tty*
- Vendor IDs: xxxx:yyyy (hex format)
- Product names: String patterns (case-insensitive)
```text

#### **Custom Validation Rules**
```bash
## Custom validation configuration
export CUSTOM*VALIDATION*RULES="strict,permissive,auto"
export CUSTOM*TIMEOUT*VALUES="10,5,3"
export CUSTOM*RETRY*STRATEGIES="linear,exponential,adaptive"

## Validation levels
- strict: Maximum validation, slower detection
- permissive: Basic validation, faster detection
- auto: Automatic level selection based on platform
```text

### **Integration Configuration**

#### **Build System Integration**
```bash
## CMake integration
set(PORT*DETECTION*SCRIPT "${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh")
set(PORT*DETECTION*ARGS "--verbose" "--test-connection")

## Build target integration
add*custom*target(detect*ports
    COMMAND ${PORT*DETECTION*SCRIPT} ${PORT*DETECTION*ARGS}
    COMMENT "Detecting ESP32 ports"
    DEPENDS ${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh
)
```text

#### **CI/CD Integration**
```bash
## GitHub Actions integration
- name: Detect ESP32 Ports
  run: |
    cd examples/esp32
    ./scripts/detect*ports.sh --verbose --test-connection

## GitLab CI integration
detect*ports:
  script:
    - cd examples/esp32
    - ./scripts/detect*ports.sh --verbose
  artifacts:
    reports:
      junit: port*detection*report.xml
```text

## 🚀 **Usage Examples and Patterns**

### **Basic Port Detection Workflows**

#### **1. Quick Port Check**
```bash
## Basic port detection
./detect*ports.sh

## Expected output
- Available ESP32 devices
- Port accessibility status
- Basic device information
- Quick status summary
```text

#### **2. Detailed Port Analysis**
```bash
## Comprehensive port analysis
./detect*ports.sh --verbose

## Expected output
- Detailed device information
- USB device details
- Driver status
- Permission information
- Troubleshooting guidance
```text

#### **3. Port Connectivity Testing**
```bash
## Test port connectivity
./detect*ports.sh --test-connection

## Expected output
- Port accessibility verification
- Connection stability testing
- Error detection and reporting
- Solution suggestions
```text

### **Advanced Port Detection Workflows**

#### **1. Troubleshooting Workflow**
```bash
## Complete troubleshooting process
./detect*ports.sh --verbose --test-connection --fix-permissions

## Process steps
1. Port detection and enumeration
2. Device identification and validation
3. Permission verification and fixing
4. Connectivity testing
5. Problem resolution and reporting
```text

#### **2. Development Environment Setup**
```bash
## Port detection for development setup
./detect*ports.sh --verbose --test-connection

## Integration with setup scripts
./setup*repo.sh
./detect*ports.sh --verify
./flash*app.sh gpio*test Release flash*monitor
```text

#### **3. CI/CD Port Validation**
```bash
## Automated port validation
./detect*ports.sh --verbose --test-connection --ci-mode

## CI process integration
- Pre-build port validation
- Post-deployment verification
- Automated troubleshooting
- Status reporting
```text

### **Integration Workflows**

#### **1. Build System Integration**
```bash
## CMake integration example
cmake*minimum*required(VERSION 3.16)

## Port detection before build
add*custom*target(check*ports
    COMMAND ${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh --verbose
    COMMENT "Checking ESP32 ports before build"
)

## Build dependency on port check
add*custom*target(build*with*port*check
    COMMAND ${CMAKE*COMMAND} --build ${CMAKE*BINARY*DIR}
    DEPENDS check*ports
)
```text

#### **2. Flash System Integration**
```bash
## Port detection before flashing
./detect*ports.sh --verbose --test-connection

## Flash with port validation
./flash*app.sh gpio*test Release flash*monitor

## Integration workflow
1. Port detection and validation
2. Port selection and confirmation
3. Flash operation execution
4. Post-flash verification
```text

#### **3. Development Workflow Integration**
```bash
## Complete development workflow
cd examples/esp32

## Environment setup
./setup*repo.sh

## Port verification
./detect*ports.sh --verbose --test-connection

## Development cycle
./build*app.sh gpio*test Release
./flash*app.sh gpio*test Release flash*monitor
./manage*logs.sh search "ERROR"
```text

## 🔧 **Troubleshooting and Debugging**

### **Common Port Issues**

#### **1. No ESP32 Devices Detected**
**Problem**: No ESP32 devices found during detection
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
journalctl -f | grep -i usb

## Verify USB port functionality
## Try different USB ports
## Check USB cable integrity
## Verify device power status
```text

#### **2. Port Permission Issues**
**Problem**: Port access denied due to insufficient permissions
**Symptoms**: "Permission denied" or "Access denied" errors
**Solutions**:
```bash
## Check user permissions
ls -la /dev/ttyUSB*
groups $USER

## Add user to required groups
sudo usermod -a -G dialout,tty $USER
newgrp dialout

## Create udev rules (Linux)
sudo nano /etc/udev/rules.d/99-esp32.rules

## Reload udev rules
sudo udevadm control --reload-rules
sudo udevadm trigger

## Verify permission changes
ls -la /dev/ttyUSB*
```text

#### **3. Port Connectivity Issues**
**Problem**: Port not accessible or unstable connection
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
## Verify USB power requirements

## Check for driver issues
lsmod | grep -i usb
kextstat | grep -i usb
```text

### **Platform-Specific Issues**

#### **Linux Issues**
```bash
## Driver problems
sudo modprobe cp210x
sudo modprobe ch341
sudo modprobe ftdi*sio

## Kernel module issues
dmesg | grep -i "module not found"
dmesg | grep -i "driver not found"

## udev rule problems
sudo udevadm info -a -n /dev/ttyUSB0
sudo udevadm test /sys/class/tty/ttyUSB0
```text

#### **macOS Issues**
```bash
## System Preferences issues
## Check System Preferences > Security & Privacy > Privacy > Full Disk Access
## Add Terminal or your IDE to Full Disk Access

## USB device enumeration
system*profiler SPUSBDataType
ioreg -p IOUSB -l -w 0

## Driver compatibility
kextstat | grep -i usb
kextstat | grep -i serial
```text

#### **WSL2 Issues**
```bash
## USB device access
## Ensure USB devices are shared with WSL2
## Check Windows Device Manager for COM ports

## Port mapping
ls /dev/ttyS*
ls /dev/ttyUSB*

## Windows device sharing
## Enable USB device sharing in WSL2 settings
```text

### **Debug and Verbose Mode**

#### **Enabling Debug Output**
```bash
## Enable debug mode
export DEBUG=1
export VERBOSE=1
export PORT*DEBUG=1

## Run with debug output
./detect*ports.sh --verbose --debug

## Debug information available
- Port detection process details
- Device enumeration information
- Permission checking details
- USB device information
- Error context and resolution
```text

#### **Debug Information Available**
```bash
## Debug output includes
- Platform detection details
- Device scanning process
- USB enumeration steps
- Permission verification
- Connection testing details
- Error resolution attempts
```text

## 📚 **Reference and Examples**

### **Command Reference**

#### **Port Detection Commands**
```bash
./detect*ports.sh [options]

## Basic options:
##   --help, -h           - Show usage information
##   --verbose            - Show detailed device information
##   --test-connection    - Test port connectivity
##   --fix-permissions    - Attempt to fix permission issues

## Advanced options:
##   --debug              - Enable debug output
##   --ci-mode            - Optimize for CI/CD environments
##   --timeout <seconds>  - Set operation timeout
##   --retries <count>    - Set retry attempts
```text

#### **Environment Variables**
```bash
## Port detection configuration
export PORT*DETECTION*VERBOSE=1        # Enable verbose output
export PORT*TEST*TIMEOUT=5             # Set connection test timeout
export PORT*SCAN*TIMEOUT=3             # Set port scan timeout
export PORT*MAX*RETRIES=3              # Set maximum retry attempts
export PORT*DEBUG=1                    # Enable debug mode
export PORT*AUTO*FIX=1                 # Enable automatic problem resolution
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
- Quick status summary
```text

#### **Advanced Port Detection Configuration**
```bash
## Comprehensive port analysis
./detect*ports.sh --verbose --test-connection --fix-permissions

## Expected behavior
- Detailed device information
- Port connectivity testing
- Permission verification and fixing
- Troubleshooting guidance
- Solution suggestions
```text

#### **CI/CD Port Detection Configuration**
```bash
## CI-optimized port detection
export CI*MODE=1
./detect*ports.sh --verbose --ci-mode --test-connection

## Expected behavior
- Automated port validation
- CI-optimized output format
- Error reporting for CI systems
- Status code for automation
```text

### **Integration Examples**

#### **CMake Integration**
```cmake
## CMakeLists.txt port detection integration
cmake*minimum*required(VERSION 3.16)

## Port detection target
add*custom*target(detect*ports
    COMMAND ${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh --verbose
    COMMENT "Detecting ESP32 ports"
    DEPENDS ${CMAKE*SOURCE*DIR}/scripts/detect*ports.sh
)

## Build dependency on port detection
add*custom*target(build*with*port*check
    COMMAND ${CMAKE*COMMAND} --build ${CMAKE*BINARY*DIR}
    DEPENDS detect*ports
)
```text

#### **CI/CD Integration**
```yaml
## GitHub Actions port detection integration
- name: Detect ESP32 Ports
  run: |
    cd examples/esp32
    ./scripts/detect*ports.sh --verbose --test-connection

- name: Validate Port Configuration
  run: |
    cd examples/esp32
    ./scripts/detect*ports.sh --ci-mode --verbose
```text

#### **Automation Scripts**
```bash
#!/bin/bash
## Automated port detection and validation

cd examples/esp32

## Detect available ports
echo "Detecting ESP32 ports..."
./detect*ports.sh --verbose

## Test port connectivity
echo "Testing port connectivity..."
./detect*ports.sh --test-connection

## Fix permission issues if needed
echo "Checking and fixing permissions..."
./detect*ports.sh --fix-permissions

## Final validation
echo "Final port validation..."
./detect*ports.sh --verbose --test-connection

echo "Port detection complete!"
```text

### **Best Practices**

#### **1. Port Detection**
- Always use verbose mode for troubleshooting
- Test port connectivity before operations
- Verify permissions and user group membership
- Use automatic detection when possible
- Regular port health checks

#### **2. Troubleshooting**
- Enable debug mode for detailed information
- Use systematic problem resolution approach
- Document solutions for future reference
- Regular system health checks
- Monitor for recurring issues

#### **3. Integration**
- Integrate port detection into build workflows
- Use port validation before critical operations
- Implement automated port health monitoring
- Regular port status reporting
- Error handling and recovery

#### **4. Performance**
- Use appropriate detection timeouts
- Implement intelligent retry strategies
- Cache port information when possible
- Optimize for CI/CD environments
- Monitor detection performance

---

**Navigation**: [← Previous: Utility Scripts](README*UTILITY*SCRIPTS.md) | [Back to
Scripts](../README.md) | [Next: Centralized Config →](README*CENTRALIZED*CONFIG.md)
