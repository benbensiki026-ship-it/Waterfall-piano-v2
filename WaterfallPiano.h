#ifndef WATERFALL_PIANO_H
#define WATERFALL_PIANO_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <memory>
#include <map>

// Piano constants
const int TOTAL_KEYS = 88;
const int FIRST_MIDI_NOTE = 21;  // A0
const int LAST_MIDI_NOTE = 108;   // C8
const int WHITE_KEYS = 52;
const int BLACK_KEYS = 36;

// Display constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int KEYBOARD_HEIGHT = 150;
const int WATERFALL_HEIGHT = SCREEN_HEIGHT - KEYBOARD_HEIGHT;

// Key dimensions
const int WHITE_KEY_WIDTH = SCREEN_WIDTH / WHITE_KEYS;
const int WHITE_KEY_HEIGHT = KEYBOARD_HEIGHT;
const int BLACK_KEY_WIDTH = WHITE_KEY_WIDTH * 0.6;
const int BLACK_KEY_HEIGHT = KEYBOARD_HEIGHT * 0.6;

// Colors
const SDL_Color COLOR_WHITE_KEY = {255, 255, 255, 255};
const SDL_Color COLOR_BLACK_KEY = {0, 0, 0, 255};
const SDL_Color COLOR_WHITE_PRESSED = {200, 230, 255, 255};
const SDL_Color COLOR_BLACK_PRESSED = {100, 100, 150, 255};
const SDL_Color COLOR_BACKGROUND = {20, 20, 30, 255};

// Note structure for waterfall visualization
struct Note {
    int midiNote;
    Uint32 startTime;
    Uint32 endTime;
    int velocity;
    bool active;
    SDL_Color color;
};

// Piano key structure
struct PianoKey {
    int midiNote;
    bool isBlack;
    SDL_Rect rect;
    bool pressed;
    int whiteKeyIndex;
};

class WaterfallPiano {
public:
    WaterfallPiano();
    ~WaterfallPiano();
    
    bool initialize();
    void run();
    void cleanup();
    
    // MIDI functions
    bool loadMidiFile(const std::string& filename);
    void playMidi();
    void pauseMidi();
    void stopMidi();
    void setMidiPosition(float position);
    
    // Rendering functions
    void render();
    void renderKeyboard();
    void renderWaterfall();
    void renderUI();
    
    // Input handling
    void handleInput();
    void handleKeyPress(int midiNote);
    void handleKeyRelease(int midiNote);
    
    // Utility functions
    int getMidiNoteFromScreenX(int x, int y);
    SDL_Color getNoteColor(int velocity);
    void updateWaterfall(float deltaTime);
    
private:
    // SDL components
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* waterfallTexture;
    
    // Piano keys
    std::vector<PianoKey> keys;
    std::map<int, int> keyMap; // MIDI note -> key index
    
    // Waterfall notes
    std::vector<Note> activeNotes;
    std::vector<Note> upcomingNotes;
    
    // MIDI data
    struct MidiEvent {
        Uint32 time;
        int note;
        int velocity;
        bool isNoteOn;
    };
    std::vector<MidiEvent> midiEvents;
    
    // Playback state
    bool running;
    bool playing;
    bool paused;
    Uint32 startTime;
    Uint32 currentTime;
    float playbackSpeed;
    float scrollSpeed;
    
    // UI state
    bool showHelp;
    std::string currentMidiFile;
    
    // Helper functions
    void initializeKeys();
    void initializeWaterfallTexture();
    bool isBlackKey(int midiNote);
    int getWhiteKeyIndex(int midiNote);
    void drawFilledRect(SDL_Rect rect, SDL_Color color);
    void drawRect(SDL_Rect rect, SDL_Color color);
};

#endif // WATERFALL_PIANO_H
