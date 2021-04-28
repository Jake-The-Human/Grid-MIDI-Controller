#ifndef _ENCODER_H
#define _ENCODER_H

#define ENCODER_PIN_1 4
#define ENCODER_PIN_2 5
#define ENCODER_BUTTON_PIN 6


void updateLeftEncoder() { 
	// if(digitalRead(ENCODER_BUTTON_PIN))
		Serial.println("Encoder LEFT");

}

void updateRightEncoder() { 
	// if(digitalRead(ENCODER_BUTTON_PIN))
		Serial.println("Encoder RIGHT");

}

class Encoder
{
public:
	void begin() {
		pinMode(ENCODER_PIN_1, INPUT);
		pinMode(ENCODER_PIN_2, INPUT);

		pinMode(ENCODER_BUTTON_PIN, INPUT);

		digitalWrite(ENCODER_PIN_1, HIGH); //turn pullup resistor on
		digitalWrite(ENCODER_PIN_2, HIGH); //turn pullup resistor on

		digitalWrite(ENCODER_BUTTON_PIN, HIGH); //turn pullup resistor on

		//call updateEncoder() when any high/low changed seen
		//on interrupt 0 (pin 2), or interrupt 1 (pin 3)
		attachInterrupt(0, updateLeftEncoder, CHANGE);
		attachInterrupt(1, updateRightEncoder, CHANGE);
	}

	bool TurnedLeft() const  { return not digitalRead(ENCODER_PIN_1); }
	bool TurnedRight() const { return not digitalRead(ENCODER_PIN_2); }
	bool ButtonPressed() const { return not digitalRead(ENCODER_BUTTON_PIN); }
private:

	int lastEncoded{ 0 };
	long encoderValue{ 0 };
};

#endif