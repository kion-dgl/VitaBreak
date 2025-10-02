#!/bin/bash
# BreakOut launcher script for PortMaster

XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source $controlfolder/control.txt
[ -f "${controlfolder}/mod_${CFW_NAME}.txt" ] && source "${controlfolder}/mod_${CFW_NAME}.txt"
get_controls

GAMEDIR="/$directory/ports/breakout"

# Exports
export LD_LIBRARY_PATH="$GAMEDIR/libs:$LD_LIBRARY_PATH"
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"

# CD and set permissions
cd $GAMEDIR
> "$GAMEDIR/log.txt" && exec > >(tee "$GAMEDIR/log.txt") 2>&1

# Determine architecture and run appropriate binary
ARCH=$(uname -m)

if [ "$ARCH" = "aarch64" ]; then
    runtime="frt"
    BINARY="$GAMEDIR/BreakOut.aarch64"
elif [ "$ARCH" = "armv7l" ]; then
    runtime="frt"
    BINARY="$GAMEDIR/BreakOut.armhf"
else
    echo "Unsupported architecture: $ARCH"
    sleep 5
    exit 1
fi

echo "Running BreakOut on $ARCH..."
echo "CFW: $CFW_NAME"
echo "Resolution: ${DISPLAY_WIDTH}x${DISPLAY_HEIGHT}"

$GPTOKEYB "BreakOut" &

# Run the game
$BINARY

# Cleanup
$ESUDO kill -9 $(pidof gptokeyb)
unset LD_LIBRARY_PATH
unset SDL_GAMECONTROLLERCONFIG
