# Multi-Version ESP-IDF Support

This document explains how the ESP32 HardFOC Interface Wrapper supports multiple ESP-IDF versions
for different applications.

## Overview

The system now supports installing and managing multiple ESP-IDF versions simultaneously,
allowing different apps to use different ESP-IDF versions as specified in `app*config.yml`.
This is particularly useful when:

- Some apps require newer ESP-IDF features (e.g., v5.5)
- Some apps need to maintain compatibility with older versions (e.g., v5.4)
- Testing compatibility across different ESP-IDF versions
- CI/CD pipelines that need to build against multiple versions

## Architecture

### Directory Structure

```yaml
$HOME/esp/
├── esp-idf-release*v5.5/     # ESP-IDF v5.5 installation
├── esp-idf-release*v5.4/     # ESP-IDF v5.4 installation
└── esp-idf -> esp-idf-release*v5.5/  # Default symlink
```text

### Version Naming Convention

ESP-IDF versions are stored with forward slashes converted to underscores:
- `release/v5.5` → `esp-idf-release*v5.5/`
- `release/v5.4` → `esp-idf-release*v5.4/`

## Configuration

### app*config.yml Structure

```yaml
metadata:
  # Global ESP-IDF versions - used by all apps unless overridden
  idf*versions: ["release/v5.5", "release/v5.4"]
  # Build types per IDF version: [["Debug", "Release"], ["Debug"]] 
## First array: build types for first IDF version (v5.5), second array: build types for second IDF
version (v5.4)
  default*build*types: [["Debug", "Release"], ["Debug"]]

apps:
  gpio*test:
    # Override global: use only ESP-IDF v5.5 with Debug and Release
    idf*versions: ["release/v5.5"]
    build*types: ["Debug", "Release"]
    
  legacy*app:
    # Override global: use only ESP-IDF v5.4 with Debug only
    idf*versions: ["release/v5.4"]
    build*types: ["Debug"]
```text

### App-Specific Overrides

Apps can override the global ESP-IDF version and build type settings:

1. **No override**: App uses global settings from `metadata.idf*versions`
2. **Partial override**: App specifies only `idf*versions` or only `build*types`
3. **Full override**: App specifies both `idf*versions` and `build*types`

## Management Scripts

### manage*idf.sh

The main script for managing ESP-IDF versions:

```bash
## Install all required ESP-IDF versions from app*config.yml
./manage*idf.sh install

## List installed versions
./manage*idf.sh list

## Export environment for specific version
./manage*idf.sh export release/v5.4

## Switch default version
./manage*idf.sh switch release/v5.4

## Update specific version
./manage*idf.sh update release/v5.5

## Remove specific version
./manage*idf.sh clean release/v5.4

## Show current status
./manage*idf.sh status
```text

### build*unified.sh

Enhanced build script with ESP-IDF version support:

```bash
## Build with specific ESP-IDF version
IDF*VERSION=release/v5.4 APP*TYPE=gpio*test ./build*unified.sh

## List available ESP-IDF versions
./build*unified.sh --list-versions

## Validate configuration before building
./build*unified.sh --prepare-only
```text

### build*app.sh

Standard build script with automatic ESP-IDF version handling:

```bash
## Build with specific ESP-IDF version
./build*app.sh gpio*test Release release/v5.4

## Build with default version
./build*app.sh gpio*test Release
```text

## Usage Examples

### 1. Initial Setup

```bash
## Navigate to examples/esp32/scripts
cd examples/esp32/scripts

## Install all required ESP-IDF versions
./manage*idf.sh install

## Verify installation
./manage*idf.sh status
```text

### 2. Building Different Apps with Different Versions

```bash
## Build gpio*test with ESP-IDF v5.5
./build*app.sh gpio*test Release release/v5.5

## Build legacy*app with ESP-IDF v5.4
./build*app.sh legacy*app Debug release/v5.4
```text

### 3. Manual Version Switching

```bash
## Export ESP-IDF v5.4 environment
./manage*idf.sh export release/v5.4

## Verify environment
echo $IDF*PATH
echo $IDF*VERSION
idf.py --version

## Switch default version
./manage*idf.sh switch release/v5.4
```text

## Environment Variables

### Automatic Export

When using the build scripts, the following environment variables are automatically set:

- `IDF*PATH`: Path to the ESP-IDF installation directory
- `IDF*VERSION`: Current ESP-IDF version being used
- `PATH`: Updated to include ESP-IDF tools

### Manual Export

