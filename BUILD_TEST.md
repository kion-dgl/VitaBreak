# Minimal Build Test - All Three Platforms

This is a **minimum viable demo** showing white shapes (paddle, ball, bricks) on all three platforms to confirm the build system works.

## What You'll See

- **Black background** (960×544)
- **White rectangle at bottom** (paddle placeholder)
- **White square in center** (ball placeholder)
- **3 rows of white rectangles at top** (42 brick placeholders)
- **ESC key to quit** (Linux/WASM)

No movement, no collision, just a static screen proving SDL2 rendering works.

---

## 1. Linux Build (Desktop)

### Build:
```bash
cd /home/kion/Documents/BreakOut/build
make
```

### Run:
```bash
./BreakOut
```

**Expected**: Window opens with white shapes. Press ESC or close window to quit.

---

## 2. PS Vita Build (VPK)

### Prerequisites:
- VitaSDK installed
- `VITASDK` environment variable set: `export VITASDK=/usr/local/vitasdk`
- Add to PATH: `export PATH=$VITASDK/bin:$PATH`

### Build:
```bash
cd /home/kion/Documents/BreakOut
rm -rf build_vita
mkdir build_vita && cd build_vita

cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake ..
make
```

### Output:
- `BreakOut.vpk` in `build_vita/` directory

### Install on Vita:
```bash
# Option 1: FTP transfer
# Start FTP server on Vita (VitaShell → SELECT button)
curl -T BreakOut.vpk ftp://YOUR_VITA_IP:1337/ux0:/

# Option 2: USB transfer
# Copy BreakOut.vpk to Vita via USB

# Install via VitaShell
# Navigate to BreakOut.vpk, press X to install
```

### Run on Vita:
- Launch "BreakOut" from LiveArea
- **Expected**: Full screen with white shapes
- **Quit**: Press PS button → Close Application

---

## 3. WebAssembly Build (Browser)

### Prerequisites:
Install Emscripten SDK (no sudo required):
```bash
# Clone Emscripten SDK
cd ~
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest
./emsdk install latest
./emsdk activate latest

# Source environment (add to ~/.bashrc for persistence)
source ./emsdk_env.sh
```

### Build:
```bash
cd /home/kion/Documents/BreakOut
rm -rf build_wasm
mkdir build_wasm && cd build_wasm

emcmake cmake ..
emmake make
```

### Output:
- `BreakOut.html` (main file)
- `BreakOut.js` (compiled code)
- `BreakOut.wasm` (WebAssembly binary)

### Run:
```bash
# Serve files (from build_wasm directory)
python3 -m http.server 8000
```

Open browser to: `http://localhost:8000/BreakOut.html`

**Expected**: Canvas with white shapes. Close tab to quit.

---

## Troubleshooting

### Linux: "SDL2 not found"
```bash
sudo apt-get install libsdl2-dev
```

### Vita: "vita.toolchain.cmake not found"
```bash
# Verify VitaSDK is installed
echo $VITASDK
ls $VITASDK/share/vita.toolchain.cmake
```

### WASM: "emcmake: command not found"
```bash
# Re-source Emscripten environment
source ~/emsdk/emsdk_env.sh

# Verify
emcc --version
```

### WASM: "Cross-origin error" or assets not loading
- Must serve via HTTP server (not file://)
- Use `python3 -m http.server` or similar

---

## Current Status

This is **Stage 1 Phase 1 (Minimal Setup)** only. The game is NOT playable yet:

- ❌ No paddle movement
- ❌ No ball physics
- ❌ No collision detection
- ❌ No game logic

**Next Phase**: Continue implementation to add input, physics, and collision (Tasks T013-T055).

---

## Verification Checklist

- [ ] **Linux**: `./build/BreakOut` shows window with white shapes
- [ ] **Vita**: VPK installs and runs on real hardware with white shapes
- [ ] **WASM**: Browser shows canvas with white shapes at `localhost:8000/BreakOut.html`

If all three pass ✅ the build system is confirmed working cross-platform!
