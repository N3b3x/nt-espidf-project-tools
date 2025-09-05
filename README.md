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

This repository contains a **decoupled build system** for ESP32 development that can be used in two
flexible ways.
The scripts feature automated ESP-IDF management, build processes,
and CI/CD integration while maintaining complete portability.

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
- **🚀 CI Pipeline Optimization** - Optimized CI pipeline with parallel execution, smart
  caching, and reliable tool installation

## 🔄 **Two Usage Modes**

### **Mode 1: Project-Integrated Scripts** 📁
**When**: Scripts are cloned/copied into your ESP32 project
**Location**: `your-project/hf-espidf-project-tools/` (default) or `your-project/scripts/` (with
special setup)
**Configuration**: Automatically finds `app*config.yml` in project root
**Usage**: `./hf-espidf-project-tools/build*app.sh app*name build*type`

```bash
## Project structure (default)
your-esp32-project/
├── app*config.yml          # ← Scripts find this automatically
├── main/
├── components/
└── hf-espidf-project-tools/  # ← Scripts live here (default)
    ├── build*app.sh
    ├── flash*app.sh
    └── config*loader.sh

## Usage (from project root)
./hf-espidf-project-tools/build*app.sh gpio*test Release
./hf-espidf-project-tools/flash*app.sh flash*monitor adc*test
```text

**Alternative: `scripts/` Directory** (requires special setup)
```bash
## Project structure (alternative)
your-esp32-project/
├── app*config.yml          # ← Scripts find this automatically
├── main/
├── components/
└── scripts/                # ← Scripts live here (alternative)
    ├── build*app.sh
    ├── flash*app.sh
    └── config*loader.sh

## Usage (from project root)
./scripts/build*app.sh gpio*test Release
./scripts/flash*app.sh flash*monitor adc*test
```text

### **Mode 2: Portable Tools** 🚀
**When**: Scripts are placed anywhere (shared tools, CI systems, etc.)
**Location**: Any directory (`/opt/esp32-tools/`, `~/tools/`, CI runners, etc.)
**Configuration**: Use `--project-path` flag or `PROJECT*PATH` environment variable
**Usage**: `./build*app.sh --project-path /path/to/project app*name build*type`

```bash
## Shared tools structure
/opt/esp32-tools/
├── build*app.sh
├── flash*app.sh
└── config*loader.sh

## Usage (from anywhere)
/opt/esp32-tools/build*app.sh --project-path ~/my-esp32-project gpio*test Release
/opt/esp32-tools/flash*app.sh --project-path ~/my-esp32-project flash*monitor adc*test

## Or with environment variable
export PROJECT*PATH=~/my-esp32-project
/opt/esp32-tools/build*app.sh gpio*test Release
```yaml

### **Configuration Discovery** 🔍

Both modes automatically discover the `app*config.yml` configuration file:

| Mode | Discovery Method | Config Location |

|------|------------------|-----------------|

| **Project-Integrated** | Parent directory of scripts | `project/app*config.yml` |

| **Portable** | `--project-path` or `PROJECT*PATH` | `specified-path/app*config.yml` |

**Configuration Priority:**
1. `--project-path` flag (highest priority)
2. `PROJECT*PATH` environment variable
3. Default: parent directory of script location

## 📥 **Getting Started - Project Integration**

### **Method 1: Default Setup (Recommended)** 🚀

Clone the repository directly into your ESP32 project:

```bash
## Navigate to your ESP32 project
cd ~/my-esp32-project

## Clone the repository
git clone https://github.com/N3b3x/hf-espidf-project-tools.git

## Your project structure will be:
## my-esp32-project/
## ├── app*config.yml
## ├── main/
## ├── components/
## └── hf-espidf-project-tools/    # ← Repository cloned here
##     ├── build*app.sh
##     ├── flash*app.sh
##     └── ...

## Usage (from project root)
./hf-espidf-project-tools/build*app.sh gpio*test Release
./hf-espidf-project-tools/flash*app.sh flash*monitor adc*test
```text

**Benefits:**
- ✅ Simple one-command setup
- ✅ Full control over scripts
- ✅ Easy to update with `git pull`
- ✅ Can modify scripts if needed

### **Method 2: Git Submodule** 🔗

Keep the scripts as a separate repository while integrating them:

```bash
## Navigate to your ESP32 project
cd ~/my-esp32-project

## Add as submodule
git submodule add https://github.com/N3b3x/hf-espidf-project-tools.git

## Initialize and update the submodule
git submodule update --init --recursive

## Your project structure will be:
## my-esp32-project/
## ├── app*config.yml
## ├── main/
## ├── components/
## └── hf-espidf-project-tools/    # ← Submodule here
##     ├── build*app.sh
##     ├── flash*app.sh
##     └── ...

## Usage (from project root)
./hf-espidf-project-tools/build*app.sh gpio*test Release
```text

**Benefits:**
- ✅ Keeps scripts as separate repository
- ✅ Easy to update with `git submodule update`
- ✅ Maintains version control
- ✅ No file duplication

### **Method 3: Custom `scripts/` Directory** (Advanced) ⚙️

If you prefer the scripts to be in a `scripts/` directory, you can achieve this with special setup:

```bash
## Navigate to your ESP32 project
cd ~/my-esp32-project

## Clone with custom directory name
git clone https://github.com/N3b3x/hf-espidf-project-tools.git scripts

## OR add as submodule with custom name
git submodule add --name scripts https://github.com/N3b3x/hf-espidf-project-tools.git scripts

## Your project structure will be:
## my-esp32-project/
## ├── app*config.yml
## ├── main/
## ├── components/
## └── scripts/                    # ← Custom directory name
##     ├── build*app.sh
##     ├── flash*app.sh
##     └── ...

## Usage (from project root)
./scripts/build*app.sh gpio*test Release
./scripts/flash*app.sh flash*monitor adc*test
```text

