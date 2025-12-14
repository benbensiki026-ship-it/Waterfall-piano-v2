#!/bin/bash
# Build script for Waterfall Piano

set -e  # Exit on error

echo "=========================================="
echo "  Waterfall Piano - Build Script"
echo "=========================================="
echo ""

# Detect OS
OS="$(uname -s)"
echo "Detected OS: $OS"
echo ""

# Check for SDL2
echo "Checking for SDL2..."
if command -v sdl2-config &> /dev/null; then
    echo "✓ SDL2 found: $(sdl2-config --version)"
else
    echo "✗ SDL2 not found!"
    echo ""
    echo "Please install SDL2:"
    case "$OS" in
        Linux*)
            echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev"
            echo "  Fedora:        sudo dnf install SDL2-devel"
            echo "  Arch:          sudo pacman -S sdl2"
            ;;
        Darwin*)
            echo "  macOS:         brew install sdl2"
            ;;
        *)
            echo "  See: https://www.libsdl.org/download-2.0.php"
            ;;
    esac
    exit 1
fi
echo ""

# Choose build method
echo "Select build method:"
echo "  1) Make (fast, simple)"
echo "  2) CMake (cross-platform)"
read -p "Enter choice [1]: " choice
choice=${choice:-1}
echo ""

case $choice in
    1)
        echo "Building with Make..."
        make clean
        make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)
        echo ""
        echo "✓ Build complete!"
        echo "  Executable: ./bin/waterfall-piano"
        ;;
    2)
        echo "Building with CMake..."
        rm -rf build
        mkdir -p build
        cd build
        cmake ..
        cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)
        cd ..
        echo ""
        echo "✓ Build complete!"
        echo "  Executable: ./build/waterfall-piano"
        ;;
    *)
        echo "Invalid choice!"
        exit 1
        ;;
esac

echo ""
echo "=========================================="
echo "  Build completed successfully!"
echo "=========================================="
echo ""
echo "Run the program:"
echo "  ./bin/waterfall-piano"
echo "  ./bin/waterfall-piano [midi-file.mid]"
echo ""
echo "Examples:"
echo "  ./bin/waterfall-piano"
echo "  ./bin/waterfall-piano assets/example.mid"
echo ""
