#include <Adafruit_NeoTrellis.h>


const byte MIDDLE_C = 0x3c;


#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys

//create a matrix of trellis panels
// WARNING ADDRESS OF TRELLIS GOT MESSED UP
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
	{ Adafruit_NeoTrellis(0x2f), Adafruit_NeoTrellis(0x2e) },
	{ Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x30) }
};

//pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

void noteOn(int cmd, int pitch, int velocity) {
	Serial.write(cmd);
	Serial.write(pitch);
	Serial.write(velocity);
}

void playNotes(int status,int note){
	if(status){
	    noteOn(0x90, MIDDLE_C, 0x45);
	}
	else{
	    noteOn(0x90, MIDDLE_C, 0x00);
	}
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} else {
		WheelPos -= 170;
		return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	return 0;
}

//define a callback for key presses
TrellisCallback keyPress(keyEvent evt){

	if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
		trellis.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM*Y_DIM, 0, 255))); //on rising
		playNotes(true, evt.bit.NUM);
	}
	else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
		trellis.setPixelColor(evt.bit.NUM, 0); //off falling
		playNotes(false, evt.bit.NUM);
	}

	trellis.show();

//	Serial.println(evt.bit.NUM);
	return 0;
}

void setup() {
	// put your setup code here, to run once:
//	Serial.begin(9600);
  Serial.begin(31250);
	//while(!Serial);

	if(!trellis.begin()){
		Serial.println("failed to begin trellis");
		while(1);
	}

	/* the array can be addressed as x,y or with the key number */
	for(int i=0; i < Y_DIM*X_DIM; i++){
	  trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
	  trellis.show();
	  delay(50);
	}

	for(int y=0; y < Y_DIM; y++){
		for(int x=0; x < X_DIM; x++){
		  //activate rising and falling edges on all keys
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
		  trellis.registerCallback(x, y, keyPress);
		  trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
		  trellis.show(); //show all LEDs
		  delay(50);
		}
	}
}

void loop() {
	// put your main code here, to run repeatedly:
	trellis.read();
	delay(20);
}
