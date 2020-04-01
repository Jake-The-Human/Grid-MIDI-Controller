#ifndef _MIDI_H
#define _MIDI_H

#include <SoftwareSerial.h>

#define C_MINUS_1 0x00
#define C0 0x0C
#define B8 0x77

// const byte CHANNEL  = 0x00;
#define NOTE_ON 0x90
#define NOTE_OFF 0x80

class MidiMess{
public:
	MidiMess();
	~MidiMess() = default;

	void sendMIDIMessage(int cmd, int pitch, int velocity);

private:
	SoftwareSerial mSerial;
};

#endif