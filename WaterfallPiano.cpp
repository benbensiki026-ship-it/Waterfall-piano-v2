#include "WaterfallPiano.h"
#include "MidiParser.h"
#include <iostream>
#include <algorithm>
#include <cmath>

WaterfallPiano::WaterfallPiano() 
    : window(nullptr)
    , renderer(nullptr)
    , waterfallTexture(nullptr)
    , running(false)
    , playing(false)
    , paused(false)
    , startTime(0)
    , currentTime(0)
    , playbackSpeed(1.0f)
    , scrollSpeed(200.0f)
    , showHelp(false)
{
}

WaterfallPiano::~WaterfallPiano() {
    cleanup();
}

bool WaterfallPiano::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("Waterfall Piano - 88 Keys",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Initialize piano keys
    initializeKeys();
    
    // Initialize waterfall texture
    initializeWaterfallTexture();
    
    running = true;
    return true;
}

void WaterfallPiano::initializeKeys() {
    keys.clear();
    keyMap.clear();
    
    int whiteKeyCount = 0;
    
    for (int i = 0; i < TOTAL_KEYS; i++) {
        int midiNote = FIRST_MIDI_NOTE + i;
        PianoKey key;
        key.midiNote = midiNote;
        key.isBlack = isBlackKey(midiNote);
        key.pressed = false;
        
        if (!key.isBlack) {
            key.whiteKeyIndex = whiteKeyCount;
            whiteKeyCount++;
        }
        
        keys.push_back(key);
        keyMap[midiNote] = i;
    }
    
    // Position white keys
    for (auto& key : keys) {
        if (!key.isBlack) {
            key.rect.x = key.whiteKeyIndex * WHITE_KEY_WIDTH;
            key.rect.y = WATERFALL_HEIGHT;
            key.rect.w = WHITE_KEY_WIDTH - 2;
            key.rect.h = WHITE_KEY_HEIGHT;
        }
    }
    
    // Position black keys (on top of white keys)
    for (size_t i = 0; i < keys.size(); i++) {
        if (keys[i].isBlack && i > 0) {
            // Find adjacent white key
            int whiteIdx = keys[i-1].whiteKeyIndex;
            keys[i].rect.x = whiteIdx * WHITE_KEY_WIDTH + WHITE_KEY_WIDTH - BLACK_KEY_WIDTH / 2;
            keys[i].rect.y = WATERFALL_HEIGHT;
            keys[i].rect.w = BLACK_KEY_WIDTH;
            keys[i].rect.h = BLACK_KEY_HEIGHT;
        }
    }
}

void WaterfallPiano::initializeWaterfallTexture() {
    waterfallTexture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         SCREEN_WIDTH,
                                         WATERFALL_HEIGHT);
    
    if (waterfallTexture) {
        SDL_SetTextureBlendMode(waterfallTexture, SDL_BLENDMODE_BLEND);
    }
}

bool WaterfallPiano::isBlackKey(int midiNote) {
    int noteInOctave = midiNote % 12;
    return (noteInOctave == 1 || noteInOctave == 3 || noteInOctave == 6 || 
            noteInOctave == 8 || noteInOctave == 10);
}

int WaterfallPiano::getWhiteKeyIndex(int midiNote) {
    int whiteKeyCount = 0;
    for (int i = FIRST_MIDI_NOTE; i <= midiNote; i++) {
        if (!isBlackKey(i)) {
            if (i == midiNote) return whiteKeyCount;
            whiteKeyCount++;
        }
    }
    return whiteKeyCount;
}

bool WaterfallPiano::loadMidiFile(const std::string& filename) {
    MidiParser parser;
    
    if (!parser.loadFile(filename)) {
        std::cerr << "Failed to load MIDI file: " << filename << std::endl;
        return false;
    }
    
    currentMidiFile = filename;
    midiEvents.clear();
    upcomingNotes.clear();
    
    // Convert MIDI notes to events
    std::vector<MidiNote> allNotes = parser.getAllNotes();
    
    for (const auto& note : allNotes) {
        MidiEvent event;
        event.time = note.time;
        event.note = note.note;
        event.velocity = note.velocity;
        event.isNoteOn = note.isNoteOn;
        midiEvents.push_back(event);
    }
    
    // Sort events by time
    std::sort(midiEvents.begin(), midiEvents.end(),
              [](const MidiEvent& a, const MidiEvent& b) { return a.time < b.time; });
    
    std::cout << "Loaded MIDI file: " << filename << std::endl;
    std::cout << "Total events: " << midiEvents.size() << std::endl;
    
    return true;
}

