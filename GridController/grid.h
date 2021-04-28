#ifndef _GRID_H
#define _GRID_H

#include <Adafruit_NeoTrellis.h>

#include "midi.h"
#include "scale.h"

#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys

uint32_t RootLEDColor();
uint32_t ThirdLEDColor();
uint32_t FifthLEDColor();
uint32_t OtherIntervalLEDColors();

uint32_t TurnOffLED();
uint32_t SelectedLEDColor();
uint32_t GreenLED();
uint32_t RedLED();
uint32_t BlueLED();
uint32_t YellowLED();
uint32_t PurpleLED();

uint32_t Wheel(byte WheelPos);

namespace Config {
	bool IndexIsWhiteKeys(byte index);
	bool IndexIsBlackKeys(byte index);
	bool IndexIsKeyboardKey(byte index);

	bool IdexIsOctiveButton(byte index);

	Scale ChangeKey(byte index, int currentOctave);
	byte indexToNote(byte index, int currentOctave);
	byte NoteToIndex(byte note, int currentOctive);
	int indexToOctive(byte index);
	byte OctiveToIndex(int octive);
};

enum class GRID_MODE{ NOTE_MODE, CHORD_MODE, EDIT_MODE };


class Grid
{
public:
	Grid(Adafruit_NeoTrellis *trellis, uint8_t rows, uint8_t cols);

  Grid(const Grid&) = default;
  Grid(Grid&&) = default;

	~Grid() = default;

	void begin();
	void read() { fTrellis.read(); }
	void show() { fTrellis.show(); }

	GRID_MODE GetCurrentPlayMode() const { return fPlayMode; }
	SCALE_MODE GetCurrentDisplayMode() const { return fDisplayMode; }

	void Setup(const Scale& scale, TrellisCallback callBack(keyEvent));
	uint32_t isActiveKey(byte index, const Scale& scale);
	void UpdateMode(const Scale& scale);
	bool SetPadColor(byte index, uint32_t color);
	void ChangePlayBackMode();
	void ChangeDisplayMode(const Scale& scale);

private:
	Adafruit_MultiTrellis fTrellis;
	GRID_MODE fPlayMode{ GRID_MODE::NOTE_MODE };
	SCALE_MODE fDisplayMode{ SCALE_MODE::CHROMATIC };
};


#endif

