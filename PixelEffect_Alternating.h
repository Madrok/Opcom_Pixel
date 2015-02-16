#ifndef PIXELEFFECT_ALTERNATING_H_INCLUDED
#define PIXELEFFECT_ALTERNATING_H_INCLUDED

#include "PixelEffect.h"
#include "Timer.h"

/**
  Alternating colors
  */
class PixelEffect_Alternating : public PixelEffect
{
private:
    /** run states */
    typedef enum {kInit, kFirstColor, kTimer1, kSecondColor, kTimer2} State;
    /** foreground color */
    uint32_t m_color1;
    /** background color */
    uint32_t m_color2;
    /** timer */
    Opcom::Timer m_timer;
    /** current run state */
    State m_state;

public:
    /**
      Alternate between two colors.
      @param strip Physical strip or logical Panel pointer
      @param color Foreground (on) RGB color to display
      @param wait Milliseconds to pause between changing
      @param color2 Background (off) color to display. Defaults to black (off)
    */
    PixelEffect_Alternating(PixelStrip *strip, uint32_t color, uint16_t wait, uint32_t color2=0) :
        PixelEffect(strip), m_timer(wait), m_color1(color), m_color2(color2)
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
            m_timer.reset();
            // no break
        case kFirstColor:
            for (int i=0; i < m_strip->numPixels(); i=i+2)
            {
                m_strip->setPixelColor(i, m_color1);    //turn every third pixel on
                if(i+1 < m_strip->numPixels()) m_strip->setPixelColor(i+1, m_color2);
            }
            m_state = kTimer1;
            break;
        case kTimer1:
            if(!m_timer.hasPeriodPassed())
                break;
            m_state = kSecondColor;
        case kSecondColor:
            for (int i=0; i < m_strip->numPixels(); i=i+2)
            {
                m_strip->setPixelColor(i, m_color2);    //turn every third pixel on
                if(i+1 < m_strip->numPixels()) m_strip->setPixelColor(i+1, m_color1);
            }
            m_state = kTimer2;
            break;
        case kTimer2:
            if(!m_timer.hasPeriodPassed())
                break;
            m_state = kFirstColor;
            break;
        }
    }
};


#endif // PIXELEFFECT_ALTERNATING_H_INCLUDED
