#include <Adafruit_NeoTrellis.h>

#include "grid.h"
#include "scale.h"
#include "midi.h"


enum class MODE { EDIT_MODE, SCALE_MODE };

// create a matrix of trellis panels
// WARNING ADDRESS OF TRELLIS GOT MESSED UP DURRING ASSEMBLY
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
	{ Adafruit_NeoTrellis(0x2f), Adafruit_NeoTrellis(0x2e) },
	{ Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x30) }
};

// -GLOBALS ////////////////////////////////////////

// pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

MidiMess MIDI{};
Scale CURRENT_SCALE(C0, MINOR_SCALE);
MODE  CURRENT_MODE{ MODE::SCALE_MODE };

///////////////////////////////////////////////////

//define a callback for key presses
TrellisCallback keyPress(keyEvent evt){

	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		trellis.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM*Y_DIM, 0, 255))); //on rising
		MIDI.sendMIDIMessage(NOTE_ON, CURRENT_SCALE.XToNote( evt.bit.NUM, X_DIM ), 0x45);
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		if (not isActiveKey(evt.bit.NUM % X_DIM))
			trellis.setPixelColor(evt.bit.NUM, 0); //off falling
		MIDI.sendMIDIMessage(NOTE_OFF, CURRENT_SCALE.XToNote( evt.bit.NUM, X_DIM ), 0x00);
	}

	trellis.show();
	return 0;
}

void setup() {
	trellisSetup(trellis, keyPress);
}

void loop() {
	// put your main code here, to run repeatedly:
	trellis.read();
	delay(2);
}
