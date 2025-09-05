#!/bin/bash
# Test script to verify the ESP32 build system is working

set -e

echo "=== Testing ESP32 Build System ==="

# Set up environment
export PATH="$HOME/.local/bin:$PATH"

# Change to the examples directory
cd /workspace/examples/esp32

echo "Current directory: $(pwd)"
echo "Scripts directory contents:"
ls -la scripts/

echo ""
echo "Testing build_app.sh list command:"
./scripts/build_app.sh list

echo ""
echo "Testing build_app.sh info gpio_test:"
./scripts/build_app.sh info gpio_test

echo ""
echo "Testing build_app.sh validate gpio_test Release:"
./scripts/build_app.sh validate gpio_test Release

echo ""
echo "=== Build System Test Complete ==="