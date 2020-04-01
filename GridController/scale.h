#ifndef _SCALE_H
#define _SCALE_H

#include "midi.h"

#define WHOLE_STEP 2
#define HALF_STEP  1
#define NOTES_IN_OCT 12

const byte MAJOR_SCALE[] = { 0, 2, 4, 5, 7, 9, 11, 12 };
const byte MINOR_SCALE[] = { 0, 2, 3, 5, 7, 9, 11, 12 };


class Scale
{
public:
	Scale(byte key, byte* scale) : mKey(key), mScale(scale) {}
	~Scale() = default;

	bool GoUpAnOct()   { 
		bool result{ false };
		if((mKey + NOTES_IN_OCT) <= B8){ 
			mKey += NOTES_IN_OCT;
			result = true;
		}
		return result;
	}

	bool GoDownAnOct() { 
		bool result{ false };
		if((mKey - NOTES_IN_OCT) >= C_MINUS_1 and (mKey - NOTES_IN_OCT) <= B8){
			mKey -= NOTES_IN_OCT;
			result = true;
		}
		return result;
	}

	inline byte XToNote(byte index, int x_dim) const { 
		return (mKey + mScale[index % x_dim]) + ((index / x_dim ) * NOTES_IN_OCT); 
	}

	inline byte GetRoot() const { return mKey; }
	inline byte Get2nd() const  { return mKey + mScale[1]; }
	inline byte Get3rd() const  { return mKey + mScale[2]; }
	inline byte Get4th() const  { return mKey + mScale[3]; }
	inline byte Get5th() const  { return mKey + mScale[4]; }
	inline byte Get6th() const  { return mKey + mScale[5]; }
	inline byte Get7th() const  { return mKey + mScale[6]; }
	inline byte GetOct() const  { return mKey + mScale[7]; }

	inline size_t GetSize() const { return 8; }

private:
	byte mKey;
	byte* mScale;	
};

#endif