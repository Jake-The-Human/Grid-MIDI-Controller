#ifndef _SCALE_H
#define _SCALE_H

#include "midi.h"
#include "grid.h"

#define WHOLE_STEP 2
#define HALF_STEP  1
#define NOTES_IN_OCT 12
#define NOTES_IN_SCALE 8
#define LOWEST_OCTIVE -1
#define HIGHEST_OCTIVE 8

enum class SCALE_MODE{ EDIT_MODE, CHROMATIC, SCALE_ONLY };

#define NUM_OF_SCALES 2
const byte MAJOR_SCALE[] = { 0, 2, 4, 5, 7, 9, 11, 12 };
const byte MINOR_SCALE[] = { 0, 2, 3, 5, 7, 9, 11, 12 };


class Scale
{
public:
	Scale(byte key, byte* scale) : mKey(key), mScale(scale) {}
	~Scale() = default;

	void ChangeKey(byte key) { mKey = key; }
	void ChangeOctive(int octive) {
		const short octiveDif{ octive - GetKeyOctive() };
		mKey += (NOTES_IN_SCALE * octiveDif);
	}
	void ChangeScale(byte* scale) { mScale = scale; }

	bool GoUpAnOct() { 
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

	inline byte GridIndexToNote(byte index, int x_dim, const SCALE_MODE& mode) const { 
		if(mode == SCALE_MODE::SCALE_ONLY)
			return (mKey + mScale[index % x_dim]) + ((index / x_dim ) * NOTES_IN_OCT); 
		else//(mode == MODE::CHROMATIC)
			return mKey + index;
	}

	inline byte NoteToGridIndex(byte note, int y_dim, const SCALE_MODE& mode) const {
		if(mode == SCALE_MODE::SCALE_ONLY) //  BinarySearchScale(mScale, (note - mKey)) + ((GetYIndexOfNote(note) - GetYIndexOfNote(highestGridNote)) * NOTES_IN_SCALE);
			return BinarySearchScale(mScale, (note - mKey) % NOTES_IN_OCT) + (GetYIndexOfNote(note) * y_dim);
		else//(mode == MODE::CHROMATIC)
			return note - GridIndexToNote(0, y_dim, mode); /*really x_dim but since this is a square grid its ok*/
	}

	inline byte GetKey() const { return mKey; }
	inline byte GetRoot(byte octive = 0) const { return (mKey + mScale[0]) + (octive * NOTES_IN_OCT); }
	inline byte Get2nd(byte octive = 0) const  { return (mKey + mScale[1]) + (octive * NOTES_IN_OCT); }
	inline byte Get3rd(byte octive = 0) const  { return (mKey + mScale[2]) + (octive * NOTES_IN_OCT); }
	inline byte Get4th(byte octive = 0) const  { return (mKey + mScale[3]) + (octive * NOTES_IN_OCT); }
	inline byte Get5th(byte octive = 0) const  { return (mKey + mScale[4]) + (octive * NOTES_IN_OCT); }
	inline byte Get6th(byte octive = 0) const  { return (mKey + mScale[5]) + (octive * NOTES_IN_OCT); }
	inline byte Get7th(byte octive = 0) const  { return (mKey + mScale[6]) + (octive * NOTES_IN_OCT); }
	inline byte GetOct(byte octive = 0) const  { return (mKey + mScale[7]) + (octive * NOTES_IN_OCT); }

	inline byte GetIntervalOffset(byte interval, const SCALE_MODE& mode) const { return mode == SCALE_MODE::SCALE_ONLY ? interval - 1 : mScale[interval - 1]; }
	inline byte GetIntervalOffset(byte interval) const { return mScale[interval - 1]; }

	short GetKeyOctive() const { return (mKey / NOTES_IN_OCT) - 1; }
	short GetNoteOctive(byte note) const { return (note / NOTES_IN_OCT) - 1; }
	short GetYIndexOfNote(byte note) const { return (note - mKey) / NOTES_IN_OCT; }
	short GetOctiveFromIndex(byte index, int x_dim, SCALE_MODE mode) const { return GetYIndexOfNote(GridIndexToNote(index, x_dim, mode)); }

private:
	byte mKey;
	byte* mScale;

	short BinarySearchScale(byte* scale, byte searching, size_t size = NOTES_IN_SCALE) {
		int l{0}, r{size -1};
		while (l <= r)
		{
			const int mid{ l + (r - l) / 2 };
			if (scale[mid] == searching)
				return mid;
			else if (scale[mid] < searching)
				l = mid + 1;
			else
				r = mid - 1;
		}

		return -1;
	}
};

#endif