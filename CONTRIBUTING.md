# Contributing to Waterfall Piano

Thank you for your interest in contributing to Waterfall Piano! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Submitting Changes](#submitting-changes)

## Code of Conduct

Be respectful, inclusive, and constructive. We're all here to make great software!

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/waterfall-piano.git`
3. Create a branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test thoroughly
6. Submit a pull request

## Development Setup

### Prerequisites

- C++17 compatible compiler
- SDL2 development libraries
- Git
- CMake (optional)

### Building for Development

```bash
# Clone the repository
git clone https://github.com/yourusername/waterfall-piano.git
cd waterfall-piano

# Build
make clean && make

# Or with CMake
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## How to Contribute

### Reporting Bugs

When reporting bugs, please include:

- Operating system and version
- SDL2 version (`sdl2-config --version`)
- Compiler version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Error messages or screenshots

### Suggesting Features

Feature suggestions are welcome! Please:

- Check if the feature already exists
- Describe the use case
- Explain why it would be valuable
- Consider implementation complexity

### Good First Issues

Look for issues tagged with `good first issue` or `help wanted`. These are great starting points for new contributors.

## Coding Standards

### C++ Style Guide

- **Indentation**: 4 spaces (no tabs)
- **Naming**:
  - Classes: `PascalCase` (e.g., `WaterfallPiano`)
  - Functions: `camelCase` (e.g., `loadMidiFile`)
  - Variables: `camelCase` (e.g., `midiNote`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `TOTAL_KEYS`)
  - Private members: prefix with underscore (optional)

- **Braces**: Opening brace on same line
  ```cpp
  if (condition) {
      // code
  }
  ```

- **Comments**:
  ```cpp
  // Single line comments for brief explanations
  
  /**
   * Multi-line comments for functions and classes
   * Describe parameters, return values, and behavior
   */
  ```

### Example

```cpp
/**
 * Loads a MIDI file and prepares it for playback
 * @param filename Path to the MIDI file
 * @return true if successful, false otherwise
 */
bool WaterfallPiano::loadMidiFile(const std::string& filename) {
    MidiParser parser;
    
    if (!parser.loadFile(filename)) {
        std::cerr << "Failed to load: " << filename << std::endl;
        return false;
    }
    
    // Process MIDI events
    currentMidiFile = filename;
    return true;
}
```

### File Organization

- Header files: `include/`
- Implementation: `src/`
- Each class in its own file
- Use include guards: `#ifndef FILENAME_H`

### Error Handling

```cpp
// Check return values
if (!initialize()) {
    std::cerr << "Initialization failed" << std::endl;
    return false;
}

// Use exceptions for critical errors (sparingly)
// Prefer return codes for expected failures
```

## Testing

### Manual Testing

Before submitting:

1. Build without warnings
2. Test on your target platform
3. Try loading various MIDI files
4. Test keyboard controls
5. Check for memory leaks (valgrind on Linux)
6. Verify performance (60 FPS)

### Testing Checklist

- [ ] Compiles without warnings
- [ ] Runs without crashes
- [ ] MIDI files load correctly
- [ ] Keyboard controls work
- [ ] Mouse interaction works
- [ ] Window resizing works (if applicable)
- [ ] No memory leaks
- [ ] Smooth 60 FPS rendering

## Submitting Changes

### Pull Request Process

1. **Update documentation** if adding features
2. **Add comments** to complex code
3. **Test thoroughly** on your platform
4. **Write clear commit messages**:
   ```
   Add MIDI tempo change support
   
   - Implement tempo meta event parsing
   - Update tick-to-millisecond conversion
   - Add tempo display in UI
   ```

5. **Create pull request** with description:
   - What changed
   - Why it changed
   - How to test it
   - Any breaking changes

### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `style`: Formatting, no code change
- `refactor`: Code restructuring
- `test`: Adding tests
- `chore`: Maintenance

Example:
```
feat: Add sustain pedal visualization

- Parse sustain pedal MIDI events
- Display pedal state in UI
- Add color overlay when pedal active

Closes #123
```

## Areas for Contribution

### High Priority

- Audio output (synthesizer)
- MIDI input from external devices
- Recording feature
- Better error messages

### Medium Priority

- Multiple color schemes
- Touch screen support
- Performance optimizations
- More MIDI event support

### Documentation

- Video tutorials
- More code comments
- Usage examples
- Translation to other languages

### Testing

- Create test suite
- Add example MIDI files
- Automated testing
- Performance benchmarks

## Development Tips

### Debugging

```bash
# Build with debug symbols
make clean
CXXFLAGS="-g -O0" make

# Run with debugger
gdb ./bin/waterfall-piano

# Memory leak detection (Linux)
valgrind --leak-check=full ./bin/waterfall-piano
```

### Profiling

```bash
# Build with profiling
CXXFLAGS="-pg" make

# Run and generate profile
./bin/waterfall-piano
gprof ./bin/waterfall-piano gmon.out > analysis.txt
```

### SDL2 Resources

- Documentation: https://wiki.libsdl.org/
- Tutorial: http://lazyfoo.net/tutorials/SDL/
- Examples: https://github.com/libsdl-org/SDL/tree/main/test

## Questions?

- Open an issue for questions
- Check existing documentation
- Look at similar projects for inspiration

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to Waterfall Piano! 🎹