```bash
## Source the common setup functions
source ./setup*common.sh

## Export specific version
export*esp*idf*version "release/v5.4"
```text

## Validation

### App Compatibility

The system automatically validates:

1. **ESP-IDF Version Compatibility**: Ensures the requested version is supported by the app
2. **Build Type Compatibility**: Ensures the requested build type is supported by the app
3. **Version Availability**: Checks if the requested ESP-IDF version is installed

### Error Handling

```bash
## Example error when app doesn't support ESP-IDF version
ERROR: App 'gpio*test' does not support ESP-IDF version 'release/v5.4'
Supported versions for 'gpio*test': release/v5.5
Global ESP-IDF versions: release/v5.5 release/v5.4

## Example error when ESP-IDF version not installed
ERROR: Failed to source ESP-IDF environment for version release/v5.4
Available versions:
  - release/v5.5
To install required versions, run: ./scripts/manage*idf.sh install
```text

## CI/CD Integration

### GitHub Actions

The multi-version system integrates seamlessly with CI/CD:

```yaml
jobs:
  build-matrix:
    strategy:
      matrix:
        idf*version: [release/v5.5, release/v5.4]
        app*type: [gpio*test, adc*test]
        build*type: [Debug, Release]
    
    steps:
      - uses: actions/checkout@v3
      - name: Setup ESP-IDF ${{ matrix.idf*version }}
        run: |
          cd examples/esp32/scripts
          ./manage*idf.sh install
          ./manage*idf.sh export ${{ matrix.idf*version }}
      
      - name: Build ${{ matrix.app*type }}
        run: |
          cd examples/esp32/scripts
          ./build*app.sh ${{ matrix.app*type }} ${{ matrix.build*type }} ${{ matrix.idf*version }}
```text

### Local Development

```bash
## Quick development cycle with version switching
./manage*idf.sh export release/v5.5
./build*app.sh gpio*test Debug
## ... make changes ...
./build*app.sh gpio*test Debug

## Test with different version
./manage*idf.sh export release/v5.4
./build*app.sh gpio*test Debug
```bash

## Troubleshooting

### Common Issues

1. **ESP-IDF Version Not Found**
   ```bash
   # Install missing versions
   ./manage*idf.sh install
   ```

1. **Environment Not Exported**
   ```bash
   # Manually export environment
   ./manage*idf.sh export release/v5.5
   ```

1. **App Compatibility Issues**
   ```bash
   # Check app requirements
   ./test*multi*idf.sh --test-apps
   ```

1. **Build Failures**
   ```bash
   # Validate configuration
   ./build*unified.sh --prepare-only
   ```

### Debug Mode

Enable verbose output for troubleshooting:

```bash
## Set debug environment variable
export SETUP*DEBUG=1

## Run commands with verbose output
./manage*idf.sh status
./build*app.sh gpio*test Debug release/v5.5
```text

## Best Practices

### 1. Version Management

- Keep only the ESP-IDF versions you actually need
- Regularly update installed versions with `./manage*idf.sh update <version>`
- Use `./manage*idf.sh clean <version>` to remove unused versions

### 2. App Configuration

- Specify app-specific ESP-IDF versions only when necessary
- Use global defaults for most apps to maintain consistency
- Document version requirements in app descriptions

### 3. Build Workflows

- Always validate ESP-IDF version compatibility before building
- Use the `--prepare-only` flag to validate configuration without building
- Test builds with multiple ESP-IDF versions in CI/CD

### 4. Environment Management

- Use `./manage*idf.sh export <version>` for manual development
- Let build scripts handle automatic environment switching
- Avoid manually setting `IDF*PATH` when using the build system

## Future Enhancements

### Planned Features

1. **Automatic Version Detection**: Detect ESP-IDF version from project files
2. **Version Migration**: Automated migration between ESP-IDF versions
3. **Dependency Resolution**: Automatic resolution of ESP-IDF version conflicts
4. **Performance Optimization**: Parallel installation and caching improvements

### Contributing

To contribute to the multi-version ESP-IDF system:

1. Test with multiple ESP-IDF versions
2. Validate app compatibility across versions
3. Update documentation for new features
4. Add tests to `test*multi*idf.sh`

## Conclusion

The multi-version ESP-IDF support provides flexibility and compatibility for the ESP32 HardFOC
Interface Wrapper.
By allowing different apps to use different ESP-IDF versions,
the system can accommodate diverse requirements while maintaining a clean and manageable development
environment.

For questions or issues, refer to the main project documentation or create an issue in the project
repository.
