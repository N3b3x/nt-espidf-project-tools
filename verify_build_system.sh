#!/bin/bash
# Simple verification script for the ESP32 build system

echo "=== ESP32 Build System Verification ==="

# Set up environment
export PATH="$HOME/.local/bin:$PATH"

# Change to the examples directory
cd /workspace/examples/esp32

echo "Current directory: $(pwd)"
echo "Checking if build_app.sh exists and is executable:"
ls -la scripts/build_app.sh

echo ""
echo "Testing build_app.sh list command:"
timeout 30 ./scripts/build_app.sh list || echo "Command timed out or failed"

echo ""
echo "Testing yq parsing:"
timeout 10 yq '.metadata.default_app' app_config.yml || echo "yq command failed"

echo ""
echo "=== Verification Complete ==="