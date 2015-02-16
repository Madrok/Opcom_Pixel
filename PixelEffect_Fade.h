#ifndef PIXELEFFECT_FADE_H_INCLUDED
#define PIXELEFFECT_FADE_H_INCLUDED

#include "PixelEffect.h"
#include "Timer.h"

/**
  This class fades the current display. The cbFinished will be called
  when this effect is complete
*/
class PixelEffect_Fade : public PixelEffectWithCallback
{
private:
    /** run states */
    typedef enum {kInit, kTimer, kFinished} State;
    /** timer */
    Opcom::Timer m_timer;
    /** current run state */
    State m_state;

public:
    /**
      Create a fader using delay between steps
      @param strip Physical strip or logical Panel pointer
      @param delay Delay between each fade step
    */
    PixelEffect_Fade(PixelStrip *strip, uint32_t delay) :
        PixelEffectWithCallback(strip), m_timer(delay)
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
            m_state = kTimer;
            m_timer.reset();
            // no break
        case kTimer:
            if(m_strip->getBrightness() > 0) {
                if(!m_timer.hasPeriodPassed())
                    break;
                m_strip->setBrightness(m_strip->getBrightness() - 1);
                m_timer.reset();
            } else {
                m_state = kFinished;
            }
        case kFinished:
            if(cbFinished) cbFinished(this);
            break;
        }
    }
};

#endif // PIXELEFFECT_FADE_H_INCLUDED
