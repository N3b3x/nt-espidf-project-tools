# ESP32 HardFOC Interface Wrapper - Logging System Guide

This document provides comprehensive documentation for the ESP32 logging system, including log
generation, management, analysis, and integration with all scripts.

---

**Navigation**: [← Previous: Configuration System](README*CONFIG*SYSTEM.md) | [Back to
Scripts](../README.md) | [Next: Utility Scripts →](README*UTILITY*SCRIPTS.md)

---

## 📋 **Table of Contents**

- [📋 Overview](#-overview)
- [🏗️ Architecture and Design](#️-architecture-and-design)
- [📝 Log Generation and Capture](#️-log-generation-and-capture)
- [🗂️ Log Management and Organization](#️-log-management-and-organization)
- [🔍 Log Analysis and Search](#️-log-analysis-and-search)
- [🚀 Usage Examples and Patterns](#️-usage-examples-and-patterns)
- [🔧 Integration and Automation](#️-integration-and-automation)
- [🔍 Troubleshooting and Debugging](#️-troubleshooting-and-debugging)
- [📚 Reference and Examples](#️-reference-and-examples)

## 📋 **Overview**

The ESP32 logging system provides comprehensive log capture, management,
and analysis capabilities for all development operations.
It features automatic log generation, intelligent organization, powerful search capabilities,
and seamless integration with the build and flash systems.

### **Core Features**
- **Automatic Log Generation**: Built-in logging for all script operations
- **Intelligent Organization**: Structured log file naming and organization
- **Comprehensive Capture**: Complete capture of build, flash, and monitor output
- **Advanced Search**: Cross-log search and pattern matching capabilities
- **Automatic Rotation**: Smart log management with configurable retention

### **Key Capabilities**
- Automatic log file creation with timestamped naming
- Cross-log search and pattern matching
- Log statistics and analysis tools
- Automatic log rotation and cleanup
- Integration with all ESP32 development scripts
- Professional log formatting and organization

## 🏗️ **Architecture and Design**

### **System Architecture**
```text
Script Execution → Log Capture → Log Storage → Log Management → Log Analysis
      ↓              ↓            ↓            ↓              ↓
Build/Flash     Output        File System   Organization   Search/Stats
Operations      Capture       Storage       & Rotation     & Analysis
```text

### **Component Interaction**
- **Script Execution**: All scripts generate log output
- **Log Capture**: Automatic capture of stdout/stderr
- **Log Storage**: Organized file system storage
- **Log Management**: Automatic rotation and cleanup
- **Log Analysis**: Search, statistics, and reporting

### **Design Principles**
- **Automatic Operation**: Logging works without user intervention
- **Comprehensive Coverage**: Captures all relevant output
- **Intelligent Organization**: Logical file naming and structure
- **Performance Optimized**: Minimal impact on script execution
- **Cross-Platform**: Consistent behavior across operating systems

## 📝 **Log Generation and Capture**

### **Automatic Log Generation**

#### **Built-in Logging**
The logging system automatically generates logs for all major operations:

```bash
## Build operations automatically log
./build*app.sh gpio*test Release --log

## Flash operations automatically log
./flash*app.sh flash*monitor gpio*test Release --log

## Monitor operations automatically log
./flash*app.sh monitor --log
```text

#### **Log File Naming Convention**
Logs follow a consistent naming pattern:

```bash
## Standard log naming format
{app*type}*{build*type}*{timestamp}.log

## Examples:
gpio*test*Release*20250115*143022.log
adc*test*Debug*20250115*150045.log
monitor*session*20250115*151230.log

## Custom log naming
./flash*app.sh flash gpio*test Release --log production*deploy
## Result: production*deploy*20250115*143022.log
```text

#### **Log Content Structure**
Each log file contains comprehensive information:

```bash
## # Log file header
## ESP32 HardFOC Interface Wrapper - Operation Log
Date: 2025-01-15
Time: 14:30:22
Operation: flash*monitor
Application: gpio*test
Build Type: Release
ESP-IDF Version: release/v5.5
Target: esp32c6
## Port: /dev/ttyUSB0
## Operation details
- Command execution details
- Configuration validation
- Port detection results
- Build/flash process output
- Monitor session data
- Error messages and warnings
- Performance metrics
- Completion status
```text

### **Log Capture Mechanisms**

#### **Output Capture**
The system captures all relevant output:

```bash
## Captured output types
- Standard output (stdout)
- Standard error (stderr)
- Command execution results
- Error messages and warnings
- Progress information
- Performance metrics
- Debug information
```text

#### **Integration Points**
Logging integrates with all major systems:

```bash
## Build system integration
- Build process output
- Compilation results
- Build statistics
- Error messages

## Flash system integration
- Port detection results
- Flash operation output
- Monitor session data
- Device communication

## Configuration system integration
- Configuration validation
- Parameter processing
- Environment setup
- Error handling
```text

## 🗂️ **Log Management and Organization**

### **Log Directory Structure**

#### **Standard Organization**
```text
examples/esp32/logs/
├── 2025-01-15/                    # Date-based organization
│   ├── gpio*test*Release*143022.log
│   ├── adc*test*Debug*150045.log
│   └── monitor*session*151230.log
├── latest/                         # Symlinks to most recent logs
│   ├── build*latest.log
│   ├── flash*latest.log
│   └── monitor*latest.log
└── archive/                        # Archived logs
    ├── 2025-01-14/
    └── 2025-01-13/
```text

#### **Automatic Organization**
The system automatically organizes logs:

```bash
## Automatic directory creation
- Date-based subdirectories
- Latest log symlinks
- Archive organization
- Cleanup and rotation
```text

### **Log Rotation and Cleanup**

#### **Automatic Rotation**
```bash
## Rotation policies
- Maximum log files: 50 (configurable)
- Retention period: 30 days (configurable)
- Archive old logs automatically
- Clean up expired logs
```text

#### **Cleanup Operations**
```bash
## Manual cleanup
./manage*logs.sh clean 7          # Keep logs for 7 days
./manage*logs.sh clean 30         # Keep logs for 30 days
./manage*logs.sh clean            # Use default retention

## Automatic cleanup
- Daily cleanup of expired logs
- Archive creation for old logs
- Storage optimization
```text

### **Log File Management**

#### **File Operations**
```bash
## List all logs
./manage*logs.sh list

## View specific log
./manage*logs.sh view gpio*test*Release*20250115*143022.log

## Show latest log
./manage*logs.sh latest

## Get log statistics
./manage*logs.sh stats
```text

#### **Storage Optimization**
```bash
## Storage features
- Automatic compression of old logs
- Efficient file organization
- Configurable retention policies
- Storage usage monitoring
```text

## 🔍 **Log Analysis and Search**

### **Cross-Log Search**

#### **Pattern Search**
```bash
## Search for specific patterns
./manage*logs.sh search "ERROR"           # Find all errors
./manage*logs.sh search "WARNING"         # Find all warnings
./manage*logs.sh search "FAILED"          # Find all failures
./manage*logs.sh search "SUCCESS"         # Find all successes

## Advanced search patterns
./manage*logs.sh search "gpio.*error"     # GPIO-related errors
./manage*logs.sh search "flash.*failed"   # Flash failures
./manage*logs.sh search "build.*success"  # Build successes
```text

#### **Search Capabilities**
The search system provides powerful capabilities:

```bash
## Search features
- Regular expression support
- Case-insensitive search
- Multi-line pattern matching
- Context display
- Result highlighting
- Search result summary
```text

### **Log Statistics and Analysis**

#### **Statistical Analysis**
```bash
## Get log statistics
./manage*logs.sh stats

## Statistics include
- Total log files
- Total log size
- Log age distribution
- Error frequency
- Success rates
- Performance metrics
```text

#### **Trend Analysis**
```bash
## Trend analysis features
- Error rate trends
- Performance trends
- Build success trends
- Flash success trends
- Time-based patterns
```text

### **Log Comparison and Diff**

#### **Cross-Log Comparison**
```bash
## Compare different logs
./manage*logs.sh compare log1.log log2.log

## Comparison features
- Side-by-side comparison
- Difference highlighting
- Common pattern identification
- Change tracking
```text

## 🚀 **Usage Examples and Patterns**

### **Basic Log Management**

#### **1. View Logs**
```bash
## List all available logs
./manage*logs.sh list

## View the most recent log
./manage*logs.sh latest

## View a specific log file
./manage*logs.sh view gpio*test*Release*20250115*143022.log
```text

#### **2. Search Logs**
```bash
## Search for errors
./manage*logs.sh search "ERROR"

## Search for specific patterns
./manage*logs.sh search "build.*failed"
./manage*logs.sh search "flash.*success"

## Search with context
./manage*logs.sh search "gpio.*error" --context 5
```text

#### **3. Log Maintenance**
```bash
## Clean old logs
./manage*logs.sh clean 7          # Keep 7 days
./manage*logs.sh clean 30         # Keep 30 days

## Get log statistics
./manage*logs.sh stats

## Archive old logs
./manage*logs.sh archive
```text

### **Advanced Log Patterns**

#### **1. Development Workflow Logging**
```bash
## Complete development workflow with logging
./build*app.sh gpio*test Debug --log
./flash*app.sh flash*monitor gpio*test Debug --log
./manage*logs.sh latest

## Expected result
- Build log with debug information
- Flash log with port detection
- Monitor log with device output
- Comprehensive development trace
```text

#### **2. Production Deployment Logging**
```bash
## Production deployment with logging
./build*app.sh gpio*test Release --log production*build
./flash*app.sh flash gpio*test Release --log production*deploy
./manage*logs.sh search "production.*success"

## Expected result
- Production build log
- Deployment log
- Success verification
- Audit trail for production
```text

#### **3. Debugging and Troubleshooting**
```bash
## Debugging workflow with logging
./flash*app.sh monitor --log debug*session
## ... perform debugging operations ...
./manage*logs.sh search "ERROR" --context 10
./manage*logs.sh search "WARNING" --context 5

## Expected result
- Complete debugging session log
- Error context and patterns
- Warning identification
- Troubleshooting guidance
```text

### **Log Analysis Workflows**

#### **1. Error Analysis**
```bash
## Comprehensive error analysis
./manage*logs.sh search "ERROR" > errors.txt
./manage*logs.sh search "FAILED" >> errors.txt
./manage*logs.sh search "WARNING" >> warnings.txt

## Analyze error patterns
cat errors.txt | grep -o "ERROR:.*" | sort | uniq -c
cat warnings.txt | grep -o "WARNING:.*" | sort | uniq -c
```text

#### **2. Performance Analysis**
```bash
## Performance analysis
./manage*logs.sh search "build.*completed" | grep -o "in [0-9.]*s"
./manage*logs.sh search "flash.*completed" | grep -o "in [0-9.]*s"

## Performance trends
for log in $(./manage*logs.sh list | grep "build.*Release"); do
    echo "$log: $(./manage*logs.sh search "build.*completed" "$log" | grep -o "in [0-9.]*s")"
done
```text

#### **3. Success Rate Analysis**
```bash
## Success rate analysis
total*ops=$(./manage*logs.sh list | wc -l)
success*ops=$(./manage*logs.sh search "SUCCESS" | wc -l)
error*ops=$(./manage*logs.sh search "ERROR" | wc -l)

echo "Total operations: $total*ops"
echo "Successful operations: $success*ops"
echo "Failed operations: $error*ops"
echo "Success rate: $((success*ops * 100 / total*ops))%"
```text

## 🔧 **Integration and Automation**

### **Script Integration**

#### **Automatic Logging**
All scripts automatically support logging:

```bash
## Build script logging
./build*app.sh gpio*test Release --log

## Flash script logging
./flash*app.sh flash*monitor gpio*test Release --log

## Monitor script logging
./flash*app.sh monitor --log

## Log management integration
./manage*logs.sh latest
./manage*logs.sh search "ERROR"
```text

#### **Log Configuration**
Logging can be configured through environment variables:

```bash
## Logging configuration
export LOG*ENABLED=1              # Enable logging
export LOG*LEVEL=INFO             # Set log level
export LOG*RETENTION*DAYS=30      # Set retention period
export LOG*MAX*FILES=50           # Set maximum log files
export LOG*COMPRESSION=1          # Enable compression
```text

### **CI/CD Integration**

#### **Automated Logging**
```yaml
## GitHub Actions workflow
- name: Build and Flash with Logging
  run: |
    cd examples/esp32
    ./scripts/build*app.sh gpio*test Release --log ci*build
    ./scripts/flash*app.sh flash gpio*test Release --log ci*deploy

- name: Analyze Logs
  run: |
    cd examples/esp32
    ./scripts/manage*logs.sh search "ERROR"
    ./scripts/manage*logs.sh stats
    ./scripts/manage*logs.sh latest
```text

#### **Log Artifacts**
```yaml
## Upload logs as artifacts
- name: Upload Logs
  uses: actions/upload-artifact@v3
  with:
    name: esp32-logs
    path: examples/esp32/logs/
    retention-days: 30
```text

### **Automation Scripts**

#### **Log Analysis Automation**
```bash
#!/bin/bash
## Automated log analysis script

cd examples/esp32

## Generate daily log report
echo "=== Daily Log Report $(date +%Y-%m-%d) ===" > daily*report.txt

## Get log statistics
echo "Log Statistics:" >> daily*report.txt
./scripts/manage*logs.sh stats >> daily*report.txt

## Check for errors
echo -e "\nError Summary:" >> daily*report.txt
./scripts/manage*logs.sh search "ERROR" | head -20 >> daily*report.txt

## Check for warnings
echo -e "\nWarning Summary:" >> daily*report.txt
./scripts/manage*logs.sh search "WARNING" | head -20 >> daily*report.txt

## Send report
mail -s "ESP32 Daily Log Report" admin@example.com < daily*report.txt
```text

#### **Log Cleanup Automation**
```bash
#!/bin/bash
## Automated log cleanup script

cd examples/esp32

## Clean old logs
./scripts/manage*logs.sh clean 30

## Archive old logs
./scripts/manage*logs.sh archive

## Generate cleanup report
echo "Log cleanup completed at $(date)" > cleanup*report.txt
echo "Logs older than 30 days have been cleaned up" >> cleanup*report.txt
echo "Old logs have been archived" >> cleanup*report.txt

## Send cleanup report
mail -s "ESP32 Log Cleanup Report" admin@example.com < cleanup*report.txt
```text

## 🔍 **Troubleshooting and Debugging**

### **Common Log Issues**

#### **1. Log Files Not Created**
**Problem**: Log files are not being generated
**Symptoms**: No log files in logs directory
**Solutions**:
```bash
## Check if logging is enabled
echo $LOG*ENABLED

## Check log directory permissions
ls -la logs/
chmod 755 logs/

## Check script logging support
./flash*app.sh --help | grep -i log

## Enable logging explicitly
export LOG*ENABLED=1
./flash*app.sh flash gpio*test Release --log
```text

#### **2. Log Directory Issues**
**Problem**: Cannot access or create log directory
**Symptoms**: "Permission denied" or "Directory not found" errors
**Solutions**:
```bash
## Check directory existence
ls -la examples/esp32/

## Create log directory
mkdir -p examples/esp32/logs

## Check permissions
ls -la examples/esp32/logs/

## Fix permissions
chmod 755 examples/esp32/logs/
```text

#### **3. Log File Corruption**
**Problem**: Log files are corrupted or incomplete
**Symptoms**: Incomplete log files or parsing errors
**Solutions**:
```bash
## Check log file integrity
./manage*logs.sh view latest.log

## Check file size
ls -la logs/*.log

## Remove corrupted logs
rm logs/corrupted.log

## Regenerate logs
./flash*app.sh flash gpio*test Release --log
```text

### **Debug and Verbose Mode**

#### **Enabling Log Debug Output**
```bash
## Enable log debug mode
export LOG*DEBUG=1
export LOG*VERBOSE=1

## Check log system status
./manage*logs.sh --help
./manage*logs.sh list --verbose

## Debug information available
- Log file creation details
- Log capture process information
- Log organization details
- Search and analysis information
```text

#### **Log System Debugging**
```bash
## Debug log system
debug*log*system() {
    echo "=== Log System Debug ==="
    echo "Log directory: $LOG*DIR"
    echo "Log directory exists: $([ -d "$LOG*DIR" ] && echo "Yes" || echo "No")"
    echo "Log directory writable: $([ -w "$LOG*DIR" ] && echo "Yes" || echo "No")"
    echo "Log files count: $(find "$LOG*DIR" -name "*.log" | wc -l)"
    echo "Latest log: $(./manage*logs.sh latest 2>/dev/null || echo "None")"
}
```text

### **Log Performance Issues**

#### **Large Log Files**
**Problem**: Log files are very large and slow to process
**Symptoms**: Slow search operations or high memory usage
**Solutions**:
```bash
## Check log file sizes
./manage*logs.sh list | xargs ls -lh

## Compress old logs
./manage*logs.sh compress

## Clean old logs
./manage*logs.sh clean 7

## Split large logs
./manage*logs.sh split large*log.log
```text

#### **Search Performance**
**Problem**: Log search operations are slow
**Symptoms**: Long search times or high CPU usage
**Solutions**:
```bash
## Use more specific search patterns
./manage*logs.sh search "ERROR.*gpio"  # More specific than just "ERROR"

## Limit search scope
./manage*logs.sh search "ERROR" --max-results 100

## Use indexed search (if available)
./manage*logs.sh search "ERROR" --indexed

## Search in specific time range
./manage*logs.sh search "ERROR" --since "2025-01-15"
```text

## 📚 **Reference and Examples**

### **Command Reference**

#### **Log Management Commands**
```bash
./manage*logs.sh [command] [options]

## Commands:
##   list                    - List all log files with details
##   view <log*name>        - View a specific log file
##   search <pattern>       - Search across all log files
##   clean [days]           - Clean logs older than N days
##   stats                  - Show log statistics and summary
##   latest                 - Show the most recent log file
##   compare <log1> <log2>  - Compare two log files
##   archive                - Archive old logs
##   compress               - Compress old logs
```text

#### **Log Options**
- **`--verbose`**: Enable verbose output
- **`--context <lines>`**: Show context lines around search results
- **`--max-results <count>`**: Limit search results
- **`--since <date>`**: Search logs since specific date
- **`--until <date>`**: Search logs until specific date
- **`--help`**: Show usage information

#### **Environment Variables**
```bash
## Logging configuration
export LOG*ENABLED=1              # Enable/disable logging
export LOG*LEVEL=INFO             # Set log level (DEBUG, INFO, WARN, ERROR)
export LOG*DIR="logs/"            # Set log directory
export LOG*RETENTION*DAYS=30      # Set log retention period
export LOG*MAX*FILES=50           # Set maximum log files
export LOG*COMPRESSION=1          # Enable/disable compression
export LOG*ROTATION=1             # Enable/disable rotation
export LOG*ARCHIVE=1              # Enable/disable archiving
```text

### **Configuration Examples**

#### **Minimal Log Configuration**
```yaml
## app*config.yml minimal logging configuration
flash*config:
  auto*logging: true
  log*rotation: true
  max*log*files: 30
  log*retention*days: 7
```text

#### **Standard Log Configuration**
```yaml
## app*config.yml standard logging configuration
flash*config:
  auto*logging: true
  log*rotation: true
  max*log*files: 50
  log*retention*days: 30
  log*compression: true
  log*archive: true
  log*format: "detailed"
  log*level: "INFO"
```text

#### **Advanced Log Configuration**
```yaml
## app*config.yml advanced logging configuration
flash*config:
  auto*logging: true
  log*rotation: true
  max*log*files: 100
  log*retention*days: 90
  log*compression: true
  log*archive: true
  log*format: "detailed"
  log*level: "DEBUG"
  
  # Log organization
  log*directory: "logs/"
  log*subdirectories: true
  date*based*organization: true
  
  # Log analysis
  log*indexing: true
  log*search*cache: true
  log*analytics: true
  
  # Performance
  log*buffer*size: "1MB"
  log*flush*interval: 5
  log*async*writing: true
```text

### **Integration Examples**

#### **CMake Integration**
```cmake
## CMakeLists.txt logging integration
cmake*minimum*required(VERSION 3.16)

## Log target integration
add*custom*target(logs
    COMMAND ${CMAKE*SOURCE*DIR}/scripts/manage*logs.sh list
    COMMENT "Listing ESP32 development logs"
)

## Log cleanup target
add*custom*target(clean*logs
    COMMAND ${CMAKE*SOURCE*DIR}/scripts/manage*logs.sh clean 7
    COMMENT "Cleaning logs older than 7 days"
)
```text

#### **CI/CD Integration**
```yaml
## GitHub Actions logging workflow
- name: Setup Logging
  run: |
    cd examples/esp32
    mkdir -p logs
    export LOG*ENABLED=1
    export LOG*LEVEL=DEBUG

- name: Build with Logging
  run: |
    cd examples/esp32
    ./scripts/build*app.sh gpio*test Release --log ci*build

- name: Flash with Logging
  run: |
    cd examples/esp32
    ./scripts/flash*app.sh flash gpio*test Release --log ci*deploy

- name: Analyze Logs
  run: |
    cd examples/esp32
    ./scripts/manage*logs.sh stats
    ./scripts/manage*logs.sh search "ERROR"
    ./scripts/manage*logs.sh search "SUCCESS"

- name: Upload Logs
  uses: actions/upload-artifact@v3
  with:
    name: esp32-logs
    path: examples/esp32/logs/
    retention-days: 30
```text

### **Best Practices**

#### **1. Log Management**
- Always enable logging for important operations
- Use descriptive log names for easy identification
- Regular log cleanup to prevent disk space issues
- Archive important logs for long-term retention

#### **2. Log Analysis**
- Use specific search patterns for better results
- Regular review of error logs for pattern identification
- Monitor log statistics for system health
- Use log analysis for performance optimization

#### **3. Log Integration**
- Integrate logging into all development workflows
- Use logging for CI/CD pipeline monitoring
- Implement automated log analysis and reporting
- Use logs for debugging and troubleshooting

#### **4. Log Performance**
- Monitor log file sizes and growth rates
- Implement log rotation and compression
- Use efficient search patterns
- Regular log maintenance and cleanup

---

**Navigation**: [← Previous: Configuration System](README*CONFIG*SYSTEM.md) | [Back to
Scripts](../README.md) | [Next: Utility Scripts →](README_UTILITY_SCRIPTS.md)