**Note:** This method requires special setup and is not the default behavior.
The scripts will work the same way,
but you'll need to use `./scripts/` instead of `./hf-espidf-project-tools/` in all commands.

### **Verification** ✅

After setup, verify the integration works:

```bash
## Navigate to your project root
cd ~/my-esp32-project

## Test script discovery (default setup)
./hf-espidf-project-tools/build*app.sh --help

## OR test script discovery (custom scripts/ setup)
./scripts/build*app.sh --help

## Test configuration loading
./hf-espidf-project-tools/build*app.sh combinations
## OR
./scripts/build*app.sh combinations

## Should show available apps and build combinations
```text

---

## 🚀 **Usage Examples**

### **Mode 1: Project-Integrated Usage** 📁

When scripts are part of your ESP32 project:

```bash
## Project structure (default)
my-esp32-project/
├── app*config.yml
├── main/
├── components/
└── hf-espidf-project-tools/    # ← Scripts are here (default)
    ├── build*app.sh
    ├── flash*app.sh
    └── config*loader.sh

## Usage (from project root)
./hf-espidf-project-tools/build*app.sh gpio*test Release
./hf-espidf-project-tools/flash*app.sh flash*monitor adc*test
./hf-espidf-project-tools/manage*idf.sh list

## Python scripts
python3 hf-espidf-project-tools/get*app*info.py list
python3 hf-espidf-project-tools/generate*matrix.py
```text

**Alternative: Custom `scripts/` Directory**
```bash
## Project structure (alternative)
my-esp32-project/
├── app*config.yml
├── main/
├── components/
└── scripts/                    # ← Scripts are here (alternative)
    ├── build*app.sh
    ├── flash*app.sh
    └── config*loader.sh

## Usage (from project root)
./scripts/build*app.sh gpio*test Release
./scripts/flash*app.sh flash*monitor adc*test
./scripts/manage*idf.sh list

## Python scripts
python3 scripts/get*app*info.py list
python3 scripts/generate*matrix.py
```text

### **Mode 2: Portable Tools Usage** 🚀

When scripts are shared tools or in CI systems:

```bash
## Shared tools structure
/opt/esp32-tools/
├── build*app.sh
├── flash*app.sh
└── config*loader.sh

## Usage with --project-path flag
/opt/esp32-tools/build*app.sh --project-path ~/my-esp32-project gpio*test Release
/opt/esp32-tools/flash*app.sh --project-path ~/my-esp32-project flash*monitor adc*test

## Usage with environment variable
export PROJECT*PATH=~/my-esp32-project
/opt/esp32-tools/build*app.sh gpio*test Release
/opt/esp32-tools/flash*app.sh flash*monitor adc*test

## Python scripts
python3 /opt/esp32-tools/get*app*info.py list --project-path ~/my-esp32-project
python3 /opt/esp32-tools/generate*matrix.py --project-path ~/my-esp32-project
```text

### **Real-World Scenarios**

```bash
## Scenario 1: Multiple ESP32 projects with shared tools
/opt/esp32-tools/build*app.sh --project-path ~/projects/robot-controller gpio*test Release
/opt/esp32-tools/build*app.sh --project-path ~/projects/sensor-node adc*test Debug

## Scenario 2: CI/CD with portable scripts
./ci-scripts/build*app.sh --project-path $GITHUB*WORKSPACE/examples/esp32 gpio*test Release

## Scenario 3: Development with project-integrated scripts
cd ~/my-esp32-project
./hf-espidf-project-tools/build*app.sh gpio*test Release
./hf-espidf-project-tools/flash*app.sh flash*monitor gpio*test

## Scenario 4: Mixed usage (some projects integrated, some portable)
cd ~/project-with-tools
./hf-espidf-project-tools/build*app.sh gpio*test Release

cd ~/project-without-tools
/opt/esp32-tools/build*app.sh --project-path . gpio*test Release
```text

---

## 🏗️ **Script Architecture**

### **Decoupled Design**

The scripts are designed to work in **two flexible modes** while maintaining the same core
functionality:

```text
## Mode 1: Project-Integrated (default)
your-esp32-project/
├── app*config.yml              # ← Configuration file
├── main/
├── components/
└── hf-espidf-project-tools/    # ← Scripts directory (default)
    ├── build*app.sh
    ├── flash*app.sh
    ├── config*loader.sh
    └── ...

## Mode 1: Project-Integrated (alternative)
your-esp32-project/
├── app*config.yml              # ← Configuration file
├── main/
├── components/
└── scripts/                    # ← Scripts directory (alternative)
    ├── build*app.sh
    ├── flash*app.sh
    ├── config*loader.sh
    └── ...

## Mode 2: Portable Tools
/opt/esp32-tools/               # ← Any location
├── build*app.sh
├── flash*app.sh
├── config*loader.sh
└── ...

## Usage with --project-path
/opt/esp32-tools/build*app.sh --project-path /path/to/project app*name build*type
```yaml

### **Core Scripts**

| Script | Purpose | Dependencies | Usage Modes |

|--------|---------|--------------|-------------|

| `build*app.sh` | Main build script | `config*loader.sh` | Both modes |

| `flash*app.sh` | Device flashing | `config*loader.sh` | Both modes |

| `config*loader.sh` | Configuration parsing | `yq`, `sed` | Both modes |

| `manage*idf.sh` | ESP-IDF management | `setup*common.sh` | Both modes |

| `manage*logs.sh` | Log management | System tools | Both modes |

| `detect*ports.sh` | Serial port detection | System tools | Both modes |

| `get*app*info.py` | App information | `pyyaml` | Both modes |

| `generate*matrix.py` | CI matrix generation | `pyyaml` | Both modes |

### **Configuration Discovery**

Both modes automatically discover `app*config.yml`:

