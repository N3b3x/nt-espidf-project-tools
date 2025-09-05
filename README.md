# 🔧 ESP32 Scripts - Build System Documentation

<div align="center">

![Scripts](https://img.shields.io/badge/Scripts-Build%20System-blue?style=for-the-badge&logo=bash)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-Auto%20Management-green?style=for-the-badge&logo=espressif)
![CI/CD](https://img.shields.io/badge/CI%2FCD-GitHub%20Actions-orange?style=for-the-badge&logo=github)

**🎯 Build System Scripts for ESP32 Development**

*Scripts providing automated ESP-IDF management, build automation, and CI/CD integration*

</div>

---

## 📚 **Table of Contents**

- [🎯 **Overview**](#-overview)
- [🏗️ **Script Architecture**](#️-script-architecture)
- [📁 **Script Reference**](#-script-reference)
- [🔧 **Core Scripts**](#-core-scripts)
- [⚙️ **Setup Scripts**](#️-setup-scripts)
- [🔄 **Utility Scripts**](#-utility-scripts)
- [📊 **Configuration Scripts**](#-configuration-scripts)
- [🚀 **Usage Examples**](#-usage-examples)
- [🔄 **CI/CD Workflows & Quality Assurance**](#-cicd-workflows--quality-assurance)
- [🔍 **Troubleshooting**](#-troubleshooting)
- [🤝 **Contributing**](#-contributing)

---

## 🎯 **Overview**

This repository contains a **decoupled build system** for ESP32 development that can be used in two flexible ways. The scripts feature automated ESP-IDF management, build processes, and CI/CD integration while maintaining complete portability.

### 🏆 **Key Features**

- **🚀 Dual Usage Modes** - Works both as project-integrated scripts and as portable tools
- **🔧 Automated ESP-IDF Management** - Auto-detection, installation, and environment setup
- **📊 Dynamic Configuration Loading** - YAML-based configuration with hierarchical overrides
- **🔄 Intelligent Build System** - Incremental builds with caching and optimization
- **🛡️ Enhanced Validation System** - Smart combination validation and error prevention
- **🧠 Smart Defaults** - Automatic ESP-IDF version selection based on app and build type
- **🌐 CI/CD Integration** - Seamless GitHub Actions integration
- **📁 Structured Output** - Parseable build directories and artifact management
- **🔍 Comprehensive Logging** - Detailed build logs and error reporting
- **🆕 Environment Separation** - Clear separation between local development and CI environments
- **🚀 CI Pipeline Optimization** - Optimized CI pipeline with parallel execution, smart caching, and reliable tool installation

## 🔄 **Two Usage Modes**

### **Mode 1: Project-Integrated Scripts** 📁
**When**: Scripts are cloned/copied into your ESP32 project
**Location**: `your-project/hf-espidf-project-tools/` (default) or `your-project/scripts/` (with special setup)
**Configuration**: Automatically finds `app_config.yml` in project root
**Usage**: `./hf-espidf-project-tools/build_app.sh app_name build_type`

```bash
# Project structure (default)
your-esp32-project/
├── app_config.yml          # ← Scripts find this automatically
├── main/
├── components/
└── hf-espidf-project-tools/  # ← Scripts live here (default)
    ├── build_app.sh
    ├── flash_app.sh
    └── config_loader.sh

# Usage (from project root)
./hf-espidf-project-tools/build_app.sh gpio_test Release
./hf-espidf-project-tools/flash_app.sh flash_monitor adc_test
```

**Alternative: `scripts/` Directory** (requires special setup)
```bash
# Project structure (alternative)
your-esp32-project/
├── app_config.yml          # ← Scripts find this automatically
├── main/
├── components/
└── scripts/                # ← Scripts live here (alternative)
    ├── build_app.sh
    ├── flash_app.sh
    └── config_loader.sh

# Usage (from project root)
./scripts/build_app.sh gpio_test Release
./scripts/flash_app.sh flash_monitor adc_test
```

### **Mode 2: Portable Tools** 🚀
**When**: Scripts are placed anywhere (shared tools, CI systems, etc.)
**Location**: Any directory (`/opt/esp32-tools/`, `~/tools/`, CI runners, etc.)
**Configuration**: Use `--project-path` flag or `PROJECT_PATH` environment variable
**Usage**: `./build_app.sh --project-path /path/to/project app_name build_type`

```bash
# Shared tools structure
/opt/esp32-tools/
├── build_app.sh
├── flash_app.sh
└── config_loader.sh

# Usage (from anywhere)
/opt/esp32-tools/build_app.sh --project-path ~/my-esp32-project gpio_test Release
/opt/esp32-tools/flash_app.sh --project-path ~/my-esp32-project flash_monitor adc_test

# Or with environment variable
export PROJECT_PATH=~/my-esp32-project
/opt/esp32-tools/build_app.sh gpio_test Release
```

### **Configuration Discovery** 🔍

Both modes automatically discover the `app_config.yml` configuration file:

| Mode | Discovery Method | Config Location |
|------|------------------|-----------------|
| **Project-Integrated** | Parent directory of scripts | `project/app_config.yml` |
| **Portable** | `--project-path` or `PROJECT_PATH` | `specified-path/app_config.yml` |

**Configuration Priority:**
1. `--project-path` flag (highest priority)
2. `PROJECT_PATH` environment variable
3. Default: parent directory of script location

## 📥 **Getting Started - Project Integration**

### **Method 1: Default Setup (Recommended)** 🚀

Clone the repository directly into your ESP32 project:

```bash
# Navigate to your ESP32 project
cd ~/my-esp32-project

# Clone the repository
git clone https://github.com/N3b3x/hf-espidf-project-tools.git

# Your project structure will be:
# my-esp32-project/
# ├── app_config.yml
# ├── main/
# ├── components/
# └── hf-espidf-project-tools/    # ← Repository cloned here
#     ├── build_app.sh
#     ├── flash_app.sh
#     └── ...

# Usage (from project root)
./hf-espidf-project-tools/build_app.sh gpio_test Release
./hf-espidf-project-tools/flash_app.sh flash_monitor adc_test
```

**Benefits:**
- ✅ Simple one-command setup
- ✅ Full control over scripts
- ✅ Easy to update with `git pull`
- ✅ Can modify scripts if needed

### **Method 2: Git Submodule** 🔗

Keep the scripts as a separate repository while integrating them:

```bash
# Navigate to your ESP32 project
cd ~/my-esp32-project

# Add as submodule
git submodule add https://github.com/N3b3x/hf-espidf-project-tools.git

# Initialize and update the submodule
git submodule update --init --recursive

# Your project structure will be:
# my-esp32-project/
# ├── app_config.yml
# ├── main/
# ├── components/
# └── hf-espidf-project-tools/    # ← Submodule here
#     ├── build_app.sh
#     ├── flash_app.sh
#     └── ...

# Usage (from project root)
./hf-espidf-project-tools/build_app.sh gpio_test Release
```

**Benefits:**
- ✅ Keeps scripts as separate repository
- ✅ Easy to update with `git submodule update`
- ✅ Maintains version control
- ✅ No file duplication

### **Method 3: Custom `scripts/` Directory** (Advanced) ⚙️

If you prefer the scripts to be in a `scripts/` directory, you can achieve this with special setup:

```bash
# Navigate to your ESP32 project
cd ~/my-esp32-project

# Clone with custom directory name
git clone https://github.com/N3b3x/hf-espidf-project-tools.git scripts

# OR add as submodule with custom name
git submodule add --name scripts https://github.com/N3b3x/hf-espidf-project-tools.git scripts

# Your project structure will be:
# my-esp32-project/
# ├── app_config.yml
# ├── main/
# ├── components/
# └── scripts/                    # ← Custom directory name
#     ├── build_app.sh
#     ├── flash_app.sh
#     └── ...

# Usage (from project root)
./scripts/build_app.sh gpio_test Release
./scripts/flash_app.sh flash_monitor adc_test
```

**Note:** This method requires special setup and is not the default behavior. The scripts will work the same way, but you'll need to use `./scripts/` instead of `./hf-espidf-project-tools/` in all commands.

### **Verification** ✅

After setup, verify the integration works:

```bash
# Navigate to your project root
cd ~/my-esp32-project

# Test script discovery (default setup)
./hf-espidf-project-tools/build_app.sh --help

# OR test script discovery (custom scripts/ setup)
./scripts/build_app.sh --help

# Test configuration loading
./hf-espidf-project-tools/build_app.sh combinations
# OR
./scripts/build_app.sh combinations

# Should show available apps and build combinations
```

---

## 🚀 **Usage Examples**

### **Mode 1: Project-Integrated Usage** 📁

When scripts are part of your ESP32 project:

```bash
# Project structure (default)
my-esp32-project/
├── app_config.yml
├── main/
├── components/
└── hf-espidf-project-tools/    # ← Scripts are here (default)
    ├── build_app.sh
    ├── flash_app.sh
    └── config_loader.sh

# Usage (from project root)
./hf-espidf-project-tools/build_app.sh gpio_test Release
./hf-espidf-project-tools/flash_app.sh flash_monitor adc_test
./hf-espidf-project-tools/manage_idf.sh list

# Python scripts
python3 hf-espidf-project-tools/get_app_info.py list
python3 hf-espidf-project-tools/generate_matrix.py
```

**Alternative: Custom `scripts/` Directory**
```bash
# Project structure (alternative)
my-esp32-project/
├── app_config.yml
├── main/
├── components/
└── scripts/                    # ← Scripts are here (alternative)
    ├── build_app.sh
    ├── flash_app.sh
    └── config_loader.sh

# Usage (from project root)
./scripts/build_app.sh gpio_test Release
./scripts/flash_app.sh flash_monitor adc_test
./scripts/manage_idf.sh list

# Python scripts
python3 scripts/get_app_info.py list
python3 scripts/generate_matrix.py
```

### **Mode 2: Portable Tools Usage** 🚀

When scripts are shared tools or in CI systems:

```bash
# Shared tools structure
/opt/esp32-tools/
├── build_app.sh
├── flash_app.sh
└── config_loader.sh

# Usage with --project-path flag
/opt/esp32-tools/build_app.sh --project-path ~/my-esp32-project gpio_test Release
/opt/esp32-tools/flash_app.sh --project-path ~/my-esp32-project flash_monitor adc_test

# Usage with environment variable
export PROJECT_PATH=~/my-esp32-project
/opt/esp32-tools/build_app.sh gpio_test Release
/opt/esp32-tools/flash_app.sh flash_monitor adc_test

# Python scripts
python3 /opt/esp32-tools/get_app_info.py list --project-path ~/my-esp32-project
python3 /opt/esp32-tools/generate_matrix.py --project-path ~/my-esp32-project
```

### **Real-World Scenarios**

```bash
# Scenario 1: Multiple ESP32 projects with shared tools
/opt/esp32-tools/build_app.sh --project-path ~/projects/robot-controller gpio_test Release
/opt/esp32-tools/build_app.sh --project-path ~/projects/sensor-node adc_test Debug

# Scenario 2: CI/CD with portable scripts
./ci-scripts/build_app.sh --project-path $GITHUB_WORKSPACE/examples/esp32 gpio_test Release

# Scenario 3: Development with project-integrated scripts
cd ~/my-esp32-project
./hf-espidf-project-tools/build_app.sh gpio_test Release
./hf-espidf-project-tools/flash_app.sh flash_monitor gpio_test

# Scenario 4: Mixed usage (some projects integrated, some portable)
cd ~/project-with-tools
./hf-espidf-project-tools/build_app.sh gpio_test Release

cd ~/project-without-tools
/opt/esp32-tools/build_app.sh --project-path . gpio_test Release
```

---

## 🏗️ **Script Architecture**

### **Decoupled Design**

The scripts are designed to work in **two flexible modes** while maintaining the same core functionality:

```
# Mode 1: Project-Integrated (default)
your-esp32-project/
├── app_config.yml              # ← Configuration file
├── main/
├── components/
└── hf-espidf-project-tools/    # ← Scripts directory (default)
    ├── build_app.sh
    ├── flash_app.sh
    ├── config_loader.sh
    └── ...

# Mode 1: Project-Integrated (alternative)
your-esp32-project/
├── app_config.yml              # ← Configuration file
├── main/
├── components/
└── scripts/                    # ← Scripts directory (alternative)
    ├── build_app.sh
    ├── flash_app.sh
    ├── config_loader.sh
    └── ...

# Mode 2: Portable Tools
/opt/esp32-tools/               # ← Any location
├── build_app.sh
├── flash_app.sh
├── config_loader.sh
└── ...

# Usage with --project-path
/opt/esp32-tools/build_app.sh --project-path /path/to/project app_name build_type
```

### **Core Scripts**

| Script | Purpose | Dependencies | Usage Modes |
|--------|---------|--------------|-------------|
| `build_app.sh` | Main build script | `config_loader.sh` | Both modes |
| `flash_app.sh` | Device flashing | `config_loader.sh` | Both modes |
| `config_loader.sh` | Configuration parsing | `yq`, `sed` | Both modes |
| `manage_idf.sh` | ESP-IDF management | `setup_common.sh` | Both modes |
| `manage_logs.sh` | Log management | System tools | Both modes |
| `detect_ports.sh` | Serial port detection | System tools | Both modes |
| `get_app_info.py` | App information | `pyyaml` | Both modes |
| `generate_matrix.py` | CI matrix generation | `pyyaml` | Both modes |

### **Configuration Discovery**

Both modes automatically discover `app_config.yml`:

| Mode | Discovery Method | Config Location |
|------|------------------|-----------------|
| **Project-Integrated** | Parent directory of scripts | `project/app_config.yml` |
| **Portable** | `--project-path` or `PROJECT_PATH` | `specified-path/app_config.yml` |

### **Data Flow**

```
Configuration Discovery:
├── Mode 1 (default): hf-espidf-project-tools/ → parent directory → app_config.yml
├── Mode 1 (alternative): scripts/ → parent directory → app_config.yml
└── Mode 2: --project-path → specified directory → app_config.yml

Build Process:
app_config.yml → config_loader.sh → build_app.sh → flash_app.sh
```

### **🆕 New Environment Setup Architecture**

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           SETUP COMMON FUNCTIONS                            │
│                    (setup_common.sh - shared utilities)                     │
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
│  🏠 LOCAL DEVELOPMENT (setup_repo.sh)        🏭 CI/CD (Direct ESP-IDF)      │
│  • Full development environment              • ESP-IDF CI action handles     │
│  • Interactive user setup                    • Direct project building       │
│  • Complete tool installation                • Cache-aware installation     │
│  • Environment variables setup               • Build directory preparation  │
│  • Development aliases                       • CI-specific optimizations    │
│  • ESP-IDF auto-installation                • ESP-IDF handled by CI action  │
└─────────────────────┬───────────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           BUILD SYSTEM INTEGRATION                          │
├─────────────────────────────────────────────────────────────────────────────┤
│  • build_app.sh uses environment from setup                                 │
│  • flash_app.sh integrates with setup                                       │
│  • CI workflows use ESP-IDF CI action directly                              │
│  • Local development uses setup_repo.sh for environment                     │
└─────────────────────────────────────────────────────────────────────────────┘
```

### **Script Categories**

1. **Configuration Scripts** - Load and parse configuration files
2. **Setup Scripts** - Environment setup with clear separation of concerns
3. **Build Scripts** - Application building and management
4. **Utility Scripts** - Helper functions and tools
5. **CI/CD Scripts** - Automation and integration

---

## 🛡️ **Enhanced Validation System**

The build system now includes a comprehensive validation system that prevents invalid build combinations and provides clear guidance to users.

### **Validation Features**

- **🔍 Combination Validation** - Validates app + build type + IDF version combinations
- **🚫 Invalid Build Prevention** - Blocks builds with unsupported combinations
- **💡 Smart Error Messages** - Clear guidance on what combinations are allowed
- **🧠 Smart Defaults** - Automatic ESP-IDF version selection when not specified

### **Validation Flow**

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           BUILD REQUEST                                     │
│  app: gpio_test, build_type: Release, idf_version: (unspecified)            │
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
│  • Result: release/v5.5                                                     │
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
│  ✅ VALID: gpio_test + Release + release/v5.5                               │
│  → Proceed with build                                                       │
│                                                                             │
│  ❌ INVALID: gpio_test + Release + release/v5.4                             │
│  → Show error with valid combinations                                       │
│  → Provide helpful next steps                                               │
└─────────────────────────────────────────────────────────────────────────────┘
```

**Key Optimization Points:**
- **Early Exit**: Basic validation happens first, failing fast on invalid inputs
- **Smart Defaults**: IDF version selection only occurs after basic validation passes
- **Function Safety**: Individual validation functions remain standalone-safe for independent sourcing
- **No Redundancy**: Combination validation doesn't repeat basic checks already performed

---

## 📁 **Script Reference**

### **File Structure**

```
scripts/
├── 📄 app_config.yml           # Centralized configuration
├── 📄 generate_matrix.py       # CI matrix generator
├── 📄 config_loader.sh         # Configuration utilities
├── 📄 setup_common.sh          # Shared setup functions for all environments
├── 📄 setup_repo.sh            # Local development setup (full environment)
├── 📄 build_app.sh             # Main build script
├── 📄 flash_app.sh             # Flashing and monitoring
└── 📄 README.md                # This documentation
```

### **Script Dependencies**

| Script | Dependencies | Purpose | Used By |
|--------|--------------|---------|---------|
| `app_config.yml` | None | Configuration source | All scripts |
| `generate_matrix.py` | `pyyaml` | CI matrix generation | CI/CD |
| `config_loader.sh` | `yq`, `sed` | Configuration parsing | Build scripts |
| `setup_common.sh` | System tools | Shared functions for all environments | Setup scripts |
| `setup_repo.sh` | `setup_common.sh` | Local development (full environment) | Development |
| `build_app.sh` | `config_loader.sh` | Application building | All users |
| `flash_app.sh` | `config_loader.sh` | Device flashing | Development |

---

## 🔧 **Core Scripts**

### **`build_app.sh` - Main Build Script**

The primary build script that orchestrates the entire build process.

#### **Functionality**
- **ESP-IDF Management** - Auto-detection, installation, and environment setup
- **Configuration Loading** - Loads app configuration and build parameters
- **Enhanced Validation** - Validates build combinations before proceeding
- **Smart Defaults** - Automatic ESP-IDF version selection when not specified
- **Build Execution** - Runs ESP-IDF build with project-specific settings
- **Output Management** - Creates structured build directories and exports paths
- **Error Handling** - Comprehensive error checking and reporting

#### **Usage**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/build_app.sh [OPTIONS] <app_name> <build_type> [idf_version]
```

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/build_app.sh [OPTIONS] <app_name> <build_type> [idf_version]
```

**Mode 2: Portable Tools**
```bash
./build_app.sh [OPTIONS] --project-path <path> <app_name> <build_type> [idf_version]
# OR
export PROJECT_PATH=<path>
./build_app.sh [OPTIONS] <app_name> <build_type> [idf_version]
```

**Options:**
- `-c, --clean` - Clean build (remove existing build directory)
- `-v, --verbose` - Verbose output
- `--project-path <path>` - Path to project directory (portable mode only)
- `-h, --help` - Show this help message

**Arguments:**
- `app_name` - Application name from app_config.yml
- `build_type` - Build type (Debug, Release)
- `idf_version` - ESP-IDF version (optional, uses default if not specified)

**Examples:**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/build_app.sh gpio_test Release
./hf-espidf-project-tools/build_app.sh adc_test Debug release/v5.4
./hf-espidf-project-tools/build_app.sh --clean wifi_test Release
```

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/build_app.sh gpio_test Release
./scripts/build_app.sh adc_test Debug release/v5.4
./scripts/build_app.sh --clean wifi_test Release
```

**Mode 2: Portable Tools**
```bash
./build_app.sh --project-path ~/my-esp32-project gpio_test Release
./build_app.sh --project-path ~/my-esp32-project adc_test Debug release/v5.4
export PROJECT_PATH=~/my-esp32-project
./build_app.sh --clean wifi_test Release
```

**Enhanced Commands:**
```bash
# Show app information
./hf-espidf-project-tools/build_app.sh info <app_name>                    # Mode 1 (default)
./scripts/build_app.sh info <app_name>                                    # Mode 1 (alternative)
./build_app.sh --project-path <path> info <app_name>                      # Mode 2

# Show all valid combinations
./hf-espidf-project-tools/build_app.sh combinations                       # Mode 1 (default)
./scripts/build_app.sh combinations                                       # Mode 1 (alternative)
./build_app.sh --project-path <path> combinations                         # Mode 2

# Validate combination
./hf-espidf-project-tools/build_app.sh validate <app> <type> [idf]        # Mode 1 (default)
./scripts/build_app.sh validate <app> <type> [idf]                        # Mode 1 (alternative)
./build_app.sh --project-path <path> validate <app> <type> [idf]          # Mode 2
```
```

#### **Environment Variables**
```bash
# Exported variables
export ESP32_BUILD_APP_MOST_RECENT_DIRECTORY="$BUILD_DIR"

# Required environment variables
export IDF_TARGET="esp32c6"
export BUILD_TYPE="Release"
export APP_TYPE="gpio_test"
export IDF_VERSION="release/v5.5"
```

#### **Build Process**
1. **Configuration Loading** - Parse `app_config.yml` for build parameters
2. **ESP-IDF Setup** - Auto-detect or install required ESP-IDF version
3. **Environment Preparation** - Source ESP-IDF and set build environment
4. **Build Execution** - Run `idf.py` with project-specific settings
5. **Output Generation** - Create structured build directory
6. **Path Export** - Export build directory path for CI/CD integration

### **`flash_app.sh` - Flashing and Monitoring**

Handles device flashing, monitoring, and related operations.

#### **Functionality**
- **Device Flashing** - Flash firmware to ESP32 devices
- **Serial Monitoring** - Monitor device output and logs
- **Port Detection** - Auto-detect and validate serial ports
- **Error Handling** - Comprehensive error checking and recovery

#### **Usage**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/flash_app.sh <action> [app_name] [build_type]
```

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/flash_app.sh <action> [app_name] [build_type]
```

**Mode 2: Portable Tools**
```bash
./flash_app.sh --project-path <path> <action> [app_name] [build_type]
# OR
export PROJECT_PATH=<path>
./flash_app.sh <action> [app_name] [build_type]
```

**Actions:**
- `flash` - Flash firmware only
- `monitor` - Monitor serial output only
- `flash_monitor` - Flash and then monitor
- `flash_erase` - Erase flash and flash firmware

**Examples:**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/flash_app.sh flash_monitor gpio_test Release
./hf-espidf-project-tools/flash_app.sh monitor
./hf-espidf-project-tools/flash_app.sh flash_erase adc_test Debug
```

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/flash_app.sh flash_monitor gpio_test Release
./scripts/flash_app.sh monitor
./scripts/flash_app.sh flash_erase adc_test Debug
```

**Mode 2: Portable Tools**
```bash
./flash_app.sh --project-path ~/my-esp32-project flash_monitor gpio_test Release
./flash_app.sh --project-path ~/my-esp32-project monitor
export PROJECT_PATH=~/my-esp32-project
./flash_app.sh flash_erase adc_test Debug
```

---

## ⚙️ **Setup Scripts**

### **`setup_common.sh` - Shared Setup Functions**

Contains common functions used by all setup scripts.

#### **Key Functions**
```bash
# ESP-IDF Management
install_esp_idf()           # Install ESP-IDF version
setup_esp_idf_env()         # Setup ESP-IDF environment
get_esp_idf_path()          # Get ESP-IDF installation path

# Development Tools
install_dev_tools()          # Install development tools
install_clang_tools()        # Install Clang toolchain
verify_tools()               # Verify tool installation

# System Utilities
check_system_requirements()  # Check system compatibility
setup_environment()          # Setup common environment
```

#### **ESP-IDF Management**
```bash
# Auto-installation process
install_esp_idf() {
    local version="$1"
    local install_dir="$HOME/esp/esp-idf-${version//\//_}"
    
    # Clone ESP-IDF repository
    git clone --recursive https://github.com/espressif/esp-idf.git "$install_dir"
    
    # Install tools and dependencies
    cd "$install_dir"
    ./install.sh
    
    # Setup environment
    source export.sh
}
```

### **`setup_repo.sh` - Local Development Setup**

Sets up the local development environment.

#### **Functionality**
- **Development Tools** - Install Clang, formatting tools, and analysis tools
- **ESP-IDF Setup** - Configure ESP-IDF environment for local development
- **Environment Variables** - Set development-specific environment variables
- **Tool Verification** - Verify all tools are properly installed
- **Complete Development Environment** - Full toolchain and user guidance

#### **Usage**
```bash
# Setup local development environment
source scripts/setup_repo.sh

# This will:
# 1. Install development tools (clang, clang-format, clang-tidy)
# 2. Setup ESP-IDF environment
# 3. Configure build tools
# 4. Export necessary environment variables
# 5. Provide complete development environment
```

#### **Installed Tools**
- **Clang Toolchain** - C/C++ compiler and tools
- **Clang-Format** - Code formatting tool
- **Clang-Tidy** - Static analysis tool
- **ESP-IDF Tools** - ESP32 development tools
- **Build Tools** - CMake, Ninja, and related tools

### **Direct ESP-IDF CI Action - 🆕 Simplified CI/CD**

The CI workflow now uses the ESP-IDF CI action directly without file copying.

#### **Functionality**
- **No File Copying** - Works directly with project files
- **ESP-IDF CI Action** - Official Espressif action handles environment
- **Direct Building** - Builds from source directory structure
- **Cache Optimized** - Leverages ESP-IDF CI action caching
- **Simplified Workflow** - Fewer moving parts, more reliable

#### **Usage**
```yaml
# In GitHub Actions workflow
- name: Build (espressif/esp-idf-ci-action)
  uses: espressif/esp-idf-ci-action@v1
  with:
    command: |
      cd "${ESP32_PROJECT_PATH}"
      ./scripts/build_app.sh --project-path "${ESP32_PROJECT_PATH}" ...
```

#### **CI Environment Features**
- **Faster CI** - No file copying overhead
- **More Reliable** - Official ESP-IDF action handles environment
- **Simpler** - Fewer scripts to maintain
- **Direct** - Works with project files in place
- **Build Directory Preparation** - Ready for CI builds

---

## 🔄 **Utility Scripts**

### **`config_loader.sh` - Configuration Utilities**

Provides functions for loading and parsing configuration files.

#### **Key Functions**
```bash
# Configuration Loading
load_config()                # Load app_config.yml
get_build_types()            # Get available build types
get_idf_versions()           # Get available IDF versions
get_target()                 # Get target MCU
get_idf_version()            # Get IDF version

# Build Directory Management
get_build_directory()        # Generate build directory name
parse_build_directory()      # Parse build directory components
get_build_component()        # Extract specific component
is_valid_build_directory()   # Validate directory format
list_build_directories()     # List all build directories

# Application Management
get_app_types()              # Get available application types
get_app_description()        # Get application description
is_valid_app_type()          # Validate application type
is_valid_build_type()        # Validate build type
get_project_name()           # Get project name

# 🆕 Enhanced Validation Functions
is_valid_combination()       # Validate app + build type + IDF version combination
get_version_index()          # Get version index for nested array access
get_app_idf_versions()       # Get app-specific IDF versions
get_app_build_types()        # Get app-specific build types
show_valid_combinations()    # Show valid combinations for specific app
get_idf_version_smart()      # Smart IDF version selection with build type matching
```

#### **Configuration Parsing**
```bash
# Load configuration with fallback paths
load_config() {
    local possible_paths=(
        "examples/esp32/app_config.yml"
        "app_config.yml"
        "../app_config.yml"
        "../../examples/esp32/app_config.yml"
    )
    
    for path in "${possible_paths[@]}"; do
        if [[ -f "$path" ]]; then
            echo "$path"
            return 0
        fi
    done
    
    return 1
}
```

#### **Build Directory Generation**
```bash
# Generate structured build directory name
get_build_directory() {
    local app_type="$1"
    local build_type="$2"
    local target="${3:-esp32c6}"
    local idf_version="${4:-release/v5.5}"
    
    # Sanitize IDF version for file system compatibility
    local sanitized_idf_version=$(echo "$idf_version" | sed 's/[^a-zA-Z0-9._-]/_/g')
    
    # Generate directory name
    echo "build-app-${app_type}-type-${build_type}-target-${target}-idf-${sanitized_idf_version}"
}
```

#### **Directory Parsing**
```bash
# Parse build directory components
parse_build_directory() {
    local build_dir="$1"
    
    # Extract components using regex
    local app_type=$(echo "$build_dir" | sed -n 's/.*app-\(.*\)-type.*/\1/p')
    local build_type=$(echo "$build_dir" | sed -n 's/.*type-\([^-]*\)-target.*/\1/p')
    local target=$(echo "$build_dir" | sed -n 's/.*target-\([^-]*\)-idf.*/\1/p')
    local idf_version=$(echo "$build_dir" | sed -n 's/.*idf-\([^-]*\)$/\1/p')
    
    echo "app_type:$app_type"
    echo "build_type:$build_type"
    echo "target:$target"
    echo "idf_version:$idf_version"
}
```

#### **🆕 Enhanced Validation System**
```bash
# Validate complete build combination
is_valid_combination() {
    local app_type="$1"
    local build_type="$2"
    local idf_version="$3"
    
    # Check if app type is valid
    if ! is_valid_app_type "$app_type"; then
        return 1
    fi
    
    # Check if build type is valid
    if ! is_valid_build_type "$build_type"; then
        return 1
    fi
    
    # Check if app supports this IDF version
    local app_idf_versions=$(get_app_idf_versions "$app_type")
    if ! echo "$app_idf_versions" | grep -q "$idf_version"; then
        return 1
    fi
    
    # Check if app supports this build type for this IDF version
    local app_build_types=$(get_app_build_types "$app_type")
    local clean_build_types=$(echo "$app_build_types" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' ' ')
    
    if [[ "$clean_build_types" == *"$build_type"* ]]; then
        return 0
    fi
    
    return 1
}

# Smart IDF version selection
get_idf_version_smart() {
    local app_type="$1"
    local build_type="$2"
    
    # Check for app-specific override first
    if check_yq; then
        local app_idf_versions=$(run_yq ".apps.${app_type}.idf_versions" -r 2>/dev/null)
        if [ "$app_idf_versions" != "null" ] && [ -n "$app_idf_versions" ]; then
            # Extract first version from array and clean it
            local first_version=$(echo "$app_idf_versions" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' '\n' | head -n1 | tr -d ' ')
            
            # For now, just return the first version
            # TODO: Implement build type matching logic
            echo "$first_version"
            return 0
        fi
    fi
    
    # Fallback to global default
    if check_yq; then
        local global_idf_versions=$(run_yq '.metadata.idf_versions' -r)
        if [ "$global_idf_versions" != "null" ] && [ -n "$global_idf_versions" ]; then
            local first_version=$(echo "$global_idf_versions" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' '\n' | head -n1 | tr -d ' ')
            echo "$first_version"
            return 0
        fi
    fi
    
    # Final fallback
    echo "release/v5.5"
}
```

---

## 📊 **Configuration Scripts**

### **`generate_matrix.py` - CI Matrix Generator**

Python script that generates CI/CD build matrices from centralized configuration.

#### **Functionality**
- **Configuration Loading** - Load and parse `app_config.yml` from multiple possible locations
- **Matrix Generation** - Generate GitHub Actions matrix configuration with hierarchical overrides
- **Configuration Validation** - Validate configuration structure and content before processing
- **Flexible Output** - Multiple output formats (JSON, YAML) and destinations (stdout, file)
- **App Filtering** - Filter matrix output for specific applications
- **Verbose Processing** - Detailed processing information and statistics

#### **Command Line Options**
```bash
--help, -h                  # Show comprehensive help message
--output <file>             # Output to file instead of stdout
--format <format>           # Output format: json, yaml (default: json)
--filter <app>              # Filter output for specific app only
--verbose                   # Show detailed processing information
--validate                  # Validate configuration before generating matrix
```

#### **Usage Examples**
**Mode 1: Project-Integrated (default)**
```bash
# Basic usage (output to stdout)
python3 hf-espidf-project-tools/generate_matrix.py

# YAML format output
python3 hf-espidf-project-tools/generate_matrix.py --format yaml

# Filter for specific app
python3 hf-espidf-project-tools/generate_matrix.py --filter gpio_test

# Validate configuration
python3 hf-espidf-project-tools/generate_matrix.py --validate
```

**Mode 1: Project-Integrated (alternative)**
```bash
# Basic usage (output to stdout)
python3 scripts/generate_matrix.py

# YAML format output
python3 scripts/generate_matrix.py --format yaml

# Filter for specific app
python3 scripts/generate_matrix.py --filter gpio_test

# Validate configuration
python3 scripts/generate_matrix.py --validate
```

**Mode 2: Portable Tools**
```bash
# Basic usage with project path
python3 generate_matrix.py --project-path ~/my-esp32-project

# YAML format output
python3 generate_matrix.py --project-path ~/my-esp32-project --format yaml

# Filter for specific app
python3 generate_matrix.py --project-path ~/my-esp32-project --filter gpio_test

# Validate configuration
python3 generate_matrix.py --project-path ~/my-esp32-project --validate
```

**Advanced Examples:**
```bash
# Verbose output with validation
python3 hf-espidf-project-tools/generate_matrix.py --verbose --validate                    # Mode 1 (default)
python3 scripts/generate_matrix.py --verbose --validate                                    # Mode 1 (alternative)
python3 generate_matrix.py --project-path <path> --verbose --validate                     # Mode 2

# Output to file
python3 hf-espidf-project-tools/generate_matrix.py --output matrix.json                   # Mode 1 (default)
python3 scripts/generate_matrix.py --output matrix.json                                   # Mode 1 (alternative)
python3 generate_matrix.py --project-path <path> --output matrix.json                     # Mode 2

# Complex combination
python3 hf-espidf-project-tools/generate_matrix.py --filter wifi_test --validate --verbose --format yaml --output wifi_matrix.yaml
python3 scripts/generate_matrix.py --filter wifi_test --validate --verbose --format yaml --output wifi_matrix.yaml
python3 generate_matrix.py --project-path <path> --filter wifi_test --validate --verbose --format yaml --output wifi_matrix.yaml
```

#### **Output Structure**

**Standard Matrix Output**
```json
{
  "include": [
    {
      "idf_version": "release/v5.5",
      "idf_version_docker": "release-v5.5",
      "build_type": "Debug",
      "app_name": "gpio_test",
      "target": "esp32c6",
      "config_source": "app"
    }
  ]
}
```

**Matrix Entry Fields**
- `idf_version`: Git format for ESP-IDF cloning (e.g., "release/v5.5")
- `idf_version_docker`: Docker-safe format for artifacts (e.g., "release-v5.5")
- `build_type`: Build configuration ("Debug" or "Release")
- `app_name`: Application identifier
- `target`: Target MCU architecture (e.g., "esp32c6")
- `config_source`: Configuration source ("app" for per-app overrides, "global" for defaults)

#### **Configuration Validation**

The script validates:
- **Required Sections**: `metadata` and `apps` sections must exist
- **Data Types**: All configuration values must have correct types
- **Structure**: Nested arrays and object structures are validated
- **Content**: App descriptions, source files, and dependencies are checked

**Validation Output**
```bash
✅ Configuration validation passed
⚠️  3 warnings found

Configuration warnings:
  ⚠️  App 'gpio_test' missing description
  ⚠️  App 'adc_test' missing source_file
  ⚠️  No 'target' specified in metadata, using default
```

#### **Verbose Processing**

With `--verbose` flag, the script shows:
```bash
🔍 Loading configuration...
  📁 Config file: /path/to/app_config.yml
  📊 Apps found: 16
  🎯 Target: esp32c6
  🔧 IDF versions: ['release/v5.5', 'release/v5.4']

🔧 Generating CI matrix...
  📋 Matrix entries: 32

🔍 Filtered matrix for app: gpio_test (2 entries)
```

#### **Configuration Overrides**

**Global Configuration**
```yaml
metadata:
  idf_versions: ["release/v5.5", "release/v5.4"]
  build_types: [["Debug", "Release"], ["Debug", "Release"]]
  target: "esp32c6"
```

**Per-App Overrides**
```yaml
apps:
  gpio_test:
    ci_enabled: true
    description: "GPIO peripheral comprehensive testing"
    source_file: "main/GpioComprehensiveTest.cpp"
    idf_versions: ["release/v5.5"]  # Override global
    build_types: [["Debug", "Release"]]
    
  wifi_test:
    ci_enabled: false  # Exclude from CI
    idf_versions: ["release/v5.4"]  # Specific version only
    build_types: [["Debug"]]  # Limited build types
```

**CI Exclusions**
```yaml
ci_config:
  exclude_combinations:
    - app_name: "bluetooth_test"
      build_type: "Debug"
      idf_version: "release/v5.4"
```

#### **Path Detection**

The script automatically detects configuration files from:
- Workspace root: `examples/esp32/app_config.yml`
- Examples directory: `app_config.yml`
- Scripts directory: `../app_config.yml`
- GitHub workflows: `../../examples/esp32/app_config.yml`
- Absolute path calculation from script location

#### **CI Integration**

**GitHub Actions**

**Mode 1: Project-Integrated (default)**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 hf-espidf-project-tools/generate_matrix.py)
    echo "matrix=$MATRIX" >> $GITHUB_OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    ./hf-espidf-project-tools/build_app.sh ${{ matrix.app_name }} ${{ matrix.build_type }}
```

**Mode 1: Project-Integrated (alternative)**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 scripts/generate_matrix.py)
    echo "matrix=$MATRIX" >> $GITHUB_OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    ./scripts/build_app.sh ${{ matrix.app_name }} ${{ matrix.build_type }}
```

**Mode 2: Portable Tools**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 /opt/esp32-tools/generate_matrix.py --project-path ${{ github.workspace }})
    echo "matrix=$MATRIX" >> $GITHUB_OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    /opt/esp32-tools/build_app.sh --project-path ${{ github.workspace }} ${{ matrix.app_name }} ${{ matrix.build_type }}
```

**GitLab CI**
```yaml
generate_matrix:
  script:
    - python3 scripts/generate_matrix.py --format yaml > matrix.yml
  artifacts:
    paths:
      - matrix.yml
```

**Jenkins Pipeline**
```groovy
pipeline {
  agent any
  stages {
    stage('Generate Matrix') {
      steps {
        script {
          def matrix = sh(
            script: 'python3 scripts/generate_matrix.py --format json',
            returnStdout: true
          ).trim()
          def matrixData = readJSON text: matrix
          env.MATRIX = matrix
        }
      }
    }
  }
}
```

#### **Error Handling**

- **Configuration Not Found**: Clear error messages with attempted paths
- **Invalid YAML**: Syntax validation with helpful error details
- **Validation Failures**: Structured error reporting with exit codes
- **File I/O Errors**: Safe file operations with proper error handling

#### **Performance Features**

- **Smart Path Detection**: Multiple fallback paths for configuration
- **Efficient Parsing**: YAML parsing with error handling
- **Memory Optimization**: Stream processing for large configurations
- **Cache Friendly**: Minimal file I/O operations

### **`app_config.yml` - Configuration File**

YAML configuration file that defines all build parameters.

#### **Structure**
```yaml
# Global metadata
metadata:
  idf_versions: ["release/v5.5", "release/v5.4", "release/v5.3"]
  build_types: [["Debug", "Release"], ["Debug", "Release"], ["Debug"]]
  target: "esp32c6"
  build_directory_pattern: "build-app-{app_type}-type-{build_type}-target-{target}-idf-{idf_version}"

# Application configurations
apps:
  gpio_test:
    ci_enabled: true
    description: "GPIO peripheral comprehensive testing"
    idf_versions: ["release/v5.5"]  # Override global
    build_types: [["Debug", "Release"]]
    
  adc_test:
    ci_enabled: true
    description: "ADC peripheral testing"
    # Uses global configuration
    
  wifi_test:
    ci_enabled: false  # Exclude from CI
    description: "WiFi functionality testing"
    idf_versions: ["release/v5.4"]
    build_types: [["Release"]]

# CI configuration
ci_config:
  exclude_combinations:
    - app_name: "wifi_test"
      idf_version: "release/v5.3"
      build_type: "Release"
```

#### **Configuration Features**
- **Hierarchical Overrides** - Per-app configuration overrides global settings
- **CI Control** - Enable/disable applications in CI builds
- **Build Type Mapping** - Different build types per IDF version
- **Exclusion Rules** - Exclude specific combinations from CI
- **Flexible Patterns** - Customizable build directory naming

---

## 🚀 **Usage Examples**

### **Environment Setup Workflows**

#### **Local Development Setup**
```bash
# Complete local development environment
./setup_repo.sh

# What it provides:
# • Full development toolchain (clang, clang-format, clang-tidy)
# • ESP-IDF auto-installation and configuration
# • Development aliases and environment variables
# • Interactive setup with user guidance
# • Complete dependency installation
```

#### **CI/CD Environment Setup**
```bash
# CI builds use ESP-IDF CI action directly
# No setup needed - ESP-IDF CI action handles everything

# What it provides:
# • Minimal CI dependencies (clang-20, yq, PyYAML)
# • Build directory structure preparation
# • Cache-aware installation
# • Non-interactive operation
# • ESP-IDF handled by ESP-IDF CI action
```

### **New Enhanced Commands**

The build system now includes several new commands for better user experience and validation:

#### **📋 Information Commands**
```bash
# Show detailed information for a specific app
./scripts/build_app.sh info gpio_test

# Show all valid build combinations across all apps
./scripts/build_app.sh combinations

# Validate a specific build combination
./scripts/build_app.sh validate gpio_test Release
./scripts/build_app.sh validate gpio_test Release release/v5.4
```

#### **🛡️ Validation Examples**
```bash
# Valid combination - proceeds with build
./scripts/build_app.sh validate gpio_test Release
# Output: ✅ VALID: This combination is allowed!

# Invalid combination - shows error with guidance
./scripts/build_app.sh validate gpio_test Release release/v5.4
# Output: ❌ INVALID: This combination is not allowed!
#        Valid combinations for 'gpio_test':
#        • release/v5.5: Debug Release
```

#### **🧠 Smart Default Examples**
```bash
# No IDF version specified - uses smart default
./scripts/build_app.sh gpio_test Release
# Output: No IDF version specified, using smart default: release/v5.5

# IDF version explicitly specified
./scripts/build_app.sh gpio_test Release release/v5.5
# Output: Uses specified version directly
```

### **Local Development Workflow**

```bash
# 1. Setup development environment
./setup_repo.sh          # Local development

# 2. Build application
./hf-espidf-project-tools/build_app.sh gpio_test Release

# 3. Flash and monitor
./hf-espidf-project-tools/flash_app.sh flash_monitor gpio_test Release

# 4. Monitor only
./hf-espidf-project-tools/flash_app.sh monitor
```

### **CI/CD Integration**

**Mode 1: Project-Integrated (default)**
```yaml
# GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    ./hf-espidf-project-tools/build_app.sh "${{ matrix.app_name }}" "${{ matrix.build_type }}" "${{ matrix.idf_version }}"
    
    # Capture build directory for artifacts
    echo "build_dir=$ESP32_BUILD_APP_MOST_RECENT_DIRECTORY" >> $GITHUB_OUTPUT
```

**Mode 1: Project-Integrated (alternative)**
```yaml
# GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    ./scripts/build_app.sh "${{ matrix.app_name }}" "${{ matrix.build_type }}" "${{ matrix.idf_version }}"
    
    # Capture build directory for artifacts
    echo "build_dir=$ESP32_BUILD_APP_MOST_RECENT_DIRECTORY" >> $GITHUB_OUTPUT
```

**Mode 2: Portable Tools**
```yaml
# GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    /opt/esp32-tools/build_app.sh --project-path ${{ github.workspace }} "${{ matrix.app_name }}" "${{ matrix.build_type }}" "${{ matrix.idf_version }}"
    
    # Capture build directory for artifacts
    echo "build_dir=$ESP32_BUILD_APP_MOST_RECENT_DIRECTORY" >> $GITHUB_OUTPUT
```

## 🔄 **CI/CD Workflows & Quality Assurance**

This repository includes comprehensive CI/CD workflows that ensure code quality, security, and reliability. All workflows run automatically on every push and pull request.

### **🛠️ Available Workflows**

| Workflow | Purpose | Files Checked | Tools Used |
|----------|---------|---------------|------------|
| **[Lint Check](.github/workflows/tools-ci.yml)** | Code formatting & style | Python, Shell, YAML, Markdown | `black`, `isort`, `flake8`, `shellcheck`, `yamllint`, `markdownlint` |
| **[Security Scan](.github/workflows/tools-ci.yml)** | Vulnerability detection | Dependencies, secrets, code | `pip-audit`, `safety`, `bandit`, `gitleaks`, `CodeQL` |
| **[Documentation](.github/workflows/tools-ci.yml)** | Docs validation | README, docs/, script headers | Custom validation, link checking |
| **[Static Analysis](.github/workflows/tools-ci.yml)** | Code quality | Python files | `pylint`, `pydocstyle`, `radon`, `xenon` |
| **[Link Check](.github/workflows/tools-ci.yml)** | Link validation | All markdown files | `markdown-link-check` |

### **📊 CI Pipeline Overview**

```
┌─────────────────────────────────────────────────────────────────┐
│                    🚀 Push/PR Trigger                          │
└─────────────────────┬───────────────────────────────────────────┘
                      │
        ┌─────────────┼─────────────┐
        │             │             │
        ▼             ▼             ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ 🔍 Lint     │ │ 🛡️ Security │ │ 📚 Docs     │
│ Check       │ │ Scan        │ │ Validation  │
└─────────────┘ └─────────────┘ └─────────────┘
        │             │             │
        └─────────────┼─────────────┘
                      │
        ┌─────────────┼─────────────┐
        │             │             │
        ▼             ▼             ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ 🔬 Static   │ │ 🔗 Link     │ │             │
│ Analysis    │ │ Check       │ │             │
└─────────────┘ └─────────────┘ └─────────────┘
        │             │             │
        └─────────────┼─────────────┘
                      │
                      ▼
            ┌─────────────────┐
            │ 📊 CI Summary   │
            │ & Reporting     │
            └─────────┬───────┘
                      │
            ┌─────────┼─────────┐
            │         │         │
            ▼         ▼         ▼
    ┌─────────────┐ ┌─────────────┐
    │ 💬 PR       │ │ 📦 Artifact │
    │ Comment     │ │ Upload      │
    └─────────────┘ └─────────────┘

All jobs run in PARALLEL for maximum speed! ⚡
```

### **🔍 What Each Workflow Does**

#### **1. Lint Check** 🔍
- **Python Code**: Formats with `black`, sorts imports with `isort`, checks style with `flake8`
- **Shell Scripts**: Validates syntax and best practices with `shellcheck`
- **YAML Files**: Checks syntax and formatting with `yamllint`
- **Markdown**: Validates documentation formatting with `markdownlint`
- **Files**: `generate_matrix.py`, `get_app_info.py`, `*.sh`, `.github/workflows/*.yml`, `*.md`

#### **2. Security Scan** 🛡️
- **Dependency Scanning**: Checks `requirements.txt` for vulnerabilities using `pip-audit` and `safety`
- **Secret Detection**: Scans for accidentally committed API keys, passwords, and tokens using `gitleaks`
- **Code Analysis**: Runs `bandit` to find security issues in Python code
- **Static Analysis**: Uses GitHub's CodeQL to find security vulnerabilities

#### **3. Documentation Validation** 📚
- **Structure Check**: Verifies all required documentation files exist
- **Content Validation**: Ensures documentation is complete and properly formatted
- **Link Verification**: Checks internal documentation links work correctly
- **Generation**: Creates documentation index and summaries

#### **4. Static Analysis** 🔬
- **Code Quality**: Analyzes Python code complexity and maintainability
- **Best Practices**: Checks for common programming mistakes and anti-patterns
- **Documentation**: Validates docstrings and code comments
- **Complexity**: Measures cyclomatic complexity to identify overly complex code

#### **5. Link Check** 🔗
- **Internal Links**: Verifies all markdown links within the repository work
- **External Links**: Checks that external URLs are accessible
- **Cross-References**: Ensures documentation references are valid
- **GitHub Links**: Validates GitHub repository and issue links

### **⚡ Quick Local Testing**

Before pushing, you can run similar checks locally:

```bash
# Python formatting and linting
black --check --diff .
isort --check-only --diff .
flake8 . --max-line-length=100

# Shell script validation
find . -name "*.sh" -exec shellcheck {} \;

# Security checks
pip-audit --requirement requirements.txt
safety check --requirement requirements.txt

# Markdown link checking
markdown-link-check README.md
```

### **📈 CI Performance**

- **Total Runtime**: ~5-10 minutes (all jobs run in parallel)
- **Resource Usage**: Optimized for efficiency
- **Caching**: Uses GitHub Actions caching for faster runs
- **Artifacts**: Generates reports and summaries for download

### **🆘 Troubleshooting CI Issues**

| Issue | Solution |
|-------|----------|
| `black: code formatting issues` | Run `black .` to auto-fix |
| `shellcheck: SC2034` | Remove or use unused variables |
| `flake8: E501` | Break long lines or add `# noqa: E501` |
| `gitleaks: secret detected` | Remove secret and rotate keys |
| `markdown-link-check: 404` | Fix or remove broken links |

For detailed CI documentation, see [`.github/README.md`](.github/README.md).

### **Advanced Configuration**

```bash
# Custom ESP-IDF version
./scripts/build_app.sh gpio_test Release release/v5.4

# Clean build
CLEAN=1 ./scripts/build_app.sh gpio_test Release

# Verbose output
./scripts/build_app.sh --verbose gpio_test Release

# Multiple applications
for app in gpio_test adc_test uart_test; do
    ./scripts/build_app.sh "$app" Release
done
```

### **Configuration Management**

```bash
# List available applications
source scripts/config_loader.sh
get_app_types

# Get application description
get_app_description "gpio_test"

# List build directories
list_build_directories

# Parse build directory
parse_build_directory "build-app-gpio_test-type-Release-target-esp32c6-idf-release_v5_5"
```

---

## 🔍 **Troubleshooting**

### **Common Issues**

#### **ESP-IDF Not Found**
```bash
# Error: ESP-IDF environment not found
# Solution: The build system will auto-install ESP-IDF

# Manual installation if needed:
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git esp-idf-release_v5_5
cd esp-idf-release_v5_5
./install.sh
source export.sh
```

#### **Permission Issues**
```bash
# Error: Permission denied
# Solution: Make scripts executable
chmod +x scripts/*.sh
chmod +x scripts/*.py
```

#### **Python Dependencies**
```bash
# Error: Module not found
# Solution: Install required packages
pip install pyyaml
pip install esptool
```

#### **Build Directory Issues**
```bash
# Error: Invalid build directory name
# Solution: Check app_config.yml build_directory_pattern

# Ensure pattern follows format:
build_directory_pattern: "build-app-{app_type}-type-{build_type}-target-{target}-idf-{idf_version}"
```

#### **🆕 Validation System Issues**
```bash
# Error: Invalid build combination
# Solution: Use validation commands to see what's allowed

# Check valid combinations for specific app:
./scripts/build_app.sh info gpio_test

# See all valid combinations:
./scripts/build_app.sh combinations

# Validate specific combination:
./scripts/build_app.sh validate gpio_test Release release/v5.4

# Common validation errors:
# • App doesn't support requested build type
# • App doesn't support requested IDF version
# • Combination constraint violation
```

#### **Smart Default Issues**
```bash
# Error: Smart default not working
# Solution: Check app_config.yml configuration

# Ensure app has idf_versions defined:
apps:
  gpio_test:
    idf_versions: ["release/v5.5"]
    build_types: ["Debug", "Release"]

# Or check global defaults:
metadata:
  idf_versions: ["release/v5.5", "release/v5.4"]
  build_types: [["Debug", "Release"], ["Debug"]]
```

#### **🆕 Environment Setup Issues**
**Problem**: Setup scripts fail or don't work as expected
**Symptoms**: Setup errors or missing dependencies
**Solutions**:
```bash
# For local development (complete environment)
./setup_repo.sh

# For CI/CD (direct ESP-IDF CI action used)

# Check script permissions
chmod +x scripts/*.sh

# Verify system requirements and dependencies
# Check environment variables and paths
```

### **Debug Mode**

Enable verbose output for debugging:

```bash
# Verbose build
./scripts/build_app.sh --verbose gpio_test Release

# Clean rebuild
CLEAN=1 ./scripts/build_app.sh gpio_test Release

# Check environment
source scripts/setup_repo.sh
echo $IDF_PATH
echo $IDF_TARGET
```

### **Log Files**

Build logs are available in:

```bash
# Build log
cat build-*/log/build.log

# CMake log
cat build-*/CMakeFiles/CMakeOutput.log

# Ninja log
cat build-*/.ninja_log
```

---

## 🤝 **Contributing**

### **Adding New Scripts**

1. **Create Script File**
```bash
   # Create new script
   touch scripts/new_script.sh
   chmod +x scripts/new_script.sh
   ```

2. **Add Documentation**
   - Update this README
   - Add usage examples
   - Document dependencies

3. **Test Integration**
   - Test with existing scripts
   - Verify CI compatibility
   - Check error handling

### **Modifying Existing Scripts**

1. **Backup Original**
   ```bash
   cp scripts/script_name.sh scripts/script_name.sh.backup
   ```

2. **Make Changes**
   - Implement new functionality
   - Maintain backward compatibility
   - Update error handling

3. **Test Changes**
   - Test locally
   - Verify CI compatibility
   - Check all use cases

### **Script Standards**

- **Error Handling** - Comprehensive error checking and reporting
- **Logging** - Detailed logs for debugging
- **Documentation** - Clear usage instructions and examples
- **Testing** - Test all functionality and edge cases
- **CI Compatibility** - Ensure scripts work in CI environment
- **🆕 Environment Separation** - Maintain clear separation between local and CI concerns

---

## 📄 **License**

This project is licensed under the GPL-3.0 License - see the [LICENSE](../../LICENSE) file for details.

---

## 🔗 **Related Documentation**

- [ESP32 Examples README](../README.md) - Examples overview and usage
- [Main Project README](../../README.md) - Project overview and architecture
- [CI/CD Workflows](../../.github/workflows/) - GitHub Actions workflows
- [**CI Pipeline Documentation**](.github/README.md) - Detailed CI/CD workflow explanations
- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/) - ESP-IDF reference

---

<div align="center">

**🚀 Built for ESP32 Development**

*Build system with automated CI/CD integration*

</div>
