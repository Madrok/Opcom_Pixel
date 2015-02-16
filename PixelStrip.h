#ifndef PIXELSTRIP_H_INCLUDED
#define PIXELSTRIP_H_INCLUDED

/*! \mainpage Opcom Pixel library

\section intro_sec Introduction
The Opcom NeoPixel Library is a stateful library for running light shows on the Adafruit NeoPixels, or any
pixel strip. Within the physical strip, multiple logical panels can be created and run independent lighting
effects, turning your one strip into a highly configurable lighting platform.

\section install_sec Installation
The Adafruit_NeoPixel library is included in this distribution, so you will not need to install it seperately.
Download the library, unzip, rename the folder to "Opcom_Pixel" and move it to your Arduino libraries folder.
After a restart of the Arduino IDE, open File->Sketchbook->Library->Opcom_Pixel->paneltest sketch. Change the
definitions for PIN and NUMPIXELS and upload.

\section compat Compatibility
The following products should work with the library, as per the included Adafruit_NeoPixel library.
<ul>
<li>flora:  http://adafruit.com/products/1060</li>
<li>strip:  http://adafruit.com/products/1138</li>
<li>pixel:  http://adafruit.com/products/1312</li>
<li>stick:  http://adafruit.com/products/1426</li>
<li>shield: http://adafruit.com/products/1430</li>
</ul>
*/

/**
  A virtual base class for any sort of strip of led pixels. This is a physical implementation, so this
  class represents the actual physical device.
*/
class PixelStrip {

public:
    /**
        Set a specific pixel to the r,g,b color value.
        @param n Pixel number
        @param r Red component
        @param g Green component
        @param b Blue component
    */
    virtual void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) = 0;

    /**
        Set a specific pixel to the RGB color value.
        @param n Pixel number
        @param c RGB Color
    */
    virtual void setPixelColor(uint16_t n, uint32_t c) = 0;

    /**
        Clear the panel (set to color 0 or dark)
    */
    virtual void clear() = 0;

    /**
        Get the number of pixels in the panel
        @returns Count of pixels
    */
    virtual uint16_t numPixels(void) const = 0;

    /**
        Return the RGB color value in a particular pixel
        @returns RGB color value
    */
    virtual uint32_t getPixelColor(uint16_t n) const = 0;

    /**
        Get the brightness of the strip
        @returns 0-255 where 0 is dark, 255 full brightness
    */
    virtual uint8_t getBrightness(void) const = 0;

    /**
        Set the brightness of the strip.
        0 is lowest, 255 highest brightness
        @param b Brightness value, 8 bit unsigned
    */
    virtual void setBrightness(uint8_t b) = 0;

    /**
      Set all the leds to a single color. show() must be called to take effect.
      @param color The color to set all leds to
    */
    virtual void setAllPixels(uint32_t color) {
        for(uint16_t i=0; i< numPixels(); i++) {
          setPixelColor(i, color);
        }
    }
};


#endif // PIXELSTRIP_H_INCLUDED
