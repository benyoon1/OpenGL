#!/bin/bash

set -e  # Exit on any error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_DIR="$SCRIPT_DIR"

echo "Building project with CMake..."
echo "Build directory: $WORKSPACE_DIR/build"

# Check if build directory exists
if [ ! -d "$WORKSPACE_DIR/build" ]; then
    echo "Error: Build directory does not exist. Please run configure first."
    exit 1
fi

cmake \
    --build "$WORKSPACE_DIR/build" \
    --config Debug \
    --target all

echo "Build completed successfully."

./build/opengl