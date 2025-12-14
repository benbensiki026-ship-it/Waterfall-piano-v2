# Waterfall Piano - 88 Keys

A beautiful waterfall-style piano visualization application with full 88-key support and MIDI file playback capability. Built with C++ and SDL2.

![Waterfall Piano](https://img.shields.io/badge/Keys-88-blue) ![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20Mac-green) ![License](https://img.shields.io/badge/License-MIT-yellow)

## Features

- **Full 88-Key Piano**: Complete range from A0 to C8
- **Waterfall Visualization**: Beautiful falling notes display
- **MIDI Import**: Load and play standard MIDI files (.mid)
- **Real-time Playback**: Watch notes fall in sync with playback
- **Interactive Mode**: Click keys with mouse to play
- **Velocity-Based Colors**: Visual feedback based on note velocity
- **Playback Controls**: Play, pause, stop, and adjust speed
- **High Performance**: Smooth 60 FPS rendering with SDL2

## Screenshots

```
┌─────────────────────────────────────────────┐
│  ║  ║ ║  ║  ║ ║ ║  ║  ║ ║  ║  ║ ║ ║  ...  │  ← Waterfall (notes falling)
│  ║  ║ ║  ║  ║ ║ ║  ║  ║ ║  ║  ║ ║ ║  ...  │
│▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│
├─────────────────────────────────────────────┤
│█ █ ██ █ █ ██ █ █ █ ██ █ █ ██ █ █ ██ █ █...│  ← Black keys
│ █ ██ █ █ ██ █ █ █ ██ █ █ ██ █ █ ██ █ █ █ │
│███████████████████████████████████████████ │  ← White keys
│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │
└─────────────────────────────────────────────┘
```

## Requirements

### Build Dependencies

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SDL2 library (version 2.0.5 or higher)
- CMake 3.10+ (optional, for CMake build)
- Make (for Makefile build)

### Runtime Dependencies

- SDL2 runtime library

## Installation

### Linux (Ubuntu/Debian)

```bash
# Install SDL2
sudo apt-get update
sudo apt-get install libsdl2-dev

# Clone or extract the project
cd waterfall-piano

# Build with Make
make

# Or build with CMake
mkdir build && cd build
cmake ..
make

# Run
./bin/waterfall-piano [optional-midi-file.mid]
```

### macOS

```bash
# Install SDL2 using Homebrew
brew install sdl2

# Build with Make
make

# Or build with CMake
mkdir build && cd build
cmake ..
make

# Run
./bin/waterfall-piano [optional-midi-file.mid]
```

### Windows

1. Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/download-2.0.php)
2. Extract SDL2 to a known location (e.g., `C:\SDL2`)
3. Set environment variable: `SDL2_DIR=C:\SDL2`

**Option A: MinGW/MSYS2**
```bash
# Install SDL2
pacman -S mingw-w64-x86_64-SDL2

# Build
make

# Run
./bin/waterfall-piano.exe [optional-midi-file.mid]
```

**Option B: Visual Studio with CMake**
```bash
mkdir build && cd build
cmake .. -DSDL2_PATH="C:\SDL2"
cmake --build . --config Release

# Run
Release\waterfall-piano.exe [optional-midi-file.mid]
```

## Building

### Using Make (Recommended)

```bash
# Build
make

# Clean build artifacts
make clean

# Build and run
make run

# Install to system (Linux/Mac)
sudo make install
```

### Using CMake

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Install (optional)
sudo cmake --install .
```

## Usage

### Basic Usage

```bash
# Run without MIDI file (interactive mode)
./bin/waterfall-piano

# Run with MIDI file
./bin/waterfall-piano song.mid

