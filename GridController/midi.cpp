#include "midi.h"

#define RX_PIN 2
#define TX_PIN 3

MidiMess::MidiMess() : mSerial(SoftwareSerial(RX_PIN, TX_PIN)) {
	mSerial.begin(31250);
}

void MidiMess::sendMIDIMessage(int cmd, int pitch, int velocity) {
	if (pitch >= C_MINUS_1 and pitch <= B8) {
		mSerial.write(cmd);
		mSerial.write(pitch);
		mSerial.write(velocity);
	}
}

void MidiMess::sendChordMessage(int cmd, char* chord, int velocity) {
	for(size_t i = 0; chord[i] != 0; ++i)
	{
		sendMIDIMessage(cmd, chord[i], velocity);
	}
}