| Mode | Discovery Method | Config Location |

|------|------------------|-----------------|

| **Project-Integrated** | Parent directory of scripts | `project/app*config.yml` |

| **Portable** | `--project-path` or `PROJECT*PATH` | `specified-path/app*config.yml` |

### **Data Flow**

```text
Configuration Discovery:
├── Mode 1 (default): hf-espidf-project-tools/ → parent directory → app*config.yml
├── Mode 1 (alternative): scripts/ → parent directory → app*config.yml
└── Mode 2: --project-path → specified directory → app*config.yml

Build Process:
app*config.yml → config*loader.sh → build*app.sh → flash*app.sh
```text

### **🆕 New Environment Setup Architecture**

```text
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
│  • build*app.sh uses environment from setup                                 │
│  • flash*app.sh integrates with setup                                       │
│  • CI workflows use ESP-IDF CI action directly                              │
│  • Local development uses setup*repo.sh for environment                     │
└─────────────────────────────────────────────────────────────────────────────┘
```text

### **Script Categories**

1. **Configuration Scripts** - Load and parse configuration files
2. **Setup Scripts** - Environment setup with clear separation of concerns
3. **Build Scripts** - Application building and management
4. **Utility Scripts** - Helper functions and tools
5. **CI/CD Scripts** - Automation and integration

---

## 🛡️ **Enhanced Validation System**

The build system now includes a comprehensive validation system that prevents invalid build
combinations and provides clear guidance to users.

### **Validation Features**

- **🔍 Combination Validation** - Validates app + build type + IDF version combinations
- **🚫 Invalid Build Prevention** - Blocks builds with unsupported combinations
- **💡 Smart Error Messages** - Clear guidance on what combinations are allowed
- **🧠 Smart Defaults** - Automatic ESP-IDF version selection when not specified

### **Validation Flow**

```text
┌─────────────────────────────────────────────────────────────────────────────┐
│                           BUILD REQUEST                                     │
│  app: gpio*test, build*type: Release, idf*version: (unspecified)            │
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
│  ✅ VALID: gpio*test + Release + release/v5.5                               │
│  → Proceed with build                                                       │
│                                                                             │
│  ❌ INVALID: gpio*test + Release + release/v5.4                             │
│  → Show error with valid combinations                                       │
│  → Provide helpful next steps                                               │
└─────────────────────────────────────────────────────────────────────────────┘
```text

**Key Optimization Points:**
- **Early Exit**: Basic validation happens first, failing fast on invalid inputs
- **Smart Defaults**: IDF version selection only occurs after basic validation passes
- **Function Safety**: Individual validation functions remain standalone-safe for independent sourcing
- **No Redundancy**: Combination validation doesn't repeat basic checks already performed

---

## 📁 **Script Reference**

### **File Structure**

```text
scripts/
├── 📄 app*config.yml           # Centralized configuration
├── 📄 generate*matrix.py       # CI matrix generator
├── 📄 config*loader.sh         # Configuration utilities
├── 📄 setup*common.sh          # Shared setup functions for all environments
├── 📄 setup*repo.sh            # Local development setup (full environment)
├── 📄 build*app.sh             # Main build script
├── 📄 flash*app.sh             # Flashing and monitoring
└── 📄 README.md                # This documentation
```yaml

### **Script Dependencies**

| Script | Dependencies | Purpose | Used By |

|--------|--------------|---------|---------|

| `app*config.yml` | None | Configuration source | All scripts |

| `generate*matrix.py` | `pyyaml` | CI matrix generation | CI/CD |

| `config*loader.sh` | `yq`, `sed` | Configuration parsing | Build scripts |

| `setup*common.sh` | System tools | Shared functions for all environments | Setup scripts |

| `setup*repo.sh` | `setup*common.sh` | Local development (full environment) | Development |

| `build*app.sh` | `config*loader.sh` | Application building | All users |

| `flash*app.sh` | `config*loader.sh` | Device flashing | Development |

---

## 🔧 **Core Scripts**

### **`build*app.sh` - Main Build Script**

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
./hf-espidf-project-tools/build*app.sh [OPTIONS] <app*name> <build*type> [idf*version]
```text

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/build*app.sh [OPTIONS] <app*name> <build*type> [idf*version]
```text

**Mode 2: Portable Tools**
```bash
./build*app.sh [OPTIONS] --project-path <path> <app*name> <build*type> [idf*version]
## OR
export PROJECT*PATH=<path>
./build*app.sh [OPTIONS] <app*name> <build*type> [idf*version]
```yaml

**Options:**
- `-c, --clean` - Clean build (remove existing build directory)
- `-v, --verbose` - Verbose output
- `--project-path <path>` - Path to project directory (portable mode only)
- `-h, --help` - Show this help message

**Arguments:**
- `app*name` - Application name from app*config.yml
- `build*type` - Build type (Debug, Release)
- `idf*version` - ESP-IDF version (optional, uses default if not specified)

**Examples:**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/build*app.sh gpio*test Release
./hf-espidf-project-tools/build*app.sh adc*test Debug release/v5.4
./hf-espidf-project-tools/build*app.sh --clean wifi*test Release
```text

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/build*app.sh gpio*test Release
./scripts/build*app.sh adc*test Debug release/v5.4
./scripts/build*app.sh --clean wifi*test Release
```text

**Mode 2: Portable Tools**
```bash
./build*app.sh --project-path ~/my-esp32-project gpio*test Release
./build*app.sh --project-path ~/my-esp32-project adc*test Debug release/v5.4
export PROJECT*PATH=~/my-esp32-project
./build*app.sh --clean wifi*test Release
```text

