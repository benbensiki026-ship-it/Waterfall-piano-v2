# Installation Guide - Waterfall Piano

This guide provides detailed installation instructions for all supported platforms.

## Table of Contents

- [Linux](#linux)
  - [Ubuntu/Debian](#ubuntudebian)
  - [Fedora/RHEL](#fedorarhel)
  - [Arch Linux](#arch-linux)
- [macOS](#macos)
- [Windows](#windows)
  - [MinGW/MSYS2](#mingwmsys2)
  - [Visual Studio](#visual-studio)
- [Verification](#verification)
- [Troubleshooting](#troubleshooting)

---

## Linux

### Ubuntu/Debian

```bash
# Update package list
sudo apt-get update

# Install SDL2 development libraries
sudo apt-get install libsdl2-dev build-essential

# Install Git (if not already installed)
sudo apt-get install git

# Clone or extract the project
cd /path/to/waterfall-piano

# Build using Make
make

# Or build using CMake
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./bin/waterfall-piano
```

### Fedora/RHEL

```bash
# Install SDL2 development libraries
sudo dnf install SDL2-devel gcc-c++ make cmake

# Clone or extract the project
cd /path/to/waterfall-piano

# Build
make

# Run
./bin/waterfall-piano
```

### Arch Linux

```bash
# Install SDL2 development libraries
sudo pacman -S sdl2 base-devel cmake

# Clone or extract the project
cd /path/to/waterfall-piano

# Build
make

# Run
./bin/waterfall-piano
```

---

## macOS

### Prerequisites

Install Xcode Command Line Tools:
```bash
xcode-select --install
```

### Using Homebrew (Recommended)

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install SDL2
brew install sdl2

# Navigate to project
cd /path/to/waterfall-piano

# Build
make

# Run
./bin/waterfall-piano
```

### Using MacPorts

```bash
# Install MacPorts from https://www.macports.org/

# Install SDL2
sudo port install libsdl2

# Build and run
cd /path/to/waterfall-piano
make
./bin/waterfall-piano
```

---

## Windows

### MinGW/MSYS2 (Recommended)

1. **Install MSYS2** from https://www.msys2.org/

2. **Open MSYS2 MinGW 64-bit terminal** and run:

```bash
# Update package database
pacman -Syu

# Install development tools
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make

# Install SDL2
pacman -S mingw-w64-x86_64-SDL2

# Navigate to project
cd /c/path/to/waterfall-piano

# Build
mingw32-make

# Run
./bin/waterfall-piano.exe
```

### Visual Studio

1. **Install Visual Studio 2019 or later** with C++ development tools

2. **Download SDL2** from https://www.libsdl.org/download-2.0.php
   - Download: "SDL2-devel-2.x.x-VC.zip"
   - Extract to: `C:\SDL2`

3. **Build using CMake GUI:**
   - Open CMake GUI
   - Set source directory to project folder
   - Set build directory to `project/build`
   - Click "Configure"
   - Set `SDL2_PATH` to `C:\SDL2`
   - Click "Generate"
   - Click "Open Project"
   - Build in Visual Studio

4. **Build using command line:**

```cmd
mkdir build
cd build
cmake .. -DSDL2_PATH="C:\SDL2"
cmake --build . --config Release
```

5. **Copy SDL2.dll to executable directory:**

```cmd
copy C:\SDL2\lib\x64\SDL2.dll Release\
```

6. **Run:**

```cmd
Release\waterfall-piano.exe
```

### Using vcpkg

```cmd
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install SDL2
.\vcpkg install sdl2:x64-windows

# Integrate with Visual Studio
.\vcpkg integrate install

# Build project
cd \path\to\waterfall-piano
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

---

## Verification

After installation, verify everything works:

```bash
# Check if executable exists
ls -l bin/waterfall-piano  # Linux/Mac
dir bin\waterfall-piano.exe  # Windows

# Run without MIDI file (interactive mode)
./bin/waterfall-piano

# Test with a MIDI file (if available)
./bin/waterfall-piano assets/example.mid
```

**Expected behavior:**
- Window opens showing piano keyboard at bottom
- Black background with waterfall area above
- No errors in terminal
- Can click keys with mouse
- ESC key closes window

---

## Troubleshooting

### SDL2 not found during build

**Linux:**
```bash
# Verify SDL2 is installed
pkg-config --modversion sdl2
sdl2-config --version

# If not found, reinstall
sudo apt-get install --reinstall libsdl2-dev
```

**macOS:**
```bash
# Verify SDL2 installation
brew list sdl2
brew info sdl2

# If issues, reinstall
brew reinstall sdl2
```

**Windows:**
- Ensure SDL2 is in your PATH
- Set `SDL2_DIR` environment variable
- Use absolute path in CMake: `-DSDL2_PATH="C:\SDL2"`

### Compiler not found

**Linux:**
```bash
sudo apt-get install build-essential
```

**macOS:**
```bash
xcode-select --install
```

**Windows:**
- Install Visual Studio with C++ tools
- Or install MinGW through MSYS2

### Make not found

**Linux:**
```bash
sudo apt-get install make
```

**macOS:**
```bash
xcode-select --install
```

**Windows (MSYS2):**
```bash
pacman -S make
```

### CMake not found

**Linux:**
```bash
sudo apt-get install cmake
```

**macOS:**
```bash
brew install cmake
```

**Windows:**
- Download from https://cmake.org/download/
- Or use Visual Studio installer

### Runtime: "SDL2.dll not found" (Windows)

Copy SDL2.dll to the same directory as the .exe:

```cmd
copy C:\SDL2\lib\x64\SDL2.dll bin\
```

Or add SDL2 to your system PATH:
1. Control Panel → System → Advanced → Environment Variables
2. Add `C:\SDL2\lib\x64` to PATH

### "Permission denied" when running

**Linux/Mac:**
```bash
chmod +x bin/waterfall-piano
```

### Black screen or crash on startup

1. Update graphics drivers
2. Check SDL2 version: `sdl2-config --version` (need 2.0.5+)
3. Run from terminal to see error messages
4. Disable hardware acceleration in code if needed

---

## Installing System-Wide (Optional)

### Linux

```bash
sudo make install
# Now you can run from anywhere:
waterfall-piano
```

### macOS

```bash
sudo make install
# Or copy to Applications:
sudo cp bin/waterfall-piano /Applications/
```

### Windows

Add the `bin` directory to your system PATH, or copy the executable to a directory already in PATH.

---

## Uninstallation

### If installed system-wide

**Linux/Mac:**
```bash
sudo rm /usr/local/bin/waterfall-piano
```

### If built locally

Simply delete the project directory:
```bash
rm -rf /path/to/waterfall-piano
```

---

## Getting Help

If you encounter issues not covered here:

1. Check the main README.md
2. Verify SDL2 installation: `sdl2-config --version`
3. Check compiler version: `g++ --version` or `clang --version`
4. Run with verbose output: `./bin/waterfall-piano -v`
5. Open an issue on GitHub with:
   - Your OS and version
   - SDL2 version
   - Compiler version
   - Full error message
   - Build commands used

---

## Next Steps

After successful installation:

1. Read the [Usage Guide](README.md#usage)
2. Try the example MIDI files in `assets/`
3. Learn the [keyboard controls](README.md#keyboard-controls)
4. Explore configuration options in `include/WaterfallPiano.h`

Enjoy your Waterfall Piano! 🎹
