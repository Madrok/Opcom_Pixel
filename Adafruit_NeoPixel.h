/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

// 'type' flags for LED pixels (third parameter to constructor):
#define NEO_RGB     0x00 // Wired for RGB data order
#define NEO_GRB     0x01 // Wired for GRB data order
#define NEO_BRG     0x04

#define NEO_COLMASK 0x01
#define NEO_KHZ800  0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x02
// Trinket flash space is tight, v1 NeoPixels aren't handled by default.
// Remove the ifndef/endif to add support -- but code will be bigger.
// Conversely, can comment out the #defines to save space on other MCUs.
#ifndef __AVR_ATtiny85__
#define NEO_KHZ400  0x00 // 400 KHz datastream
#endif

#include <PixelStrip.h>

/**
 Class for the physical Adafruit NeoPixel panel products.
*/
class Adafruit_NeoPixel : public PixelStrip
{

public:

    /**
      Create a NeoPixel Object
      @param n Number of leds
      @param p Pin number
      @param t LED type
    */
    Adafruit_NeoPixel(uint16_t n, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);
    ~Adafruit_NeoPixel();

    /** Initialize the NeoPixel */
    void begin(void);

    /**
        Refresh the data into the NeoPixel. This needs to be called to
        display the current data.
    */
    void show(void);

    /**
      Set the output pin number
    */
    void setPin(uint8_t p);

    /**
      Set pixel color from separate R,G,B components:
      @param n Pixel number
      @param r Red component (0-255
      @param g Green component
      @param b Blue component
    */
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

    /**
      Set pixel color from 'packed' 32-bit RGB color:
    */
    void setPixelColor(uint16_t n, uint32_t c);

    /**
      Adjust output brightness;   This does
      NOT immediately affect what's currently displayed on the LEDs.  The
      next call to show() will refresh the LEDs at this level.  However,
      this process is potentially "lossy," especially when increasing
      brightness.  The tight timing in the WS2811/WS2812 code means there
      aren't enough free cycles to perform this scaling on the fly as data
      is issued.  So we make a pass through the existing color data in RAM
      and scale it (subsequent graphics commands also work at this
      brightness level).  If there's a significant step up in brightness,
      the limited number of steps (quantization) in the old data will be
      quite visible in the re-scaled version.  For a non-destructive
      change, you'll need to re-render the full strip data.  C'est la vie.
      @param bright 0=darkest (off), 255=brightest.
    */
    void setBrightness(uint8_t bright);

    /**
      //Return the brightness value
    */
    uint8_t getBrightness(void) const;

    /**
      Sets all values to 0 (off)
    */
    void clear();

    /**
      Returns pointer to pixels[] array.  Pixel data is stored in device-
      native format and is not translated here.  Application will need to be
      aware whether pixels are RGB vs. GRB and handle colors appropriately.
    */
    uint8_t *getPixels(void) const;

    /**
      Get the number of pixels this strip was initialized to.
    */
    uint16_t numPixels(void) const;

    /**
      Convert separate R,G,B into packed 32-bit RGB color.
      Packed format is always RGB, regardless of LED strand color order.
    */
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

    /**
      Query color from previously-set pixel (returns packed 32-bit RGB value)
    */
    uint32_t getPixelColor(uint16_t n) const;

    inline bool canShow(void)
    {
        return (micros() - endTime) >= 50L;
    }

private:

    const uint16_t
    numLEDs,       // Number of RGB LEDs in strip
    numBytes;      // Size of 'pixels' buffer below
    uint8_t
    pin,           // Output pin number
    brightness,
    *pixels,        // Holds LED color values (3 bytes each)
    rOffset,       // Index of red byte within each 3-byte pixel
    gOffset,       // Index of green byte
    bOffset;       // Index of blue byte
    const uint8_t
    type;          // Pixel flags (400 vs 800 KHz, RGB vs GRB color)
    uint32_t
    endTime;       // Latch timing reference
#ifdef __AVR__
    const volatile uint8_t
    *port;         // Output PORT register
    uint8_t
    pinMask;       // Output PORT bitmask
#endif

};

#endif // ADAFRUIT_NEOPIXEL_H