**Enhanced Commands:**
```bash
## Show app information
./hf-espidf-project-tools/build*app.sh info <app*name>                    # Mode 1 (default)
./scripts/build*app.sh info <app*name>                                    # Mode 1 (alternative)
./build*app.sh --project-path <path> info <app*name>                      # Mode 2

## Show all valid combinations
./hf-espidf-project-tools/build*app.sh combinations                       # Mode 1 (default)
./scripts/build*app.sh combinations                                       # Mode 1 (alternative)
./build*app.sh --project-path <path> combinations                         # Mode 2

## Validate combination
./hf-espidf-project-tools/build*app.sh validate <app> <type> [idf]        # Mode 1 (default)
./scripts/build*app.sh validate <app> <type> [idf]                        # Mode 1 (alternative)
./build*app.sh --project-path <path> validate <app> <type> [idf]          # Mode 2
```text
```text

#### **Environment Variables**
```bash
## Exported variables
export ESP32*BUILD*APP*MOST*RECENT*DIRECTORY="$BUILD*DIR"

## Required environment variables
export IDF*TARGET="esp32c6"
export BUILD*TYPE="Release"
export APP*TYPE="gpio*test"
export IDF*VERSION="release/v5.5"
```yaml

#### **Build Process**
1. **Configuration Loading** - Parse `app*config.yml` for build parameters
2. **ESP-IDF Setup** - Auto-detect or install required ESP-IDF version
3. **Environment Preparation** - Source ESP-IDF and set build environment
4. **Build Execution** - Run `idf.py` with project-specific settings
5. **Output Generation** - Create structured build directory
6. **Path Export** - Export build directory path for CI/CD integration

### **`flash*app.sh` - Flashing and Monitoring**

Handles device flashing, monitoring, and related operations.

#### **Functionality**
- **Device Flashing** - Flash firmware to ESP32 devices
- **Serial Monitoring** - Monitor device output and logs
- **Port Detection** - Auto-detect and validate serial ports
- **Error Handling** - Comprehensive error checking and recovery

#### **Usage**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/flash*app.sh <action> [app*name] [build*type]
```text

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/flash*app.sh <action> [app*name] [build*type]
```text

**Mode 2: Portable Tools**
```bash
./flash*app.sh --project-path <path> <action> [app*name] [build*type]
## OR
export PROJECT*PATH=<path>
./flash*app.sh <action> [app*name] [build*type]
```text

**Actions:**
- `flash` - Flash firmware only
- `monitor` - Monitor serial output only
- `flash*monitor` - Flash and then monitor
- `flash*erase` - Erase flash and flash firmware

**Examples:**

**Mode 1: Project-Integrated (default)**
```bash
./hf-espidf-project-tools/flash*app.sh flash*monitor gpio*test Release
./hf-espidf-project-tools/flash*app.sh monitor
./hf-espidf-project-tools/flash*app.sh flash*erase adc*test Debug
```text

**Mode 1: Project-Integrated (alternative)**
```bash
./scripts/flash*app.sh flash*monitor gpio*test Release
./scripts/flash*app.sh monitor
./scripts/flash*app.sh flash*erase adc*test Debug
```text

**Mode 2: Portable Tools**
```bash
./flash*app.sh --project-path ~/my-esp32-project flash*monitor gpio*test Release
./flash*app.sh --project-path ~/my-esp32-project monitor
export PROJECT*PATH=~/my-esp32-project
./flash*app.sh flash*erase adc*test Debug
```text

---

## ⚙️ **Setup Scripts**

### **`setup*common.sh` - Shared Setup Functions**

Contains common functions used by all setup scripts.

#### **Key Functions**
```bash
## ESP-IDF Management
install*esp*idf()           # Install ESP-IDF version
setup*esp*idf*env()         # Setup ESP-IDF environment
get*esp*idf*path()          # Get ESP-IDF installation path

## Development Tools
install*dev*tools()          # Install development tools
install*clang*tools()        # Install Clang toolchain
verify*tools()               # Verify tool installation

## System Utilities
check*system*requirements()  # Check system compatibility
setup*environment()          # Setup common environment
```text

#### **ESP-IDF Management**
```bash
## Auto-installation process
install*esp*idf() {
    local version="$1"
    local install*dir="$HOME/esp/esp-idf-${version//\//*}"
    
    # Clone ESP-IDF repository
    git clone --recursive https://github.com/espressif/esp-idf.git "$install*dir"
    
    # Install tools and dependencies
    cd "$install*dir"
    ./install.sh
    
    # Setup environment
    source export.sh
}
```text

### **`setup*repo.sh` - Local Development Setup**

Sets up the local development environment.

#### **Functionality**
- **Development Tools** - Install Clang, formatting tools, and analysis tools
- **ESP-IDF Setup** - Configure ESP-IDF environment for local development
- **Environment Variables** - Set development-specific environment variables
- **Tool Verification** - Verify all tools are properly installed
- **Complete Development Environment** - Full toolchain and user guidance

#### **Usage**
```bash
## Setup local development environment
source scripts/setup*repo.sh

## This will:
## 1. Install development tools (clang, clang-format, clang-tidy)
## 2. Setup ESP-IDF environment
## 3. Configure build tools
## 4. Export necessary environment variables
## 5. Provide complete development environment
```text

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
## In GitHub Actions workflow
- name: Build (espressif/esp-idf-ci-action)
  uses: espressif/esp-idf-ci-action@v1
  with:
    command: |
      cd "${ESP32*PROJECT*PATH}"
      ./scripts/build*app.sh --project-path "${ESP32*PROJECT*PATH}" ...
```text

#### **CI Environment Features**
- **Faster CI** - No file copying overhead
- **More Reliable** - Official ESP-IDF action handles environment
- **Simpler** - Fewer scripts to maintain
- **Direct** - Works with project files in place
- **Build Directory Preparation** - Ready for CI builds

---

## 🔄 **Utility Scripts**

### **`config*loader.sh` - Configuration Utilities**

Provides functions for loading and parsing configuration files.

