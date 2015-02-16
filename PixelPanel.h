#ifndef OPCOM_NEOPIXELPANEL_H_INCLUDED
#define OPCOM_NEOPIXELPANEL_H_INCLUDED

/**
  A pixel panel is a logical division of a physical PixelStrip. These are virtual
  panels, so multiple panels ccan be on a physical strip.
*/
class PixelPanel : public PixelStrip {

protected:
    /** Pointer to the physical strip */
    PixelStrip *m_strip;
    /** starting pixel number in strip */
    uint16_t m_offset;
    /** total pixels in this logical panel */
    uint16_t m_numPixels;
    /** current brightness level for this logical panel */
    uint8_t m_brightness;

public:

    /**
      Create a logical panel inside a physical strip.
      @param strip The physical pixel strip
      @param startIdx The starting pixl number on the physical device. Start at 0 for first pixel.
      @param numPixels The number of pixels to reserve for this logical panel
    */
    PixelPanel(PixelStrip *strip, uint16_t startIdx, uint16_t numPixels) {
        m_strip = strip;
        m_offset = startIdx;
        m_numPixels = numPixels;
        m_brightness = 0;
    }

    /**
      Get the physical strip this panel is associated with
      @returns Pointer to PixelStrip
    */
    PixelStrip *getStrip() { return m_strip; };

    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
        if(m_strip && n < m_numPixels) {
            if(m_brightness) {
                r = (r * m_brightness) >> 8;
                g = (g * m_brightness) >> 8;
                b = (b * m_brightness) >> 8;
            }
            m_strip->setPixelColor(n+m_offset, r, g, b);
        }
    }

    void setPixelColor(uint16_t n, uint32_t c) {
        if(m_strip && n < m_numPixels) {
            if(m_brightness) { // brightness 0 == full bright
                uint8_t
                    r = (uint8_t)(c >> 16),
                    g = (uint8_t)(c >>  8),
                    b = (uint8_t)c;
                // here we let the other method scale the values
                setPixelColor(n, r, g, b);
            } else {
                m_strip->setPixelColor(n+m_offset, c);
            }
        }
    }

    void clear() {
        if(m_strip)
            for(int i = 0; i < m_numPixels; i++)
                m_strip->setPixelColor(i+m_offset, 0);
    }

    uint16_t numPixels(void) const {
        return m_numPixels;
    }

    uint32_t getPixelColor(uint16_t n) const {
        if(m_strip != NULL) {
            return m_strip->getPixelColor(n+m_offset);
        }
        return 0;
    }

    /**
      Returns the offset of this logical panel in the physical strip.
      @returns Pixel offset value
    */
    uint16_t getOffset() { return m_offset; }


    void setBrightness(uint8_t b) {
      // Stored brightness value is different than what's passed.
      // This simplifies the actual scaling math later, allowing a fast
      // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
      // adding 1 here may (intentionally) roll over...so 0 = max brightness
      // (color values are interpreted literally; no scaling), 1 = min
      // brightness (off), 255 = just below max brightness.
      uint8_t newBrightness = b + 1;
      if(newBrightness != m_brightness) { // Compare against prior value
        // Brightness has changed -- re-scale existing data in RAM
        if(m_strip != NULL) {
            uint32_t  c;
            uint8_t  oldBrightness = m_brightness - 1; // De-wrap old brightness value
            uint16_t scale;

            if(oldBrightness == 0) scale = 0; // Avoid /0
            else if(b == 255) scale = 65535 / oldBrightness;
            else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;

            for(uint16_t i=0; i<m_numPixels; i++) {
                c = m_strip->getPixelColor(i);
                uint8_t
                  r = (uint8_t)(c >> 16),
                  g = (uint8_t)(c >>  8),
                  b = (uint8_t)c;
                r = (r * scale) >> 8;
                g = (g * scale) >> 8;
                b = (b * scale) >> 8;
                m_strip->setPixelColor(i+m_offset, r, g, b);
            }
        }

        m_brightness = newBrightness;
      }
    }

    uint8_t getBrightness(void) const {
      return m_brightness - 1;
    }



};

#endif // OPCOM_NEOPIXELPANEL_H_INCLUDED
