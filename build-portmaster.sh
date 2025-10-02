#!/bin/bash
# PortMaster build script for BreakOut
# Builds for both ARM64 and ARM32 using Docker

set -e

PROJECT_DIR=$(pwd)
BUILD_DIR_ARM64="$PROJECT_DIR/build-portmaster-arm64"
BUILD_DIR_ARM32="$PROJECT_DIR/build-portmaster-arm32"

echo "======================================"
echo "BreakOut PortMaster Build Script"
echo "======================================"
echo ""

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo "Error: Docker is not running. Please start Docker first:"
    echo "  sudo systemctl start docker"
    echo "  # or"
    echo "  sudo service docker start"
    exit 1
fi

# Pull Docker images if not present
echo "Pulling PortMaster build images..."
docker pull monkeyx/retro_builder:arm64 || echo "Warning: Could not pull arm64 image"
docker pull monkeyx/retro_builder:arm32 || echo "Warning: Could not pull arm32 image"

echo ""
echo "======================================"
echo "Building for ARM64 (aarch64)"
echo "======================================"

# Create build directory
mkdir -p "$BUILD_DIR_ARM64"

# Build ARM64 version
docker run --rm \
    --platform=linux/arm64 \
    -v "$PROJECT_DIR:/src" \
    -w /src \
    monkeyx/retro_builder:arm64 \
    bash -c "
        mkdir -p build-portmaster-arm64 && \
        cd build-portmaster-arm64 && \
        cmake -DPORTMASTER=ON .. && \
        make clean && \
        make -j\$(nproc)
    "

if [ -f "$BUILD_DIR_ARM64/BreakOut" ]; then
    echo "✓ ARM64 build successful: $BUILD_DIR_ARM64/BreakOut"
else
    echo "✗ ARM64 build failed"
    exit 1
fi

echo ""
echo "======================================"
echo "Building for ARM32 (armhf)"
echo "======================================"

# Create build directory
mkdir -p "$BUILD_DIR_ARM32"

# Build ARM32 version
docker run --rm \
    --platform=linux/armhf \
    -v "$PROJECT_DIR:/src" \
    -w /src \
    monkeyx/retro_builder:arm32 \
    bash -c "
        mkdir -p build-portmaster-arm32 && \
        cd build-portmaster-arm32 && \
        cmake -DPORTMASTER=ON .. && \
        make clean && \
        make -j\$(nproc)
    "

if [ -f "$BUILD_DIR_ARM32/BreakOut" ]; then
    echo "✓ ARM32 build successful: $BUILD_DIR_ARM32/BreakOut"
else
    echo "✗ ARM32 build failed"
    exit 1
fi

echo ""
echo "======================================"
echo "Build Summary"
echo "======================================"
echo "ARM64 binary: $BUILD_DIR_ARM64/BreakOut"
echo "ARM32 binary: $BUILD_DIR_ARM32/BreakOut"
echo ""
echo "Next steps:"
echo "1. Create PortMaster package directory structure"
echo "2. Copy binaries to ports/breakout/"
echo "3. Create launch script"
echo "4. Package for distribution"
echo ""
echo "See: https://portmaster.games/packaging.html"