#### **Key Functions**
```bash
## Configuration Loading
load*config()                # Load app*config.yml
get*build*types()            # Get available build types
get*idf*versions()           # Get available IDF versions
get*target()                 # Get target MCU
get*idf*version()            # Get IDF version

## Build Directory Management
get*build*directory()        # Generate build directory name
parse*build*directory()      # Parse build directory components
get*build*component()        # Extract specific component
is*valid*build*directory()   # Validate directory format
list*build*directories()     # List all build directories

## Application Management
get*app*types()              # Get available application types
get*app*description()        # Get application description
is*valid*app*type()          # Validate application type
is*valid*build*type()        # Validate build type
get*project*name()           # Get project name

## 🆕 Enhanced Validation Functions
is*valid*combination()       # Validate app + build type + IDF version combination
get*version*index()          # Get version index for nested array access
get*app*idf*versions()       # Get app-specific IDF versions
get*app*build*types()        # Get app-specific build types
show*valid*combinations()    # Show valid combinations for specific app
get*idf*version*smart()      # Smart IDF version selection with build type matching
```text

#### **Configuration Parsing**
```bash
## Load configuration with fallback paths
load*config() {
    local possible*paths=(
        "examples/esp32/app*config.yml"
        "app*config.yml"
        "../app*config.yml"
        "../../examples/esp32/app*config.yml"
    )
    
    for path in "${possible*paths[@]}"; do
        if [[ -f "$path" ]]; then
            echo "$path"
            return 0
        fi
    done
    
    return 1
}
```text

#### **Build Directory Generation**
```bash
## Generate structured build directory name
get*build*directory() {
    local app*type="$1"
    local build*type="$2"
    local target="${3:-esp32c6}"
    local idf*version="${4:-release/v5.5}"
    
    # Sanitize IDF version for file system compatibility
    local sanitized*idf*version=$(echo "$idf*version" | sed 's/[^a-zA-Z0-9.*-]/*/g')
    
    # Generate directory name
    echo "build-app-${app*type}-type-${build*type}-target-${target}-idf-${sanitized*idf*version}"
}
```text

#### **Directory Parsing**
```bash
## Parse build directory components
parse*build*directory() {
    local build*dir="$1"
    
    # Extract components using regex
    local app*type=$(echo "$build*dir" | sed -n 's/.*app-\(.*\)-type.*/\1/p')
    local build*type=$(echo "$build*dir" | sed -n 's/.*type-\([^-]*\)-target.*/\1/p')
    local target=$(echo "$build*dir" | sed -n 's/.*target-\([^-]*\)-idf.*/\1/p')
    local idf*version=$(echo "$build*dir" | sed -n 's/.*idf-\([^-]*\)$/\1/p')
    
    echo "app*type:$app*type"
    echo "build*type:$build*type"
    echo "target:$target"
    echo "idf*version:$idf*version"
}
```text

#### **🆕 Enhanced Validation System**
```bash
## Validate complete build combination
is*valid*combination() {
    local app*type="$1"
    local build*type="$2"
    local idf*version="$3"
    
    # Check if app type is valid
    if ! is*valid*app*type "$app*type"; then
        return 1
    fi
    
    # Check if build type is valid
    if ! is*valid*build*type "$build*type"; then
        return 1
    fi
    
    # Check if app supports this IDF version
    local app*idf*versions=$(get*app*idf*versions "$app*type")
    if ! echo "$app*idf*versions" | grep -q "$idf*version"; then
        return 1
    fi
    
    # Check if app supports this build type for this IDF version
    local app*build*types=$(get*app*build*types "$app*type")
    local clean*build*types=$(echo "$app*build*types" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' ' ')
    
    if [[ "$clean*build*types" == *"$build*type"* ]]; then
        return 0
    fi
    
    return 1
}

## Smart IDF version selection
get*idf*version*smart() {
    local app*type="$1"
    local build*type="$2"
    
    # Check for app-specific override first
    if check*yq; then
        local app*idf*versions=$(run*yq ".apps.${app*type}.idf*versions" -r 2>/dev/null)
        if [ "$app*idf*versions" != "null" ] && [ -n "$app*idf*versions" ]; then
            # Extract first version from array and clean it
            local first*version=$(echo "$app*idf*versions" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' '\n' | head -n1 | tr -d ' ')
            
            # For now, just return the first version
            # TODO: Implement build type matching logic
            echo "$first*version"
            return 0
        fi
    fi
    
    # Fallback to global default
    if check*yq; then
        local global*idf*versions=$(run*yq '.metadata.idf*versions' -r)
        if [ "$global*idf*versions" != "null" ] && [ -n "$global*idf*versions" ]; then
            local first*version=$(echo "$global*idf*versions" | sed 's/\[//g' | sed 's/\]//g' | sed 's/"//g' | tr ',' '\n' | head -n1 | tr -d ' ')
            echo "$first*version"
            return 0
        fi
    fi
    
    # Final fallback
    echo "release/v5.5"
}
```python

---

## 📊 **Configuration Scripts**

### **`generate*matrix.py` - CI Matrix Generator**

Python script that generates CI/CD build matrices from centralized configuration.

#### **Functionality**
- **Configuration Loading** - Load and parse `app*config.yml` from multiple possible locations
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
```text

#### **Usage Examples**
**Mode 1: Project-Integrated (default)**
```bash
## Basic usage (output to stdout)
python3 hf-espidf-project-tools/generate*matrix.py

## YAML format output
python3 hf-espidf-project-tools/generate*matrix.py --format yaml

## Filter for specific app
python3 hf-espidf-project-tools/generate*matrix.py --filter gpio*test

## Validate configuration
python3 hf-espidf-project-tools/generate*matrix.py --validate
```text

**Mode 1: Project-Integrated (alternative)**
```bash
## Basic usage (output to stdout)
python3 scripts/generate*matrix.py

