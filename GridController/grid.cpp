#include "grid.h"


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

bool isActiveKey(byte index)
{
	return 0 == index or 2 == index or 4 == index or 7 == index;
}

void trellisSetup(Adafruit_MultiTrellis& trellis, TrellisCallback callBack(keyEvent)) {
		if(not trellis.begin()){
		Serial.println("failed to begin trellis");
		while(1);
	}

	/* the array can be addressed as x,y or with the key number */
	for(int i=0; i < Y_DIM*X_DIM; i++){
	  trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
	  trellis.show();
	  delay(25);
	}

	for(int y=0; y < Y_DIM; y++){
		for(int x=0; x < X_DIM; x++){
		  //activate rising and falling edges on all keys
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
		  trellis.registerCallback(x, y, callBack);

		  if (not isActiveKey(x)) {
		  	trellis.setPixelColor(x, y, 0x000000); //addressed with x,y
		  }
		  trellis.show(); //show all LEDs
		  delay(25);
		}
	}
}