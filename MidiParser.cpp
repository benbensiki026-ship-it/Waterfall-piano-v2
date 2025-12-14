#include "MidiParser.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

MidiParser::MidiParser()
    : ticksPerQuarterNote(480)
    , totalDuration(0)
    , tempo(500000) // Default: 120 BPM
{
}

MidiParser::~MidiParser() {
}

uint32_t MidiParser::readVariableLength(const uint8_t* data, size_t& offset) {
    uint32_t value = 0;
    uint8_t byte;
    
    do {
        byte = data[offset++];
        value = (value << 7) | (byte & 0x7F);
    } while (byte & 0x80);
    
    return value;
}

uint32_t MidiParser::read32Bit(const uint8_t* data, size_t offset) {
    return (data[offset] << 24) | (data[offset+1] << 16) | 
           (data[offset+2] << 8) | data[offset+3];
}

uint16_t MidiParser::read16Bit(const uint8_t* data, size_t offset) {
    return (data[offset] << 8) | data[offset+1];
}

uint32_t MidiParser::ticksToMilliseconds(uint32_t ticks) {
    // Convert MIDI ticks to milliseconds
    // tempo is in microseconds per quarter note
    // ticksPerQuarterNote is ticks per quarter note
    return (ticks * tempo) / (ticksPerQuarterNote * 1000);
}

bool MidiParser::loadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(fileSize);
    if (!file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return false;
    }
    
    file.close();
    
    // Parse MIDI file
    size_t offset = 0;
    
    // Parse header
    if (!parseHeader(data.data(), fileSize)) {
        return false;
    }
    offset = 14; // Header is always 14 bytes
    
    // Parse tracks
    tracks.clear();
    while (offset < fileSize) {
        if (!parseTrack(data.data(), fileSize, offset)) {
            break;
        }
    }
    
    return !tracks.empty();
}

bool MidiParser::parseHeader(const uint8_t* data, size_t size) {
    if (size < 14) {
        std::cerr << "File too small to be a MIDI file" << std::endl;
        return false;
    }
    
    // Check for "MThd" signature
    if (data[0] != 'M' || data[1] != 'T' || data[2] != 'h' || data[3] != 'd') {
        std::cerr << "Not a valid MIDI file (missing MThd header)" << std::endl;
        return false;
    }
    
    uint32_t headerLength = read32Bit(data, 4);
    uint16_t format = read16Bit(data, 8);
    uint16_t numTracks = read16Bit(data, 10);
    ticksPerQuarterNote = read16Bit(data, 12);
    
    std::cout << "MIDI Format: " << format << std::endl;
    std::cout << "Number of tracks: " << numTracks << std::endl;
    std::cout << "Ticks per quarter note: " << ticksPerQuarterNote << std::endl;
    
    return true;
}