void WaterfallPiano::playMidi() {
    if (midiEvents.empty()) {
        std::cout << "No MIDI file loaded!" << std::endl;
        return;
    }
    
    playing = true;
    paused = false;
    startTime = SDL_GetTicks();
}

void WaterfallPiano::pauseMidi() {
    paused = !paused;
}

void WaterfallPiano::stopMidi() {
    playing = false;
    paused = false;
    currentTime = 0;
    activeNotes.clear();
    
    // Release all keys
    for (auto& key : keys) {
        key.pressed = false;
    }
}

SDL_Color WaterfallPiano::getNoteColor(int velocity) {
    // Color gradient based on velocity
    float normalizedVel = velocity / 127.0f;
    
    SDL_Color color;
    if (normalizedVel < 0.33f) {
        // Blue to cyan
        color = {100, 150, 255, 200};
    } else if (normalizedVel < 0.66f) {
        // Cyan to green
        color = {100, 255, 150, 200};
    } else {
        // Green to yellow
        color = {255, 220, 100, 200};
    }
    
    return color;
}

void WaterfallPiano::updateWaterfall(float deltaTime) {
    if (!playing || paused) return;
    
    currentTime = SDL_GetTicks() - startTime;
    
    // Process MIDI events
    for (auto& event : midiEvents) {
        if (event.time <= currentTime * playbackSpeed) {
            if (event.isNoteOn && event.velocity > 0) {
                // Note on
                handleKeyPress(event.note);
                
                Note note;
                note.midiNote = event.note;
                note.startTime = currentTime;
                note.endTime = 0;
                note.velocity = event.velocity;
                note.active = true;
                note.color = getNoteColor(event.velocity);
                activeNotes.push_back(note);
            } else {
                // Note off
                handleKeyRelease(event.note);
                
                // Find and end the active note
                for (auto& note : activeNotes) {
                    if (note.active && note.midiNote == event.note) {
                        note.endTime = currentTime;
                        note.active = false;
                        break;
                    }
                }
            }
        }
    }
    
    // Remove old notes that have scrolled off screen
    activeNotes.erase(
        std::remove_if(activeNotes.begin(), activeNotes.end(),
                      [this](const Note& note) {
                          return !note.active && 
                                 (currentTime - note.endTime) > (WATERFALL_HEIGHT / scrollSpeed * 1000);
                      }),
        activeNotes.end()
    );
}

void WaterfallPiano::handleKeyPress(int midiNote) {
    if (midiNote < FIRST_MIDI_NOTE || midiNote > LAST_MIDI_NOTE) return;
    
    auto it = keyMap.find(midiNote);
    if (it != keyMap.end()) {
        keys[it->second].pressed = true;
    }
}

void WaterfallPiano::handleKeyRelease(int midiNote) {
    if (midiNote < FIRST_MIDI_NOTE || midiNote > LAST_MIDI_NOTE) return;
    
    auto it = keyMap.find(midiNote);
    if (it != keyMap.end()) {
        keys[it->second].pressed = false;
    }
}

void WaterfallPiano::renderKeyboard() {
    // Draw white keys first
    for (const auto& key : keys) {
        if (!key.isBlack) {
            SDL_Color color = key.pressed ? COLOR_WHITE_PRESSED : COLOR_WHITE_KEY;
            drawFilledRect(key.rect, color);
            drawRect(key.rect, COLOR_BLACK_KEY);
        }
    }
    
    // Draw black keys on top
    for (const auto& key : keys) {
        if (key.isBlack) {
            SDL_Color color = key.pressed ? COLOR_BLACK_PRESSED : COLOR_BLACK_KEY;
            drawFilledRect(key.rect, color);
        }
    }
}

