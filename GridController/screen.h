#ifndef _SCREEN_H
#define _SCREEN_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)


class Screen
{
public:
	Screen(uint8_t w, uint8_t h) : display(w, h, &Wire, OLED_RESET) {}
	~Screen() = default;
	void begin() {
		// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
		if(not display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
			Serial.println(F("SSD1306 allocation failed"));
			for(;;); // Don't proceed, loop forever
		}

		// Show initial display buffer contents on the screen --
		// the library initializes this with an Adafruit splash screen.
		display.display();
		delay(2000); // Pause for 2 seconds

		// Clear the buffer
		display.clearDisplay();


		// Show initial display buffer contents on the screen --
		// the library initializes this with an Adafruit splash screen.
		display.display();
		delay(2000); // Pause for 2 seconds
	}

private:
	Adafruit_SSD1306 display;
};

#endif