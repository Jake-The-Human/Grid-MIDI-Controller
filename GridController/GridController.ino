#include <Adafruit_NeoTrellis.h>

#include "chord.h"
#include "encoder.h"
#include "grid.h"
#include "scale.h"
// #include "screen.h"
#include "midi.h"

#include "test.h"

#define DEBUG 0

#define INT_PIN 10
#define MODE_PIN 7

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// create a matrix of trellis panels
// WARNING ADDRESS OF TRELLIS GOT MESSED UP DURRING ASSEMBLY
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
	{ Adafruit_NeoTrellis(0x2f), Adafruit_NeoTrellis(0x2e) },
	{ Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x30) }
};

// -GLOBALS VAR ////////////////////////////////////////

// pass this matrix to the multitrellis object
Grid GRID((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);
MidiMess MIDI{};
Scale CURRENT_SCALE(C2, MAJOR_SCALE);
Encoder knob{};

///////////////////////////////////////////////////

//define a callback for key presses
TrellisCallback noteModeCallback(keyEvent evt) {
	const SCALE_MODE scaleMode{ GRID.GetCurrentDisplayMode() };
	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		MIDI.sendMIDIMessage(NOTE_ON, CURRENT_SCALE.GridIndexToNote( evt.bit.NUM, X_DIM, scaleMode ), 0x45);
		GRID.SetPadColor(evt.bit.NUM, SelectedLEDColor());
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		MIDI.sendMIDIMessage(NOTE_OFF, CURRENT_SCALE.GridIndexToNote( evt.bit.NUM, X_DIM, scaleMode ), 0x00);

		const uint32_t ledColor{ GRID.isActiveKey(evt.bit.NUM, CURRENT_SCALE) };
		GRID.SetPadColor(evt.bit.NUM, ledColor);
	}

	GRID.show();
	return 0;
}

//define a callback for key presses
TrellisCallback chordModeCallback(keyEvent evt) {
	const byte rootNote{ CURRENT_SCALE.GridIndexToNote( evt.bit.NUM, X_DIM, GRID.GetCurrentDisplayMode() ) };
	MajorChord chord(rootNote, CURRENT_SCALE.GetKey());

	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		MIDI.sendChordMessage(NOTE_ON, chord.GetChordNotes(), 0x45);
		chord.TrellisUpdate(GRID, true, evt.bit.NUM);
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		MIDI.sendChordMessage(NOTE_OFF, chord.GetChordNotes(), 0x00);
		chord.TrellisUpdate(GRID, false, evt.bit.NUM);
	}

	GRID.show();
	return 0;
}

//define a callback for key presses
TrellisCallback editModeCallback(keyEvent evt) {
	const SCALE_MODE scaleMode{ GRID.GetCurrentDisplayMode() };
	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		if (Config::IndexIsKeyboardKey(evt.bit.NUM))
		{
			// reset last scale key to right color
			const byte prevKeyIndex{ Config::NoteToIndex(CURRENT_SCALE.GetKey(), CURRENT_SCALE.GetKeyOctive()) };
			GRID.SetPadColor(prevKeyIndex, Config::IndexIsWhiteKeys(prevKeyIndex) ? RedLED() : BlueLED());

			// highlight new scale key
			CURRENT_SCALE.ChangeKey( Config::indexToNote(evt.bit.NUM, CURRENT_SCALE.GetKeyOctive()) );
			MIDI.sendMIDIMessage(NOTE_ON, Config::indexToNote(evt.bit.NUM, CURRENT_SCALE.GetKeyOctive()), 0x45);
			GRID.SetPadColor(evt.bit.NUM, SelectedLEDColor());
		}
		else if (Config::IdexIsOctiveButton(evt.bit.NUM))
		{
			GRID.SetPadColor(Config::OctiveToIndex(CURRENT_SCALE.GetKeyOctive()), YellowLED());
			Serial.println(CURRENT_SCALE.GetKeyOctive());
			Serial.println(Config::indexToOctive(evt.bit.NUM));
			CURRENT_SCALE.ChangeOctive(Config::indexToOctive(evt.bit.NUM));
			Serial.println(CURRENT_SCALE.GetKeyOctive());
			Serial.println("");
			GRID.SetPadColor(Config::OctiveToIndex(CURRENT_SCALE.GetKeyOctive()), SelectedLEDColor());
		}

	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		if (Config::IndexIsKeyboardKey(evt.bit.NUM))
		{
			MIDI.sendMIDIMessage(NOTE_OFF, Config::indexToNote(evt.bit.NUM, CURRENT_SCALE.GetKeyOctive()), 0x00);
		}
	}

	GRID.show();
	return 0;
}

TrellisCallback keyPressCallback(keyEvent evt) {
	const GRID_MODE currentPlayMode{ GRID.GetCurrentPlayMode() };
	if (currentPlayMode == GRID_MODE::NOTE_MODE)
		return noteModeCallback(evt);
	else if (currentPlayMode == GRID_MODE::CHORD_MODE)
		return chordModeCallback(evt);
	else if (currentPlayMode == GRID_MODE::EDIT_MODE)
		return editModeCallback(evt);

	return 0;
}

void setup() {
	// open the serial port at 9600 bps:
  	Serial.begin(9600);

  	knob.begin();
  	GRID.begin();

	// Test(GRID, true).Run();

	GRID.Setup(CURRENT_SCALE, keyPressCallback);

	pinMode(INT_PIN, INPUT);
	pinMode(MODE_PIN, INPUT);
}

void loop() {
	if(digitalRead(MODE_PIN)){
		GRID.ChangeDisplayMode(CURRENT_SCALE);
		delay(50);
	}
	else if (knob.ButtonPressed()){
		GRID.ChangePlayBackMode();
		delay(50);
	}
	else if(not digitalRead(INT_PIN))
		GRID.read();

	delay(2);
}

