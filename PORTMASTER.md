# PortMaster Build Guide

This document explains how to build BreakOut for PortMaster-compatible devices (Anbernic RG35XX, Miyoo Mini, TrimUI Smart Pro, etc.).

## Prerequisites

1. **Docker** installed and running
   ```bash
   # Check if Docker is installed
   docker --version

   # Start Docker daemon (if not running)
   sudo systemctl start docker
   # or
   sudo service docker start
   ```

2. **Disk space**: ~2GB for Docker images + build artifacts

## Quick Start

### 1. Build Binaries (ARM64 + ARM32)

```bash
./build-portmaster.sh
```

This script will:
- Pull PortMaster Docker images (`monkeyx/retro_builder:arm64` and `:arm32`)
- Build ARM64 binary in `build-portmaster-arm64/`
- Build ARM32 binary in `build-portmaster-arm32/`
- Takes ~5-10 minutes on first run (Docker image download)

### 2. Package for Distribution

```bash
./package-portmaster.sh
```

This script will:
- Create `portmaster-package/breakout/` directory structure
- Copy ARM64 and ARM32 binaries
- Include launcher script and metadata
- Generate `BreakOut.PortMaster.zip`

### 3. Install on Device

```bash
# Copy to your device (via SD card, FTP, etc.)
# Extract BreakOut.PortMaster.zip to:
#   /roms/ports/           (most devices)
#   /opt/tools/PortMaster/ (some CFWs)

# Refresh PortMaster and launch!
```

## Architecture Support

BreakOut builds for both ARM architectures used by handheld devices:

- **ARM64 (aarch64)**: Modern devices (RG35XX H/Plus, Miyoo A30, etc.)
- **ARM32 (armhf)**: Older devices (original RG35XX, Miyoo Mini, etc.)

The launcher script (`BreakOut.sh`) automatically detects and runs the correct binary.

## Resolution & Letterboxing

BreakOut is designed for 960×544 (PS Vita resolution) and will letterbox on devices with different aspect ratios:

| Device          | Native Res  | BreakOut Display |
|-----------------|-------------|------------------|
| RG35XX         | 640×480     | Letterboxed      |
| Miyoo Mini     | 640×480     | Letterboxed      |
| RG35XX Plus    | 720×720     | Letterboxed      |
| TrimUI Smart   | 854×480     | Letterboxed      |
| Anbernic RG406 | 960×544     | Perfect fit!     |

## Manual Build (Without Scripts)

If you prefer manual control:

### ARM64:
```bash
docker run --rm \
    --platform=linux/arm64 \
    -v "$(pwd):/src" \
    -w /src \
    monkeyx/retro_builder:arm64 \
    bash -c "mkdir -p build-portmaster-arm64 && \
             cd build-portmaster-arm64 && \
             cmake -DPORTMASTER=ON .. && \
             make -j$(nproc)"
```

### ARM32:
```bash
docker run --rm \
    --platform=linux/armhf \
    -v "$(pwd):/src" \
    -w /src \
    monkeyx/retro_builder:arm32 \
    bash -c "mkdir -p build-portmaster-arm32 && \
             cd build-portmaster-arm32 && \
             cmake -DPORTMASTER=ON .. && \
             make -j$(nproc)"
```

## Package Structure

```
breakout/
├── BreakOut.sh              # Launcher script
├── BreakOut.aarch64         # ARM64 binary
├── BreakOut.armhf           # ARM32 binary
├── port.json                # PortMaster metadata
├── libs/                    # Shared libraries (future)
└── README.txt               # User instructions
```

## Troubleshooting

### Docker Not Running
```
Error: Cannot connect to the Docker daemon
```
**Solution**: Start Docker daemon
```bash
sudo systemctl start docker
```

### Permission Denied
```
Error: permission denied while trying to connect to Docker
```
**Solution**: Add user to docker group
```bash
sudo usermod -aG docker $USER
newgrp docker
```

### Binary Won't Run on Device
- Check architecture with `uname -m` on device
- Verify PortMaster is installed
- Check `log.txt` in game directory for errors

### Game Crashes on Launch
- Ensure SDL2 is available (PortMaster should provide it)
- Check device has enough RAM (game uses ~50MB)
- Try removing `libs/` directory to force system SDL2

## Testing Without Device

You can test ARM binaries using QEMU:

```bash
# Install QEMU
sudo apt-get install qemu-user-static

# Test ARM64 binary
qemu-aarch64-static build-portmaster-arm64/BreakOut

# Test ARM32 binary
qemu-arm-static build-portmaster-arm32/BreakOut
```

## Resources

- **PortMaster Website**: https://portmaster.games/
- **Packaging Guide**: https://portmaster.games/packaging.html
- **Build Environments**: https://portmaster.games/build-environments.html
- **GitHub**: https://github.com/PortsMaster/PortMaster-New
- **Discord**: https://discord.gg/JZJRvT (community support)

## Supported Devices (Partial List)

- Anbernic: RG35XX, RG35XX Plus, RG35XX H, RG406V/M
- Miyoo: Mini, Mini Plus, A30
- TrimUI: Smart, Smart Pro
- Powkiddy: RGB30, RGB10 MAX
- Retroid: Pocket 2/2S/3/3+
- Many more! Check https://portmaster.games/supported.html

## Controls Mapping

BreakOut uses standard SDL2 game controller mappings:

- **Move Paddle**: D-pad Left/Right or Left Analog X-axis
- **Launch Ball**: A button (confirm)
- **Quit**: SELECT button

These are automatically mapped by PortMaster's `gptokeyb` utility.
