#ifndef _GRID_H
#define _GRID_H

#include <Adafruit_NeoTrellis.h>

#include "midi.h"

enum class ScaleMode { CHROMATIC, SCALE_ONLY };

#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys


uint32_t Wheel(byte WheelPos);
bool isActiveKey(byte index);
void trellisSetup(Adafruit_MultiTrellis& trellis, TrellisCallback callBack(keyEvent));


#endif