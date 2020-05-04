#include "grid.h"

#include "leds.h"


uint32_t RootLEDColor() { return LED_COLORS::BLUE; }
uint32_t ThirdLEDColor() { return LED_COLORS::GREEN; }
uint32_t FifthLEDColor() { return LED_COLORS::RED; }
uint32_t OctiveLEDColors() { return LED_COLORS::YELLOW; }
uint32_t OtherIntervalLEDColors() { return LED_COLORS::PURPLE; }

uint32_t TurnOffLED() { return LED_COLORS::OFF; }
uint32_t SelectedLEDColor() { return LED_COLORS::WHITE; }

uint32_t GreenLED() { return LED_COLORS::GREEN; }
uint32_t RedLED() { return LED_COLORS::RED; }
uint32_t BlueLED() { return LED_COLORS::BLUE; }
uint32_t YellowLED() { return LED_COLORS::YELLOW; }
uint32_t PurpleLED() { return LED_COLORS::PURPLE; }

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

uint32_t isActiveKey(byte index, const Scale& scale, const SCALE_MODE& mode)
{
	const byte currentNote{ scale.GridIndexToNote(index, X_DIM, mode) };
	const short currentOctive{ scale.GetYIndexOfNote(currentNote) };
	
	if(scale.GetOct(currentOctive) == currentNote and mode == SCALE_MODE::SCALE_ONLY)
		return OctiveLEDColors();
	if(scale.GetRoot(currentOctive) == currentNote)
		return RootLEDColor();
	else if ( scale.Get3rd(currentOctive) == currentNote )
		return ThirdLEDColor();
	else if ( scale.Get5th(currentOctive) == currentNote )
		return FifthLEDColor();
	else if (
		scale.Get2nd(currentOctive) == currentNote or
		scale.Get4th(currentOctive) == currentNote or
		scale.Get6th(currentOctive) == currentNote or
		scale.Get7th(currentOctive) == currentNote)
		return OtherIntervalLEDColors();
	else
		return TurnOffLED();
}

void trellisSetup(Adafruit_MultiTrellis& trellis, const Scale& scale, const SCALE_MODE& mode, TrellisCallback callBack(keyEvent)) {
	/* the array can be addressed as x,y or with the key number */
	for(int i=0; i < Y_DIM*X_DIM; i++){
	  trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
	  trellis.show();
	  delay(15);
	}

	for(int y=0; y < Y_DIM; y++){
		for(int x=0; x < X_DIM; x++){
		  //activate rising and falling edges on all keys
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
		  trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
		  trellis.registerCallback(x, y, callBack);

		  trellis.setPixelColor(x, y, isActiveKey((y * Y_DIM) + x, scale, mode)); //addressed with x,y
		  
		  trellis.show(); //show all LEDs
		  delay(15);
		}
	}
}

void trellisUpdateMode(Adafruit_MultiTrellis& trellis, const Scale& scale, const SCALE_MODE& mode)
{
	/* the array can be addressed as x,y or with the key number */
	for(int i=0; i < Y_DIM*X_DIM; i++){
	 	trellis.setPixelColor(i, isActiveKey(i, scale, mode)); //addressed with x,y
		trellis.show();
	  	delay(15);
	}
}


bool setTrellisPadColor(Adafruit_MultiTrellis& trellis, byte index, uint32_t color)
{
	bool result{ false };
	if(index > -1 and index < 64){
		trellis.setPixelColor(index, color);
		result = true;
	}
	return result;
}


Grid::Grid(Adafruit_NeoTrellis *trellis, uint8_t rows, uint8_t cols) : fTrellis(trellis, rows, cols)
{}


void Grid::begin()
{
	if(not fTrellis.begin()) {
		Serial.println("failed to begin trellis");
		while(1);
	}
}


void Grid::Setup(const Scale& scale, TrellisCallback callBack(keyEvent))
{
	/* the array can be addressed as x,y or with the key number */
	for(int i=0; i < Y_DIM*X_DIM; i++){
	  fTrellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
	  fTrellis.show();
	  delay(15);
	}

	for(int y=0; y < Y_DIM; y++){
		for(int x=0; x < X_DIM; x++){
		  //activate rising and falling edges on all keys
		  fTrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
		  fTrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
		  fTrellis.registerCallback(x, y, callBack);

		  fTrellis.setPixelColor(x, y, isActiveKey((y * Y_DIM) + x, scale)); //addressed with x,y
		  
		  fTrellis.show(); //show all LEDs
		  delay(15);
		}
	}
}


uint32_t Grid::isActiveKey(byte index, const Scale& scale)
{
	const byte currentNote{ scale.GridIndexToNote(index, X_DIM, fDisplayMode) };
	const short currentOctive{ scale.GetYIndexOfNote(currentNote) };
	
	if(scale.GetOct(currentOctive) == currentNote and fDisplayMode == SCALE_MODE::SCALE_ONLY)
		return OctiveLEDColors();
	if(scale.GetRoot(currentOctive) == currentNote)
		return RootLEDColor();
	else if ( scale.Get3rd(currentOctive) == currentNote )
		return ThirdLEDColor();
	else if ( scale.Get5th(currentOctive) == currentNote )
		return FifthLEDColor();
	else if (
		scale.Get2nd(currentOctive) == currentNote or
		scale.Get4th(currentOctive) == currentNote or
		scale.Get6th(currentOctive) == currentNote or
		scale.Get7th(currentOctive) == currentNote)
		return OtherIntervalLEDColors();
	else
		return TurnOffLED();
}


