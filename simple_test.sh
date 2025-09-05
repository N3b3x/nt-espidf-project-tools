#!/bin/bash
cd /workspace/examples/esp32
export PATH="$HOME/.local/bin:$PATH"
echo "Testing build system..."
./scripts/build_app.sh list