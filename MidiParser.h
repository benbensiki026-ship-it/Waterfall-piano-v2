#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

#include <vector>
#include <string>
#include <cstdint>

struct MidiNote {
    uint32_t time;      // Time in milliseconds
    uint8_t note;       // MIDI note number (0-127)
    uint8_t velocity;   // Velocity (0-127)
    bool isNoteOn;      // true for note on, false for note off
    uint32_t duration;  // Duration in milliseconds (calculated)
};

struct MidiTrack {
    std::vector<MidiNote> notes;
    std::string name;
};

class MidiParser {
public:
    MidiParser();
    ~MidiParser();
    
    bool loadFile(const std::string& filename);
    const std::vector<MidiTrack>& getTracks() const { return tracks; }
    std::vector<MidiNote> getAllNotes() const;
    
    uint16_t getTicksPerQuarterNote() const { return ticksPerQuarterNote; }
    uint32_t getTotalDuration() const { return totalDuration; }
    
private:
    std::vector<MidiTrack> tracks;
    uint16_t ticksPerQuarterNote;
    uint32_t totalDuration;
    uint32_t tempo; // microseconds per quarter note
    
    // Parsing helper functions
    uint32_t readVariableLength(const uint8_t* data, size_t& offset);
    uint32_t read32Bit(const uint8_t* data, size_t offset);
    uint16_t read16Bit(const uint8_t* data, size_t offset);
    
    bool parseHeader(const uint8_t* data, size_t size);
    bool parseTrack(const uint8_t* data, size_t size, size_t& offset);
    uint32_t ticksToMilliseconds(uint32_t ticks);
};

#endif // MIDI_PARSER_H
