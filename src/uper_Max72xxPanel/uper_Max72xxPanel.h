#ifndef Max72xxPanel_h
#define Max72xxPanel_h

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
  #include "pins_arduino.h"
#endif

class UPER_Max72xxPanel : public Adafruit_GFX {

public:

  /*
   * Create a new controler
   * Parameters:
   * csPin		pin for selecting the device
   * hDisplays  number of displays horizontally
   * vDisplays  number of displays vertically
   */
  UPER_Max72xxPanel(byte csPin, byte hDisplays=1, byte vDisplays=1);

	/*
	 * Define how the displays are ordered. The first display (0)
	 * is the one closest to the Arduino.
	 */
	void setPosition(byte display, byte x, byte y);

	/*
	 * Define if and how the displays are rotated. The first display
	 * (0) is the one closest to the Arduino. rotation can be:
	 *   0: no rotation
	 *   1: 90 degrees clockwise
	 *   2: 180 degrees
	 *   3: 90 degrees counter clockwise
	 */
	void setRotation(byte display, byte rotation);

	/*
	 * Implementation of Adafruit's setRotation(). Probably, you don't
	 * need this function as you can achieve the same result by using
	 * the previous two functions.
	 */
	void setRotation(byte rotation);

  /*
   * Draw a pixel on your canvas. Note that for performance reasons,
   * the pixels are not actually send to the displays. Only the internal
   * bitmap buffer is modified.
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color);

  /*
   * As we can do this much faster then setting all the pixels one by
   * one, we have a dedicated function to clear the screen.
   * The color can be 0 (blank) or non-zero (pixel on).
   */
  void fillScreen(uint16_t color);

  /*
   * Set the shutdown (power saving) mode for the device
   * Paramaters:
   * status	If true the device goes into power-down mode. Set to false
   *		for normal operation.
   */
  void shutdown(boolean status);

  /*
   * Set the brightness of the display.
   * Paramaters:
   * intensity	the brightness of the display. (0..15)
   */
  void setIntensity(byte intensity);

  /*
   * After you're done filling the bitmap buffer with your picture,
   * send it to the display(s).
   */
  void write();

private:
  byte SPI_CS; /* SPI chip selection */

  /* Send out a single command to the device */
  void spiTransfer(byte opcode, byte data=0);

  /* We keep track of the led-status for 8 devices in this array */
  byte *bitmap;
  byte bitmapSize;

  byte hDisplays;
  byte *matrixPosition;
  byte *matrixRotation;
};

#endif	// Max72xxPanel_h



