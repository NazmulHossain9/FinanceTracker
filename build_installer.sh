#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

echo "=== FinanceTracker – Linux .deb Installer Builder ==="

# 1. Create a stub LICENSE file if missing (required by CPack)
if [ ! -f "$PROJECT_DIR/LICENSE" ]; then
    echo "MIT License – Copyright (c) 2026 Nazmul Hossain" > "$PROJECT_DIR/LICENSE"
fi

# 2. Configure
echo "[1/3] Configuring with CMake..."
cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release

# 3. Build
echo "[2/3] Building..."
cmake --build "$BUILD_DIR" --config Release -j"$(nproc)"

# 4. Package
echo "[3/3] Creating .deb package..."
cd "$BUILD_DIR"
cpack --config CPackConfig.cmake

echo ""
echo "Done! Installer created:"
ls -lh "$BUILD_DIR"/FinanceTracker-*.deb 2>/dev/null || ls -lh "$BUILD_DIR"/*.deb

echo ""
echo "To install:  sudo dpkg -i $BUILD_DIR/FinanceTracker-*.deb"
echo "             sudo apt-get install -f   # fix any missing dependencies"
echo "To remove:   sudo dpkg -r FinanceTracker"
