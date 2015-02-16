#ifndef PIXELEFFECT_THEATRECHASE_H_INCLUDED
#define PIXELEFFECT_THEATRECHASE_H_INCLUDED


#include "PixelEffect.h"
#include "Timer.h"

/**
      Theatre-style crawling lights.
*/
class PixelEffect_TheatreChase : public PixelEffect
{
private:
    /** run states */
    typedef enum {kInit, kShow, kTimer} State;
    /** timer */
    Opcom::Timer m_timer;
    /** current run state */
    State m_state;
    /** Foreground color */
    uint32_t m_color;
    /** run in reverse or not */
    bool m_reverse;


public:
    /**
      Theatre-style crawling lights.
      @param strip Physical strip or logical Panel pointer
      @param color The color to wipe
      @param wait Milliseconds to wait between transitions
      @param reverse Reverses crawl direction
    */
    PixelEffect_TheatreChase(PixelStrip *strip, uint32_t color, uint32_t wait, bool reverse = false) :
        PixelEffect(strip), m_color(color), m_timer(wait), m_reverse(reverse)
    {
    }

    void init()
    {
        m_state = kInit;
    }

    void run()
    {
        static int q = 0;
        if(m_strip == NULL || !isEnabled()) return;
        switch(m_state)
        {
        case kInit:
            m_strip->clear();
            m_state = kShow;
            q = m_reverse ? 2 : 0;
        // no break
        case kShow:
            // deal with the ends
            m_strip->setPixelColor(0, 0);
            m_strip->setPixelColor(m_strip->numPixels()-1, 0);
            // turn on every third
            for (int i=0; i < m_strip->numPixels(); i=i+3)
            {
                if(m_reverse) {
                    if(i+q+1 < m_strip->numPixels())
                        m_strip->setPixelColor(i+q+1, 0);
                } else {
                    if(i+q-1 >= 0)
                        m_strip->setPixelColor(i+q-1, 0);
                    m_strip->setPixelColor(i+q, 0);
                }
                m_strip->setPixelColor(i+q, m_color);
            }
            if(m_reverse)
                q--;
            else
                q++;
            if(q < 0 || q > 2)
                q = m_reverse ? 2 : 0;
            m_state = kTimer;
            break;
        case kTimer:
            if(!m_timer.hasPeriodPassed())
                break;
            m_state = kShow;
        }
    }
};


#endif // PIXELEFFECT_THEATRECHASE_H_INCLUDED
