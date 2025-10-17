#!/bin/bash

# Build the OS image
make clean
make

# Check if build was successful
if [ -f build/os-image.bin ]; then
    echo "Build successful! Starting QEMU..."
    echo "Press Ctrl+A then X to exit QEMU"
    echo "-----------------------------------"
    echo ""

    # Try to run QEMU with different options based on environment
    # Use -nographic for headless mode (WSL2 without X11)
    if [ -z "$DISPLAY" ]; then
        echo "No display detected, using -nographic mode"
        qemu-system-i386 -drive format=raw,file=build/os-image.bin -nographic
    else
        echo "Display detected, using graphical mode"
        qemu-system-i386 -drive format=raw,file=build/os-image.bin
    fi
else
    echo "Build failed! Check the error messages above."
    exit 1
fi
