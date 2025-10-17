#!/bin/bash

# Test script to verify kernel works
echo "Testing kernel in QEMU with serial output..."
echo "This will run for 3 seconds then exit"
echo "-------------------------------------------"

# Run QEMU with serial output, timeout after 3 seconds
timeout 3 qemu-system-i386 \
    -drive format=raw,file=build/os-image.bin \
    -serial stdio \
    -nographic \
    || true

echo ""
echo "-------------------------------------------"
echo "Test complete. If you see 'Disk read error!' above,"
echo "there's still an issue with disk loading."
echo "If you see the boot messages, it's working!"
