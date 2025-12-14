# Quick Start Guide - Waterfall Piano

Get up and running in 5 minutes!

## 📦 What You Need

- **Linux/Mac**: GCC/Clang compiler + SDL2
- **Windows**: MinGW or Visual Studio + SDL2

## 🚀 Quick Install

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev build-essential
make
./bin/waterfall-piano
```

### macOS
```bash
brew install sdl2
make
./bin/waterfall-piano
```

### Windows (MSYS2)
```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-gcc
mingw32-make
./bin/waterfall-piano.exe
```

## 🎹 First Run

1. **Without MIDI file** (interactive mode):
   ```bash
   ./bin/waterfall-piano
   ```
   - Click keys with your mouse
   - Press ESC to quit

2. **With MIDI file**:
   ```bash
   ./bin/waterfall-piano song.mid
   ```
   - Press SPACE to play/pause
   - Press S to stop
   - Press +/- to change speed

## ⌨️ Controls

| Key | Action |
|-----|--------|
| SPACE | Play/Pause |
| S | Stop |
| +/- | Speed up/down |
| H | Help |
| ESC | Quit |
| Mouse | Click keys |

## 🔧 Troubleshooting

**SDL2 not found?**
```bash
# Linux
sudo apt-get install libsdl2-dev

# Mac
brew install sdl2

# Windows
# Download from libsdl.org
```

**Build errors?**
```bash
make clean
make
```

**Still stuck?**
- Read [INSTALL.md](INSTALL.md) for detailed instructions
- Check [README.md](README.md) for full documentation

## 📁 Project Structure

```
waterfall-piano/
├── src/           # Source code
├── include/       # Header files
├── Makefile       # Build file
├── README.md      # Full docs
└── INSTALL.md     # Installation guide
```

## 🎵 Where to Get MIDI Files?

- **Free MIDI files**: 
  - https://freemidi.org/
  - https://www.mutopiaproject.org/
  - https://musescore.com/

- **Best for testing**:
  - Piano compositions
  - Classical music
  - 88-key range pieces

## 💡 Pro Tips

1. Start with simple MIDI files
2. Use +/- keys to adjust playback speed
3. Works best with piano MIDI files
4. Click keys while MIDI plays for overlay effect

## 🐛 Common Issues

**Black screen**: Update graphics drivers
**No sound**: This version is visual-only (no audio output yet)
**Slow performance**: Reduce window size in source code

## 📖 Next Steps

1. ✅ Build and run
2. Load a MIDI file
3. Experiment with controls
4. Read full [README.md](README.md)
5. Customize in `include/WaterfallPiano.h`

## 🤝 Need Help?

- Full docs: [README.md](README.md)
- Installation: [INSTALL.md](INSTALL.md)
- Contributing: [CONTRIBUTING.md](CONTRIBUTING.md)

---

**Enjoy your Waterfall Piano!** 🎹✨