## YAML format output
python3 scripts/generate*matrix.py --format yaml

## Filter for specific app
python3 scripts/generate*matrix.py --filter gpio*test

## Validate configuration
python3 scripts/generate*matrix.py --validate
```text

**Mode 2: Portable Tools**
```bash
## Basic usage with project path
python3 generate*matrix.py --project-path ~/my-esp32-project

## YAML format output
python3 generate*matrix.py --project-path ~/my-esp32-project --format yaml

## Filter for specific app
python3 generate*matrix.py --project-path ~/my-esp32-project --filter gpio*test

## Validate configuration
python3 generate*matrix.py --project-path ~/my-esp32-project --validate
```text

**Advanced Examples:**
```bash
## Verbose output with validation
python3 hf-espidf-project-tools/generate*matrix.py --verbose --validate # Mode 1 (default)
python3 scripts/generate*matrix.py --verbose --validate # Mode 1 (alternative)
python3 generate*matrix.py --project-path <path> --verbose --validate                     # Mode 2

## Output to file
python3 hf-espidf-project-tools/generate*matrix.py --output matrix.json # Mode 1 (default)
python3 scripts/generate*matrix.py --output matrix.json # Mode 1 (alternative)
python3 generate*matrix.py --project-path <path> --output matrix.json                     # Mode 2

## Complex combination
python3 hf-espidf-project-tools/generate*matrix.py --filter wifi*test --validate --verbose --format
yaml --output wifi*matrix.yaml
python3 scripts/generate*matrix.py --filter wifi*test --validate --verbose --format yaml --output
wifi*matrix.yaml
python3 generate*matrix.py --project-path <path> --filter wifi*test --validate --verbose --format
yaml --output wifi*matrix.yaml
```text

#### **Output Structure**

**Standard Matrix Output**
```json
{
  "include": [
    {
      "idf*version": "release/v5.5",
      "idf*version*docker": "release-v5.5",
      "build*type": "Debug",
      "app*name": "gpio*test",
      "target": "esp32c6",
      "config*source": "app"
    }
  ]
}
```text

**Matrix Entry Fields**
- `idf*version`: Git format for ESP-IDF cloning (e.g., "release/v5.5")
- `idf*version*docker`: Docker-safe format for artifacts (e.g., "release-v5.5")
- `build*type`: Build configuration ("Debug" or "Release")
- `app*name`: Application identifier
- `target`: Target MCU architecture (e.g., "esp32c6")
- `config*source`: Configuration source ("app" for per-app overrides, "global" for defaults)

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
  ⚠️  App 'gpio*test' missing description
  ⚠️  App 'adc*test' missing source*file
  ⚠️  No 'target' specified in metadata, using default
```text

#### **Verbose Processing**

With `--verbose` flag, the script shows:
```bash
🔍 Loading configuration...
  📁 Config file: /path/to/app*config.yml
  📊 Apps found: 16
  🎯 Target: esp32c6
  🔧 IDF versions: ['release/v5.5', 'release/v5.4']

🔧 Generating CI matrix...
  📋 Matrix entries: 32

🔍 Filtered matrix for app: gpio*test (2 entries)
```text

#### **Configuration Overrides**

**Global Configuration**
```yaml
metadata:
  idf*versions: ["release/v5.5", "release/v5.4"]
  build*types: [["Debug", "Release"], ["Debug", "Release"]]
  target: "esp32c6"
```text

**Per-App Overrides**
```yaml
apps:
  gpio*test:
    ci*enabled: true
    description: "GPIO peripheral comprehensive testing"
    source*file: "main/GpioComprehensiveTest.cpp"
    idf*versions: ["release/v5.5"]  # Override global
    build*types: [["Debug", "Release"]]
    
  wifi*test:
    ci*enabled: false  # Exclude from CI
    idf*versions: ["release/v5.4"]  # Specific version only
    build*types: [["Debug"]]  # Limited build types
```text

**CI Exclusions**
```yaml
ci*config:
  exclude*combinations:
    - app*name: "bluetooth*test"
      build*type: "Debug"
      idf*version: "release/v5.4"
```yaml

#### **Path Detection**

The script automatically detects configuration files from:
- Workspace root: `examples/esp32/app*config.yml`
- Examples directory: `app*config.yml`
- Scripts directory: `../app*config.yml`
- GitHub workflows: `../../examples/esp32/app*config.yml`
- Absolute path calculation from script location

#### **CI Integration**

**GitHub Actions**

**Mode 1: Project-Integrated (default)**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 hf-espidf-project-tools/generate*matrix.py)
    echo "matrix=$MATRIX" >> $GITHUB*OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    ./hf-espidf-project-tools/build*app.sh ${{ matrix.app*name }} ${{ matrix.build*type }}
```text

**Mode 1: Project-Integrated (alternative)**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 scripts/generate*matrix.py)
    echo "matrix=$MATRIX" >> $GITHUB*OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    ./scripts/build*app.sh ${{ matrix.app*name }} ${{ matrix.build*type }}
```text

**Mode 2: Portable Tools**
```yaml
- name: Generate Build Matrix
  run: |
    MATRIX=$(python3 /opt/esp32-tools/generate*matrix.py --project-path ${{ github.workspace }})
    echo "matrix=$MATRIX" >> $GITHUB*OUTPUT

- name: Build Applications
  strategy:
    matrix: ${{ fromJson(steps.matrix.outputs.matrix) }}
  run: |
    /opt/esp32-tools/build*app.sh --project-path ${{ github.workspace }} ${{ matrix.app*name }} ${{ matrix.build*type }}
```text

**GitLab CI**
```yaml
generate*matrix:
  script:
    - python3 scripts/generate*matrix.py --format yaml > matrix.yml
  artifacts:
    paths:
      - matrix.yml
```text