void WaterfallPiano::renderWaterfall() {
    // Clear waterfall area
    SDL_Rect waterfallArea = {0, 0, SCREEN_WIDTH, WATERFALL_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 
                          COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, 
                          COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);
    SDL_RenderFillRect(renderer, &waterfallArea);
    
    // Draw guide lines for keys
    SDL_SetRenderDrawColor(renderer, 40, 40, 50, 50);
    for (const auto& key : keys) {
        if (!key.isBlack) {
            int x = key.rect.x + key.rect.w / 2;
            SDL_RenderDrawLine(renderer, x, 0, x, WATERFALL_HEIGHT);
        }
    }
    
    // Draw falling notes
    for (const auto& note : activeNotes) {
        auto it = keyMap.find(note.midiNote);
        if (it == keyMap.end()) continue;
        
        const PianoKey& key = keys[it->second];
        
        float timeOffset = (currentTime - note.startTime) / 1000.0f * scrollSpeed;
        int yEnd = WATERFALL_HEIGHT - static_cast<int>(timeOffset);
        
        int yStart = yEnd;
        if (!note.active && note.endTime > 0) {
            float duration = (note.endTime - note.startTime) / 1000.0f * scrollSpeed;
            yStart = yEnd - static_cast<int>(duration);
        } else {
            yStart = 0; // Note is still active, extends to top
        }
        
        // Only draw if visible
        if (yEnd > 0 && yStart < WATERFALL_HEIGHT) {
            SDL_Rect noteRect;
            noteRect.x = key.rect.x + 2;
            noteRect.y = std::max(0, yStart);
            noteRect.w = key.rect.w - 4;
            noteRect.h = std::min(yEnd - noteRect.y, WATERFALL_HEIGHT - noteRect.y);
            
            drawFilledRect(noteRect, note.color);
        }
    }
}

void WaterfallPiano::renderUI() {
    // Draw info text (simplified - would use SDL_ttf in full implementation)
    // For now, just draw basic indicators
    
    if (showHelp) {
        // Draw help overlay
        SDL_Rect helpBox = {50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(renderer, &helpBox);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &helpBox);
    }
    
    // Draw playback indicator
    if (playing && !paused) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect playIndicator = {10, 10, 20, 20};
        SDL_RenderFillRect(renderer, &playIndicator);
    } else if (paused) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect pauseBar1 = {10, 10, 7, 20};
        SDL_Rect pauseBar2 = {20, 10, 7, 20};
        SDL_RenderFillRect(renderer, &pauseBar1);
        SDL_RenderFillRect(renderer, &pauseBar2);
    }
}

void WaterfallPiano::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 
                          COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, 
                          COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);
    SDL_RenderClear(renderer);
    
    // Render components
    renderWaterfall();
    renderKeyboard();
    renderUI();
    
    // Present
    SDL_RenderPresent(renderer);
}

void WaterfallPiano::drawFilledRect(SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void WaterfallPiano::drawRect(SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}

void WaterfallPiano::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_SPACE:
                        if (playing) pauseMidi();
                        else playMidi();
                        break;
                    case SDLK_s:
                        stopMidi();
                        break;
                    case SDLK_h:
                        showHelp = !showHelp;
                        break;
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                        playbackSpeed = std::min(playbackSpeed + 0.1f, 3.0f);
                        break;
                    case SDLK_MINUS:
                        playbackSpeed = std::max(playbackSpeed - 0.1f, 0.1f);
                        break;
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.y >= WATERFALL_HEIGHT) {
                    int note = getMidiNoteFromScreenX(event.button.x, event.button.y);
                    if (note >= 0) {
                        handleKeyPress(note);
                    }
                }
                break;
                
            case SDL_MOUSEBUTTONUP:
                if (event.button.y >= WATERFALL_HEIGHT) {
                    int note = getMidiNoteFromScreenX(event.button.x, event.button.y);
                    if (note >= 0) {
                        handleKeyRelease(note);
                    }
                }
                break;
        }
    }
}

int WaterfallPiano::getMidiNoteFromScreenX(int x, int y) {
    // Check black keys first (they're on top)
    if (y < WATERFALL_HEIGHT + BLACK_KEY_HEIGHT) {
        for (const auto& key : keys) {
            if (key.isBlack && x >= key.rect.x && x < key.rect.x + key.rect.w) {
                return key.midiNote;
            }
        }
    }
    
    // Check white keys
    for (const auto& key : keys) {
        if (!key.isBlack && x >= key.rect.x && x < key.rect.x + key.rect.w) {
            return key.midiNote;
        }
    }
    
    return -1;
}

void WaterfallPiano::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTime) / 1000.0f;
        lastTime = currentTicks;
        
        handleInput();
        updateWaterfall(deltaTime);
        render();
        
        SDL_Delay(1); // Small delay to prevent CPU hogging
    }
}

void WaterfallPiano::cleanup() {
    if (waterfallTexture) {
        SDL_DestroyTexture(waterfallTexture);
        waterfallTexture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}
