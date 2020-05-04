#ifndef _TEST_H
#define _TEST_H

#include "grid.h"
#include "scale.h"
#include "midi.h"

class Test
{
public:
	Test(Grid& trellis, bool justTest = false) :
		fTrellis(trellis), fJustTest(justTest) {}
	~Test() = default;
	
	void Run() {
		/* the array can be addressed as x,y or with the key number */
		for(int i=0; i < Y_DIM*X_DIM; i++){
		  fTrellis.SetPadColor(i, TurnOffLED()); //addressed with keynum
		  fTrellis.show();
		}

		Scale s(C4, MAJOR_SCALE);
		
		{ // INVERVAL TESTS
			// index 0
			TestHelper((short)4, s.GetKeyOctive());

			// index 1
		 	TestHelper((short)0, s.GetYIndexOfNote(C4));
		 	TestHelper((short)1, s.GetYIndexOfNote(0x48));
		 	TestHelper((short)2, s.GetYIndexOfNote(0x54));

		 	// index 1
		 	TestHelper((short)1, s.GetOctiveFromIndex(12, X_DIM, SCALE_MODE::CHROMATIC));

		 	// index 2
		 	TestHelper((short)1, s.GetOctiveFromIndex(8, X_DIM, SCALE_MODE::SCALE_ONLY));

		 	// index 2
		 	TestHelper(s.GetOctiveFromIndex(0, X_DIM, SCALE_MODE::CHROMATIC), s.GetOctiveFromIndex(0, X_DIM, SCALE_MODE::SCALE_ONLY));

		 	// index 3
		 	TestHelper(s.GetOctiveFromIndex(16, X_DIM, SCALE_MODE::CHROMATIC), s.GetOctiveFromIndex(10, X_DIM, SCALE_MODE::SCALE_ONLY));

		 	// index 4
		 	TestHelper(s.GetRoot(), static_cast<byte>(C4));

		 	// index 5
		 	TestHelper(s.Get2nd(), static_cast<byte>(C4 + 2));

		 	// index 6
		 	TestHelper(s.Get2nd(1), static_cast<byte>(C4 + 2 + NOTES_IN_OCT));

		 	// index 7
		 	TestHelper(s.GetOct(), static_cast<byte>(C4 + NOTES_IN_OCT));

		 	// index 8
		 	TestHelper(s.GetOct(), static_cast<byte>(C4 + NOTES_IN_OCT));

		 	// index 9
		 	TestHelper(s.GridIndexToNote(16, X_DIM, SCALE_MODE::CHROMATIC), s.GridIndexToNote(10, X_DIM, SCALE_MODE::SCALE_ONLY));

		 	// index 10
		 	TestHelper(s.GridIndexToNote(s.NoteToGridIndex(0x4C, X_DIM, SCALE_MODE::CHROMATIC), X_DIM, SCALE_MODE::CHROMATIC), (byte)0x4C); // E5

		 	// index 10
		 	TestHelper(s.GridIndexToNote(s.NoteToGridIndex(0x4C, X_DIM, SCALE_MODE::SCALE_ONLY), X_DIM, SCALE_MODE::SCALE_ONLY), (byte)0x4C); // E5

		 	// index 11
		 	TestHelper(s.NoteToGridIndex(s.GridIndexToNote(10, X_DIM, SCALE_MODE::CHROMATIC), X_DIM, SCALE_MODE::CHROMATIC), (byte)10);

		 	// index 12
		 	TestHelper(s.NoteToGridIndex(s.GridIndexToNote(10, X_DIM, SCALE_MODE::SCALE_ONLY), X_DIM, SCALE_MODE::SCALE_ONLY), (byte)10);
	 	}

		fTrellis.show();
		while(fJustTest); //
	}

private:
	template<class T>
	void TestHelper(T left, T right){
		if(left == right)
			fTrellis.SetPadColor(fTestCount, GreenLED());
		else
			fTrellis.SetPadColor(fTestCount, RedLED());
	 	fTestCount++;
	}

	Grid& fTrellis;
	bool fJustTest;
	int fTestCount{0};
};


#endif