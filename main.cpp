#include "WaterfallPiano.h"
#include <iostream>
#include <string>

void printUsage(const char* programName) {
    std::cout << "\n=== Waterfall Piano - 88 Keys ===" << std::endl;
    std::cout << "Usage: " << programName << " [midi_file.mid]" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "  SPACE     - Play/Pause MIDI" << std::endl;
    std::cout << "  S         - Stop playback" << std::endl;
    std::cout << "  +/-       - Increase/Decrease playback speed" << std::endl;
    std::cout << "  H         - Toggle help" << std::endl;
    std::cout << "  ESC       - Quit" << std::endl;
    std::cout << "  Mouse     - Click keys to play" << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << "  - Full 88-key piano (A0 to C8)" << std::endl;
    std::cout << "  - Waterfall visualization" << std::endl;
    std::cout << "  - MIDI file import and playback" << std::endl;
    std::cout << "  - Real-time note visualization" << std::endl;
    std::cout << "  - Adjustable playback speed" << std::endl;
    std::cout << "  - Color-coded velocity" << std::endl;
    std::cout << "\nExample:" << std::endl;
    std::cout << "  " << programName << " example.mid" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Waterfall Piano - Starting..." << std::endl;
    
    WaterfallPiano piano;
    
    if (!piano.initialize()) {
        std::cerr << "Failed to initialize Waterfall Piano!" << std::endl;
        return 1;
    }
    
    std::cout << "Waterfall Piano initialized successfully!" << std::endl;
    
    // Load MIDI file if provided
    if (argc > 1) {
        std::string midiFile = argv[1];
        std::cout << "Loading MIDI file: " << midiFile << std::endl;
        
        if (piano.loadMidiFile(midiFile)) {
            std::cout << "MIDI file loaded successfully!" << std::endl;
            std::cout << "Press SPACE to start playback" << std::endl;
        } else {
            std::cerr << "Warning: Could not load MIDI file. You can still use the piano interactively." << std::endl;
        }
    } else {
        printUsage(argv[0]);
        std::cout << "No MIDI file specified. Starting in interactive mode..." << std::endl;
        std::cout << "You can click on keys to play them!" << std::endl;
    }
    
    std::cout << "\nStarting main loop..." << std::endl;
    piano.run();
    
    std::cout << "Cleaning up..." << std::endl;
    piano.cleanup();
    
    std::cout << "Waterfall Piano closed. Goodbye!" << std::endl;
    return 0;
}
