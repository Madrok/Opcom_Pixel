#ifndef PIXELEFFECT_FLASH_H_INCLUDED
#define PIXELEFFECT_FLASH_H_INCLUDED

#include "PixelEffect.h"
#include "Timer.h"

/**
  Flashes a color on and off, with configurable on and off time
*/
class PixelEffect_Flash : public PixelEffect
{
private:
    /** run states */
    typedef enum {kInit, kFirstColor, kTimer1, kSecondColor, kTimer2} State;
    /** On color */
    uint32_t m_color1;
    /** Off color */
    uint32_t m_color2;
    /** On timer */
    Opcom::Timer m_timer1;
    /** Off timer */
    Opcom::Timer m_timer2;
    /** current run state */
    State m_state;

public:
    /**
      Flash a color on and off.
      @param strip Physical strip or logical Panel pointer
      @param colorOn Color to flash
      @param onTime milliseconds for on state
      @param colorOff Color during 'off' state
      @param offTime milliseconds for off state
    */
    PixelEffect_Flash(PixelStrip *strip, uint32_t colorOn, uint32_t onTime, uint32_t colorOff, uint32_t offTime) :
        PixelEffect(strip), m_timer1(onTime), m_color1(colorOn), m_timer2(offTime), m_color2(colorOff)
    {

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
            setAll(m_color1);
            m_timer1.reset();
            m_state = kTimer1;
            break;
        case kTimer1:
            if(!m_timer1.hasPeriodPassed())
                break;
            m_state = kSecondColor;
        case kSecondColor:
            setAll(m_color2);
            m_timer2.reset();
            m_state = kTimer2;
            break;
        case kTimer2:
            if(!m_timer2.hasPeriodPassed())
                break;
            m_state = kFirstColor;
            break;
        }
    }

private:
    /**
      Sets all the pixels to one color
      @param color the RGB value
    */
    void setAll(uint32_t color) {
        if(m_strip)
            for(uint16_t i=0; i< m_strip->numPixels(); i++)
                m_strip->setPixelColor(i, color);
    }
};


#endif // PIXELEFFECT_FLASH_H_INCLUDED
