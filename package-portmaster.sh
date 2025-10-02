#!/bin/bash
# Package BreakOut for PortMaster distribution

set -e

PROJECT_DIR=$(pwd)
PACKAGE_DIR="$PROJECT_DIR/portmaster-package"
PORT_DIR="$PACKAGE_DIR/breakout"

echo "======================================"
echo "BreakOut PortMaster Packaging"
echo "======================================"
echo ""

# Check if binaries exist
if [ ! -f "build-portmaster-arm64/BreakOut" ] || [ ! -f "build-portmaster-arm32/BreakOut" ]; then
    echo "Error: Binaries not found. Run ./build-portmaster.sh first"
    exit 1
fi

# Create package directory structure
echo "Creating package directory structure..."
rm -rf "$PACKAGE_DIR"
mkdir -p "$PORT_DIR"

# Copy binaries
echo "Copying binaries..."
cp build-portmaster-arm64/BreakOut "$PORT_DIR/BreakOut.aarch64"
cp build-portmaster-arm32/BreakOut "$PORT_DIR/BreakOut.armhf"

# Copy launcher script and metadata
echo "Copying launcher and metadata..."
cp portmaster/BreakOut.sh "$PORT_DIR/"
cp portmaster/port.json "$PORT_DIR/"
chmod +x "$PORT_DIR/BreakOut.sh"

# Create libs directory (for future dependencies)
mkdir -p "$PORT_DIR/libs"

# Create README
cat > "$PORT_DIR/README.txt" << 'EOF'
BreakOut for PortMaster
========================

Classic brick-breaking arcade game!

Controls:
- D-pad/Left Analog: Move paddle
- A Button: Launch ball / Start game
- SELECT: Quit

Features:
- Progressive difficulty with multiple stages
- Lives system (3 lives per game)
- Score tracking
- Ball speed increases with collision count

Version: 1.0.0
Built: $(date +%Y-%m-%d)

Source: https://github.com/kion-dgl/VitaBreak
EOF

# Create archive
echo "Creating distribution archive..."
cd "$PACKAGE_DIR"
zip -r ../BreakOut.PortMaster.zip breakout/

cd "$PROJECT_DIR"

echo ""
echo "======================================"
echo "Packaging Complete!"
echo "======================================"
echo "Package location: $PROJECT_DIR/BreakOut.PortMaster.zip"
echo ""
echo "Installation:"
echo "1. Copy BreakOut.PortMaster.zip to your device"
echo "2. Extract to /roms/ports/ (or appropriate PortMaster location)"
echo "3. Refresh PortMaster game list"
echo "4. Launch BreakOut from PortMaster menu"
echo ""
echo "For more info: https://portmaster.games/"