**Jenkins Pipeline**
```groovy
pipeline {
  agent any
  stages {
    stage('Generate Matrix') {
      steps {
        script {
          def matrix = sh(
            script: 'python3 scripts/generate*matrix.py --format json',
            returnStdout: true
          ).trim()
          def matrixData = readJSON text: matrix
          env.MATRIX = matrix
        }
      }
    }
  }
}
```yaml

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

### **`app*config.yml` - Configuration File**

YAML configuration file that defines all build parameters.

#### **Structure**
```yaml
## Global metadata
metadata:
  idf*versions: ["release/v5.5", "release/v5.4", "release/v5.3"]
  build*types: [["Debug", "Release"], ["Debug", "Release"], ["Debug"]]
  target: "esp32c6"
build*directory*pattern: "build-app-{app*type}-type-{build*type}-target-{target}-idf-{idf*version}"

## Application configurations
apps:
  gpio*test:
    ci*enabled: true
    description: "GPIO peripheral comprehensive testing"
    idf*versions: ["release/v5.5"]  # Override global
    build*types: [["Debug", "Release"]]
    
  adc*test:
    ci*enabled: true
    description: "ADC peripheral testing"
    # Uses global configuration
    
  wifi*test:
    ci*enabled: false  # Exclude from CI
    description: "WiFi functionality testing"
    idf*versions: ["release/v5.4"]
    build*types: [["Release"]]

## CI configuration
ci*config:
  exclude*combinations:
    - app*name: "wifi*test"
      idf*version: "release/v5.3"
      build*type: "Release"
```text

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
## Complete local development environment
./setup*repo.sh

## What it provides:
## • Full development toolchain (clang, clang-format, clang-tidy)
## • ESP-IDF auto-installation and configuration
## • Development aliases and environment variables
## • Interactive setup with user guidance
## • Complete dependency installation
```text

#### **CI/CD Environment Setup**
```bash
## CI builds use ESP-IDF CI action directly
## No setup needed - ESP-IDF CI action handles everything

## What it provides:
## • Minimal CI dependencies (clang-20, yq, PyYAML)
## • Build directory structure preparation
## • Cache-aware installation
## • Non-interactive operation
## • ESP-IDF handled by ESP-IDF CI action
```text

### **New Enhanced Commands**

The build system now includes several new commands for better user experience and validation:

#### **📋 Information Commands**
```bash
## Show detailed information for a specific app
./scripts/build*app.sh info gpio*test

## Show all valid build combinations across all apps
./scripts/build*app.sh combinations

## Validate a specific build combination
./scripts/build*app.sh validate gpio*test Release
./scripts/build*app.sh validate gpio*test Release release/v5.4
```text

#### **🛡️ Validation Examples**
```bash
## Valid combination - proceeds with build
./scripts/build*app.sh validate gpio*test Release
## Output: ✅ VALID: This combination is allowed
## Invalid combination - shows error with guidance
./scripts/build*app.sh validate gpio*test Release release/v5.4
## Output: ❌ INVALID: This combination is not allowed
##        Valid combinations for 'gpio*test':
##        • release/v5.5: Debug Release
```text

#### **🧠 Smart Default Examples**
```bash
## No IDF version specified - uses smart default
./scripts/build*app.sh gpio*test Release
## Output: No IDF version specified, using smart default: release/v5.5

## IDF version explicitly specified
./scripts/build*app.sh gpio*test Release release/v5.5
## Output: Uses specified version directly
```text

### **Local Development Workflow**

```bash
## 1. Setup development environment
./setup*repo.sh          # Local development

## 2. Build application
./hf-espidf-project-tools/build*app.sh gpio*test Release

## 3. Flash and monitor
./hf-espidf-project-tools/flash*app.sh flash*monitor gpio*test Release

## 4. Monitor only
./hf-espidf-project-tools/flash*app.sh monitor
```text

### **CI/CD Integration**

**Mode 1: Project-Integrated (default)**
```yaml
## GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    ./hf-espidf-project-tools/build*app.sh "${{ matrix.app*name }}" "${{ matrix.build*type }}" "${{ matrix.idf*version }}"
    
    # Capture build directory for artifacts
    echo "build*dir=$ESP32*BUILD*APP*MOST*RECENT*DIRECTORY" >> $GITHUB*OUTPUT
```text

**Mode 1: Project-Integrated (alternative)**
```yaml
## GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    ./scripts/build*app.sh "${{ matrix.app*name }}" "${{ matrix.build*type }}" "${{ matrix.idf*version }}"
    
    # Capture build directory for artifacts
    echo "build*dir=$ESP32*BUILD*APP*MOST*RECENT*DIRECTORY" >> $GITHUB*OUTPUT
```text

**Mode 2: Portable Tools**
```yaml
## GitHub Actions workflow
- name: ESP-IDF Build
  run: |
    # Build application directly (no setup needed)
    /opt/esp32-tools/build*app.sh --project-path ${{ github.workspace }} "${{ matrix.app*name }}" "${{ matrix.build*type }}" "${{ matrix.idf*version }}"
    
    # Capture build directory for artifacts
    echo "build*dir=$ESP32*BUILD*APP*MOST*RECENT*DIRECTORY" >> $GITHUB*OUTPUT
