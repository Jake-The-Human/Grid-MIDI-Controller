# Grid-MIDI-Controller
MIDI Controller inspired by Ableton Push/Novation Launchpad.

## Overview 
- Parts
	- Arduino Uno
	- 4 Trellis Pad (set up in an 8x8 grid)
	- 2 buttons
	- MIDI jack

- Display Modes
	- Chromatic scale mode
		- You have all the notes available but the notes in the selected scale are highlighted.
	- Selected scale mode
		- You only have the notes in the scale available to you.
	- Setting mode
		- This is where you can pick your key, octave, scale, and chord.

- Playback Modes
	- Notes
		- Plays back only the note that you pushed.
	- Chords
		- Plays back the chord you have selected in the setting using the note you pushed and the root.
