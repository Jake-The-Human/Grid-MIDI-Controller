#include "midi.h"

#define RX_PIN 2
#define TX_PIN 3

MidiMess::MidiMess() : mSerial(SoftwareSerial(RX_PIN, TX_PIN))
{
	mSerial.begin(31250);
}

void MidiMess::sendMIDIMessage(int cmd, int pitch, int velocity) {
	mSerial.write(cmd);
	mSerial.write(pitch);
	mSerial.write(velocity);
}