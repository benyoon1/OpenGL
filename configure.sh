#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_DIR="$SCRIPT_DIR"

echo "Configuring CMake build system..."

cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
    -S "$WORKSPACE_DIR" \
    -B "$WORKSPACE_DIR/build" \

echo "CMake configuration completed successfully."