void Grid::UpdateMode(const Scale& scale)
{
	if (fDisplayMode == SCALE_MODE::EDIT_MODE)
	{
		auto isActive = [](byte index, const Scale& scale) -> bool {
			if (Config::IndexIsWhiteKeys(index) or Config::IndexIsBlackKeys(index))
				return Config::indexToNote(index, scale.GetKeyOctive()) == scale.GetKey();
			else if (Config::IdexIsOctiveButton(index))
				return Config::indexToOctive(index) == scale.GetKeyOctive();
		};

		uint32_t color{ TurnOffLED() };
		/* the array can be addressed as x,y or with the key number */
		for(int i=0; i < Y_DIM*X_DIM; i++) {
			if(isActive(i, scale))
				color = SelectedLEDColor();
			else if (Config::IndexIsBlackKeys(i))
				color = BlueLED();
			else if (Config::IndexIsWhiteKeys(i))
				color = RedLED();
			else if (Config::IdexIsOctiveButton(i))
				color = YellowLED();
			else if (i >= 32 and i <= 32 + NUM_OF_SCALES - 1 )
				color = GreenLED();
			else if (i >= 48 and i <= 48 + 2/*num of chords - 1*/)
				color = PurpleLED();
			else
		 		color = TurnOffLED();
			
			fTrellis.setPixelColor(i, color);
			fTrellis.show();
			delay(15);
		}
	}
	else
	{
		/* the array can be addressed as x,y or with the key number */
		for(int i=0; i < Y_DIM*X_DIM; i++){
		 	fTrellis.setPixelColor(i, isActiveKey(i, scale)); //addressed with x,y
			fTrellis.show();
		  	delay(15);
		}
	}
}


bool Grid::SetPadColor(byte index, uint32_t color)
{
	bool result{ false };
	if(index > -1 and index < 64){
		fTrellis.setPixelColor(index, color);
		result = true;
	}
	return result;
}


void Grid::ChangePlayBackMode()
{
	switch(fPlayMode) {
		case GRID_MODE::CHORD_MODE:
			fPlayMode = GRID_MODE::NOTE_MODE;
			break;
		case GRID_MODE::NOTE_MODE:
			fPlayMode = GRID_MODE::CHORD_MODE;
			break;
	}
}


void Grid::ChangeDisplayMode(const Scale& scale)
{
	switch(fDisplayMode) {
		case SCALE_MODE::EDIT_MODE:
			fDisplayMode = SCALE_MODE::CHROMATIC;
			fPlayMode = GRID_MODE::NOTE_MODE;
			break;
		case SCALE_MODE::CHROMATIC:
			fDisplayMode = SCALE_MODE::SCALE_ONLY;
			fPlayMode = GRID_MODE::NOTE_MODE;
			break;
		case SCALE_MODE::SCALE_ONLY:
			fDisplayMode = SCALE_MODE::EDIT_MODE;
			fPlayMode = GRID_MODE::EDIT_MODE;
			break;
	}
		
	UpdateMode(scale);
}

Scale Config::ChangeKey(byte index, int currentOctive)
{
	return { Config::indexToNote(index, currentOctive), MAJOR_SCALE };//{ newRoot + (currentOctive * 12), scaleType };
}

byte Config::indexToNote(byte index, int currentOctave) {
	byte newRoot{ 0x0C};

	if(Config::IndexIsKeyboardKey(index))
	{
		switch (index) {
			case 8:
				break;
			case 1: 
				newRoot += 1;
				break;
			case 9:
				newRoot += 2;
				break;
			case 2:
				newRoot += 3;
				break;
			case 10:
				newRoot += 4;
				break;
			case 11:
				newRoot += 5;
				break;
			case 4:
				newRoot += 6;
				break;
			case 12:
				newRoot += 7;
				break;
			case 5:
				newRoot += 8;
				break;
			case 13:
				newRoot += 9;
				break;
			case 6:
				newRoot += 10;
				break;
			case 14: 
				newRoot += 11;
				break ;
			case 15:
				newRoot += 12;
				break;
		};
	}

	return newRoot + (currentOctave * 12);
}

byte Config::NoteToIndex(byte note, int currentOctive) {
	byte index{ 8 };
	byte tempNote{ note - (currentOctive * 12)};

	switch (tempNote) {
		case 0x0C:
			index = 8;
			break;
		case 0x0C + 1: 
			index = 1;
			break;
		case 0x0C + 2:
			index = 9;
			break;
		case 0x0C + 3:
			index = 2;
			break;
		case 0x0C + 4:
			index = 10;
			break;
		case 0x0C + 5:
			index = 11;
			break;
		case 0x0C + 6:
			index = 4;
			break;
		case 0x0C + 7:
			index = 12;
			break;
		case 0x0C + 8:
			index = 5;
			break;
		case 0x0C + 9:
			index = 13;
			break;
		case 0x0C + 10:
			index = 6;
			break;
		case 0x0C + 11: 
			index = 14;
			break ;
	};

	return index;
}

int Config::indexToOctive(byte index) {
	if(index == 16)
		return -1;
	else if(index == 23)
		return 8;
	else
		return index - 24;
}


byte Config::OctiveToIndex(int octive) {
	if(octive == -1)
		return 16;
	else if(octive == 8)
		return 23;
	else
		return octive + 24;
}


bool Config::IndexIsWhiteKeys(byte index) { 
	return index >= 8 and index < 15; 
}


bool Config::IndexIsBlackKeys(byte index) {
	return index == 1 or index == 2 or index == 4 or 
			index == 5 or index == 6;
}
	
bool Config::IndexIsKeyboardKey(byte index) { 
	return Config::IndexIsWhiteKeys(index) or Config::IndexIsBlackKeys(index);
}


bool Config::IdexIsOctiveButton(byte index)
{
	return (index >= 24 and index <= 31) or index == 16 or index ==23;
}
