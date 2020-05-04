#ifndef _LEDS_H
#define _LEDS_H


enum LED_COLORS : uint32_t {
	OFF   = 0x000000,
	WHITE = 0xFFFFFF,

	RED = 0xFF0000,
	GREEN = 0x00FF00,
	BLUE = 0x0000FF,

	YELLOW = 0xFFFF00,
	PURPLE = 0X800080,
};

#endif