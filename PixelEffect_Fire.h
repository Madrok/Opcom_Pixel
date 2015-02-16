#ifndef PIXELEFFECT_FIRE_H_INCLUDED
#define PIXELEFFECT_FIRE_H_INCLUDED

#include "PixelEffect.h"
#include "Timer.h"

// Based on the code from
// http://www.walltech.cc/neopixel-fire-effect/

/**
 A display that looks like fire
*/
class PixelEffect_Fire : public PixelEffect
{
private:
    /** run states */
    typedef enum {kInit, kFirstColor, kTimer} State;
    /** base red value */
    int16_t m_r;
    /** base green value */
    int16_t m_g;
    /** base blue value */
    int16_t m_b;
    /** timer */
    Opcom::Timer m_timer;
    /** current run state */
    State m_state;

public:
    /**
      Create a fader using delay between steps
      @param strip Physical strip or logical Panel pointer
    */
    PixelEffect_Fire(PixelStrip *strip) :
        PixelEffect(strip), m_timer(50)
    {
        m_r = 255;
        m_g = 215;
        m_b = 40;
    }

    /**
      Set the base values for the colors to flicker.
      @param r Red
      @param g Green
      @param b Blue
    */
    void setBaseColors(uint8_t r, uint8_t g, uint8_t b) {
        m_r = r;
        m_g = g;
        m_b = b;
    }

    void init() {
        m_state = kInit;
    }

    void run()
    {
        if(m_strip == NULL || !isEnabled()) return;
        switch(m_state)
        {
        case kInit:
            m_strip->clear();
            m_state = kFirstColor;
        // no break
        case kFirstColor:
            for(uint16_t x = 0; x < m_strip->numPixels(); x++)
            {
                int16_t flicker = random(0,150);
                int16_t r1 = m_r-flicker;
                int16_t g1 = m_g-flicker;
                int16_t b1 = m_b-flicker;
                if(g1<0) g1=0;
                if(r1<0) r1=0;
                if(b1<0) b1=0;
                m_strip->setPixelColor(x,r1,g1, b1);
            }
            m_timer.setInterval(random(50,150));
            m_state = kTimer;
            break;
        case kTimer:
            if(!m_timer.hasPeriodPassed())
                break;
            m_state = kFirstColor;
        }
    }
};

#endif // PIXELEFFECT_FIRE_H_INCLUDED