bool MidiParser::parseTrack(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + 8 > size) {
        return false;
    }
    
    // Check for "MTrk" signature
    if (data[offset] != 'M' || data[offset+1] != 'T' || 
        data[offset+2] != 'r' || data[offset+3] != 'k') {
        std::cerr << "Invalid track header" << std::endl;
        return false;
    }
    
    offset += 4;
    uint32_t trackLength = read32Bit(data, offset);
    offset += 4;
    
    size_t trackEnd = offset + trackLength;
    
    MidiTrack track;
    uint32_t absoluteTime = 0;
    uint8_t runningStatus = 0;
    
    // Track active notes for duration calculation
    struct ActiveNote {
        uint8_t note;
        uint32_t startTime;
        size_t noteIndex;
    };
    std::vector<ActiveNote> activeNotes;
    
    while (offset < trackEnd && offset < size) {
        // Read delta time
        uint32_t deltaTime = readVariableLength(data, offset);
        absoluteTime += deltaTime;
        
        if (offset >= size) break;
        
        uint8_t statusByte = data[offset];
        
        // Handle running status
        if (statusByte < 0x80) {
            statusByte = runningStatus;
        } else {
            offset++;
            runningStatus = statusByte;
        }
        
        uint8_t eventType = statusByte & 0xF0;
        uint8_t channel = statusByte & 0x0F;
        
        if (eventType == 0x90) { // Note On
            if (offset + 1 >= size) break;
            uint8_t note = data[offset++];
            uint8_t velocity = data[offset++];
            
            MidiNote midiNote;
            midiNote.time = ticksToMilliseconds(absoluteTime);
            midiNote.note = note;
            midiNote.velocity = velocity;
            midiNote.isNoteOn = (velocity > 0);
            midiNote.duration = 0;
            
            if (velocity > 0) {
                // Store for duration calculation
                ActiveNote active;
                active.note = note;
                active.startTime = absoluteTime;
                active.noteIndex = track.notes.size();
                activeNotes.push_back(active);
            } else {
                // Velocity 0 is note off
                // Find matching note on and calculate duration
                for (auto it = activeNotes.begin(); it != activeNotes.end(); ++it) {
                    if (it->note == note) {
                        uint32_t duration = absoluteTime - it->startTime;
                        track.notes[it->noteIndex].duration = ticksToMilliseconds(duration);
                        activeNotes.erase(it);
                        break;
                    }
                }
            }
            
            track.notes.push_back(midiNote);
            
        } else if (eventType == 0x80) { // Note Off
            if (offset + 1 >= size) break;
            uint8_t note = data[offset++];
            uint8_t velocity = data[offset++];
            
            // Find matching note on and calculate duration
            for (auto it = activeNotes.begin(); it != activeNotes.end(); ++it) {
                if (it->note == note) {
                    uint32_t duration = absoluteTime - it->startTime;
                    track.notes[it->noteIndex].duration = ticksToMilliseconds(duration);
                    activeNotes.erase(it);
                    break;
                }
            }
            
            MidiNote midiNote;
            midiNote.time = ticksToMilliseconds(absoluteTime);
            midiNote.note = note;
            midiNote.velocity = 0;
            midiNote.isNoteOn = false;
            midiNote.duration = 0;
            track.notes.push_back(midiNote);
            
        } else if (eventType == 0xA0) { // Polyphonic aftertouch
            offset += 2;
        } else if (eventType == 0xB0) { // Control change
            offset += 2;
        } else if (eventType == 0xC0) { // Program change
            offset += 1;
        } else if (eventType == 0xD0) { // Channel aftertouch
            offset += 1;
        } else if (eventType == 0xE0) { // Pitch bend
            offset += 2;
        } else if (statusByte == 0xFF) { // Meta event
            if (offset >= size) break;
            uint8_t metaType = data[offset++];
            uint32_t length = readVariableLength(data, offset);
            
            if (metaType == 0x51 && length == 3) { // Set tempo
                tempo = (data[offset] << 16) | (data[offset+1] << 8) | data[offset+2];
                std::cout << "Tempo change: " << tempo << " microseconds per quarter note" << std::endl;
            } else if (metaType == 0x03 && length > 0) { // Track name
                track.name = std::string(reinterpret_cast<const char*>(&data[offset]), length);
            }
            
            offset += length;
        } else if (statusByte == 0xF0 || statusByte == 0xF7) { // SysEx
            uint32_t length = readVariableLength(data, offset);
            offset += length;
        }
    }
    
    if (!track.notes.empty()) {
        tracks.push_back(track);
        
        // Update total duration
        uint32_t trackDuration = track.notes.back().time + track.notes.back().duration;
        totalDuration = std::max(totalDuration, trackDuration);
    }
    
    offset = trackEnd;
    return true;
}

std::vector<MidiNote> MidiParser::getAllNotes() const {
    std::vector<MidiNote> allNotes;
    
    for (const auto& track : tracks) {
        allNotes.insert(allNotes.end(), track.notes.begin(), track.notes.end());
    }
    
    // Sort by time
    std::sort(allNotes.begin(), allNotes.end(),
              [](const MidiNote& a, const MidiNote& b) { return a.time < b.time; });
    
    return allNotes;
}
