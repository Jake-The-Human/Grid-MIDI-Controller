#ifndef _CHORD_H
#define _CHORD_H

#include "grid.h"
#include "leds.h"
#include "midi.h"
#include "scale.h"

#define CHORD_BUFFER_SIZE 11
#define CHORD_END -1

#define NUM_OF_CHORDS 3

byte CHORD_BUFFER[CHORD_BUFFER_SIZE] = {-1};

class Chord
{
public:
	Chord(byte root, const Scale& scale) : mRoot(root), mScale(scale) {}
	Chord(byte root, byte scaleKey, byte* scale) : mRoot(root), mScale(scaleKey, scale) {}
	virtual ~Chord() = default;

	inline byte GetRoot() const { return mRoot; }
	virtual byte* GetChordNotes() const = 0;
	virtual void TrellisUpdate(Grid& trellis, bool noteOn, byte index) const {
		const SCALE_MODE scaleMode{ trellis.GetCurrentDisplayMode() };
		byte* chord{ GetChordNotes() };
		if(noteOn)
		{
			for(size_t i = 0; chord[i] != CHORD_END and i < CHORD_BUFFER_SIZE; ++i ) {
				const byte noteIndex{ mScale.NoteToGridIndex(chord[i], X_DIM, scaleMode) };
				trellis.SetPadColor(noteIndex, LED_COLORS::WHITE);
			}
		}
		else
		{
			for(size_t i = 0; chord[i] != CHORD_END and i < CHORD_BUFFER_SIZE; ++i ) {
				const byte noteIndex{ mScale.NoteToGridIndex(chord[i], X_DIM, scaleMode) };
				const uint32_t ledColor{ trellis.isActiveKey(noteIndex, mScale) };

				trellis.SetPadColor(noteIndex, ledColor);
			}
		}
	}

protected:
	byte mRoot;
	Scale mScale;
};


class MajorChord : public Chord
{
public:
	MajorChord(byte root, const Scale& scale) : Chord(root, scale) {}
	MajorChord(byte root, byte scaleKey) : Chord(root, scaleKey, MAJOR_SCALE) {}
	~MajorChord() = default;

	byte Get3rd() const {return mRoot + mScale.GetIntervalOffset(3); }
	byte Get5th() const {return mRoot + mScale.GetIntervalOffset(5); }

	virtual byte* GetChordNotes() const override {
		CHORD_BUFFER[0] = GetRoot();
		CHORD_BUFFER[1] = Get3rd();
		CHORD_BUFFER[2] = Get5th();
		CHORD_BUFFER[3] = CHORD_END;
		return CHORD_BUFFER;
	}
};


class MinorChord : public Chord
{
public:
	MinorChord(byte root, const Scale& scale) : Chord(root, scale) {}
	MinorChord(byte root, byte scaleKey) : Chord(root, scaleKey, MINOR_SCALE) {}
	~MinorChord() = default;

	byte Get3rd() const {return mRoot + mScale.GetIntervalOffset(3); }
	byte Get5th() const {return mRoot + mScale.GetIntervalOffset(5); }

	virtual byte* GetChordNotes() const override {
		CHORD_BUFFER[0] = GetRoot();
		CHORD_BUFFER[1] = Get3rd();
		CHORD_BUFFER[2] = Get5th();
		CHORD_BUFFER[3] = CHORD_END;
		return CHORD_BUFFER;
	}
};


class PowerChord : public Chord
{
public:
	PowerChord(byte root, const Scale& scale) : Chord(root, scale) {}
	PowerChord(byte root, byte scaleKey) : Chord(root, scaleKey, MAJOR_SCALE) {}
	~PowerChord() = default;

	byte Get5th() const {return mRoot + mScale.GetIntervalOffset(5); }

	virtual byte* GetChordNotes() const override {
		CHORD_BUFFER[0] = GetRoot();
		CHORD_BUFFER[1] = Get5th();
		CHORD_BUFFER[2] = CHORD_END;
		return CHORD_BUFFER;
	}
};


#endif