```python

## 🔄 **CI/CD Workflows & Quality Assurance**

This repository includes comprehensive CI/CD workflows that ensure code quality, security,
and reliability.
All workflows run automatically on every push and pull request.

### **🛠️ Available Workflows**

| Workflow | Purpose | Files Checked | Tools Used |

|----------|---------|---------------|------------|

| **[Lint Check](.github/workflows/tools-ci.yml)** | Code formatting & style | Python, Shell, YAML, Markdown | `black`, `isort`, `flake8`, `shellcheck`, `yamllint`, `markdownlint` |

| **[Security Scan](.github/workflows/tools-ci.yml)** | Vulnerability detection | Dependencies, secrets, code | `pip-audit`, `safety`, `bandit`, `gitleaks`, `CodeQL` |

| **[Documentation](.github/workflows/tools-ci.yml)** | Docs validation | README, docs/, script headers | Custom validation, link checking |

| **[Static Analysis](.github/workflows/tools-ci.yml)** | Code quality | Python files | `pylint`, `pydocstyle`, `radon`, `xenon` |

| **[Link Check](.github/workflows/tools-ci.yml)** | Link validation | All markdown files | `markdown-link-check` |

### **📊 CI Pipeline Overview**

```text
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
```python

### **🔍 What Each Workflow Does**

#### **1. Lint Check** 🔍
- **Python Code**: Formats with `black`, sorts imports with `isort`, checks style with `flake8`
- **Shell Scripts**: Validates syntax and best practices with `shellcheck`
- **YAML Files**: Checks syntax and formatting with `yamllint`
- **Markdown**: Validates documentation formatting with `markdownlint`
- **Files**: `generate*matrix.py`, `get*app*info.py`, `*.sh`, `.github/workflows/*.yml`, `*.md`

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
## Python formatting and linting
black --check --diff .
isort --check-only --diff .
flake8 . --max-line-length=100

## Shell script validation
find . -name "*.sh" -exec shellcheck {} \;

## Security checks
pip-audit --requirement requirements.txt
safety check --requirement requirements.txt

## Markdown link checking
markdown-link-check README.md
```text

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
## Custom ESP-IDF version
./scripts/build*app.sh gpio*test Release release/v5.4

## Clean build
CLEAN=1 ./scripts/build*app.sh gpio*test Release

## Verbose output
./scripts/build*app.sh --verbose gpio*test Release

## Multiple applications
for app in gpio*test adc*test uart*test; do
    ./scripts/build*app.sh "$app" Release
done
```text

### **Configuration Management**

```bash
## List available applications
source scripts/config*loader.sh
get*app*types

## Get application description
get*app*description "gpio*test"

## List build directories
list*build*directories

## Parse build directory
parse*build*directory "build-app-gpio*test-type-Release-target-esp32c6-idf-release*v5*5"
```text

---

## 🔍 **Troubleshooting**

### **Common Issues**

#### **ESP-IDF Not Found**
```bash
## Error: ESP-IDF environment not found
## Solution: The build system will auto-install ESP-IDF

## Manual installation if needed:
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git esp-idf-release*v5*5
cd esp-idf-release*v5*5
./install.sh
source export.sh
```text

#### **Permission Issues**
```bash
## Error: Permission denied
## Solution: Make scripts executable
chmod +x scripts/*.sh
chmod +x scripts/*.py
```python

#### **Python Dependencies**
```bash
## Error: Module not found
## Solution: Install required packages
pip install pyyaml
pip install esptool
```text

#### **Build Directory Issues**
```bash
## Error: Invalid build directory name
## Solution: Check app*config.yml build*directory*pattern

## Ensure pattern follows format:
build*directory*pattern: "build-app-{app*type}-type-{build*type}-target-{target}-idf-{idf*version}"
```text

#### **🆕 Validation System Issues**
```bash
## Error: Invalid build combination
## Solution: Use validation commands to see what's allowed

## Check valid combinations for specific app:
./scripts/build*app.sh info gpio*test

## See all valid combinations:
./scripts/build*app.sh combinations

## Validate specific combination:
./scripts/build*app.sh validate gpio*test Release release/v5.4

## Common validation errors:
## • App doesn't support requested build type
## • App doesn't support requested IDF version
## • Combination constraint violation
```text

#### **Smart Default Issues**
```bash
## Error: Smart default not working
## Solution: Check app*config.yml configuration

## Ensure app has idf*versions defined:
apps:
  gpio*test:
    idf*versions: ["release/v5.5"]
    build*types: ["Debug", "Release"]

## Or check global defaults:
metadata:
  idf*versions: ["release/v5.5", "release/v5.4"]
  build*types: [["Debug", "Release"], ["Debug"]]
```text

#### **🆕 Environment Setup Issues**
**Problem**: Setup scripts fail or don't work as expected
**Symptoms**: Setup errors or missing dependencies
**Solutions**:
```bash
## For local development (complete environment)
./setup*repo.sh

## For CI/CD (direct ESP-IDF CI action used)

## Check script permissions
chmod +x scripts/*.sh

## Verify system requirements and dependencies
## Check environment variables and paths
```text

### **Debug Mode**

Enable verbose output for debugging:

```bash
## Verbose build
./scripts/build*app.sh --verbose gpio*test Release

## Clean rebuild
CLEAN=1 ./scripts/build*app.sh gpio*test Release

## Check environment
source scripts/setup*repo.sh
echo $IDF*PATH
echo $IDF*TARGET
```text

### **Log Files**

Build logs are available in:

```bash
## Build log
cat build-*/log/build.log

## CMake log
cat build-*/CMakeFiles/CMakeOutput.log

## Ninja log
cat build-*/.ninja*log
```text

---

## 🤝 **Contributing**

### **Adding New Scripts**

1. **Create Script File**
```bash
   # Create new script
   touch scripts/new*script.sh
   chmod +x scripts/new*script.sh
   ```

1. **Add Documentation**
   - Update this README
   - Add usage examples
   - Document dependencies

1. **Test Integration**
   - Test with existing scripts
   - Verify CI compatibility
   - Check error handling

### **Modifying Existing Scripts**

1. **Backup Original**
   ```bash
   cp scripts/script*name.sh scripts/script*name.sh.backup
   ```

1. **Make Changes**
   - Implement new functionality
   - Maintain backward compatibility
   - Update error handling

1. **Test Changes**
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

This project is licensed under the GPL-3.0 License - see the [LICENSE](../../LICENSE) file for
details.

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