# Examples
./bin/waterfall-piano bach_prelude.mid
./bin/waterfall-piano chopin_nocturne.mid
```

### Keyboard Controls

| Key | Action |
|-----|--------|
| **SPACE** | Play/Pause MIDI playback |
| **S** | Stop playback and reset |
| **+** / **=** | Increase playback speed |
| **-** | Decrease playback speed |
| **H** | Toggle help overlay |
| **ESC** | Quit application |

### Mouse Controls

- **Left Click**: Press piano key
- **Release**: Release piano key
- Works on both white and black keys

## MIDI File Support

The application supports standard MIDI files (.mid) with the following features:

- **Format 0**: Single track
- **Format 1**: Multiple tracks (all tracks merged)
- **Tempo changes**: Supported
- **Note velocity**: Visualized with colors
- **Polyphony**: Unlimited simultaneous notes

### Supported MIDI Events

- Note On (0x90)
- Note Off (0x80)
- Tempo changes (Meta event 0x51)
- Track names (Meta event 0x03)

### MIDI File Recommendations

- Works best with piano or keyboard MIDI files
- Files with extreme note ranges may have some notes off-screen
- Recommended note range: A0 (MIDI 21) to C8 (MIDI 108)

## Configuration

You can modify these constants in `include/WaterfallPiano.h`:

```cpp
// Display settings
const int SCREEN_WIDTH = 1600;      // Window width
const int SCREEN_HEIGHT = 900;      // Window height
const int KEYBOARD_HEIGHT = 150;    // Piano keyboard height

// Colors (RGBA)
const SDL_Color COLOR_WHITE_KEY = {255, 255, 255, 255};
const SDL_Color COLOR_BLACK_KEY = {0, 0, 0, 255};
const SDL_Color COLOR_BACKGROUND = {20, 20, 30, 255};

// Playback
float scrollSpeed = 200.0f;  // Pixels per second for waterfall
float playbackSpeed = 1.0f;  // 1.0 = normal speed
```

## Project Structure

```
waterfall-piano/
├── src/
│   ├── main.cpp              # Entry point
│   ├── WaterfallPiano.cpp    # Main application logic
│   └── MidiParser.cpp        # MIDI file parser
├── include/
│   ├── WaterfallPiano.h      # Main header
│   └── MidiParser.h          # Parser header
├── assets/                   # (Optional) MIDI files for testing
├── CMakeLists.txt            # CMake build configuration
├── Makefile                  # Traditional makefile
└── README.md                 # This file
```

## Troubleshooting

### SDL2 not found

**Linux:**
```bash
sudo apt-get install libsdl2-dev
```

**macOS:**
```bash
brew install sdl2
```

**Windows:**
- Ensure SDL2 is in your PATH or specify SDL2_DIR

### Build errors

```bash
# Clean and rebuild
make clean
make

# Or with CMake
rm -rf build
mkdir build && cd build
cmake ..
make
```

### MIDI file won't load

- Ensure the file is a valid MIDI file (.mid extension)
- Check file permissions
- Try a different MIDI file to verify the program works
- Check console output for specific error messages

### Performance issues

- Reduce window size in `WaterfallPiano.h`
- Lower the scroll speed
- Use MIDI files with fewer simultaneous notes

## Advanced Features

### Planned Features

- [ ] Audio output (synthesizer integration)
- [ ] Record keyboard input to MIDI
- [ ] Multiple color schemes
- [ ] Sustain pedal visualization
- [ ] MIDI input from external keyboards
- [ ] Export visualization to video
- [ ] Touch screen support

### Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest features
- Submit pull requests
- Improve documentation

## Technical Details

### Architecture

- **Rendering**: SDL2 hardware-accelerated rendering
- **MIDI Parsing**: Custom lightweight MIDI parser
- **Performance**: ~60 FPS with hundreds of simultaneous notes
- **Memory**: Efficient note management with automatic cleanup

### Algorithms

- **Note Layout**: Proper piano key positioning with black key offsets
- **Timing**: Precise tick-to-millisecond conversion
- **Collision Detection**: Efficient key click detection
- **Scrolling**: Smooth velocity-based waterfall animation

## License

This project is released under the MIT License. Feel free to use, modify, and distribute.

```
MIT License

Copyright (c) 2024 Waterfall Piano

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

## Credits

- Inspired by various piano visualization projects on GitHub
- Built with SDL2 (Simple DirectMedia Layer)
- MIDI specification: https://www.midi.org/

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Check existing documentation
- Review troubleshooting section

## Version History

- **v1.0.0** (2024-12-14)
  - Initial release
  - Full 88-key support
  - MIDI file import
  - Waterfall visualization
  - Interactive keyboard

---

**Enjoy making music visible!** 🎹🎵
