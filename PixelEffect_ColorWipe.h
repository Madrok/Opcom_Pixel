#ifndef PIXELEFFECT_COLORWIPE_H_INCLUDED
#define PIXELEFFECT_COLORWIPE_H_INCLUDED

#include "PixelEffectWithCallback.h"

/**
  A color wipe that fills strip pixel by pixel. Defaults to immediately clearing
  and starting a new wipe.

  The cbInit function is called each time the effect starts over, before clearing
  the panel. cbFinished is called once the panel is full, after the pixels have displayed
  for the on time. Return false from cbFinished to stop the display from being cleared.
*/
class PixelEffect_ColorWipe : public PixelEffectWithCallback
{
private:
    /** run states */
    typedef enum {kInit, kSetPixels, kTimerPixel, kFilled, kTimerFilled, kDone} State;
    /** foreground color */
    uint32_t m_colorFG;
    //uint32_t m_colorBG;
    /** timer between pixels */
    Opcom::Timer m_timerPixel;
    /** timer at end of fill */
    Opcom::Timer m_timerFilled;
    /** true if background is cleared on first run */
    bool m_clearBackground;
    /** true if effect is reversed **/
    bool m_reversed;
    /** current run state */
    State m_state;
    /** current pixel to be filled */
    uint16_t m_curPixel;

public:
    /**
      Wipe a color in to fill a strip
      @param strip Pixel panel
      @param color Foreground color for fill
      @param delay milliseconds between pixels
    */
    PixelEffect_ColorWipe(PixelStrip *strip, uint32_t color, uint32_t delay) :
        PixelEffectWithCallback(strip), m_colorFG(color), m_timerPixel(delay),
        /*m_colorBG(0),*/ m_timerFilled(delay), m_clearBackground(true), m_reversed(false)
    {

    }

    /**
      Set to false if you do not wish the existing pixels cleared on first run.
      The strip will be cleared on each subsequent run regardless.
      @param v True to clear background before first run
    **/
    void setClearBackground(bool v) {
        m_clearBackground = v;
    }

    /**
        Set the delay at the end of the cycle when all the pixels have filled.
        By default this value is the same as the delay between pixels.
        @param delay Delay time in ms
    */
    void setFilledDelay(uint32_t delay) {
        m_timerFilled.setInterval(delay);
    }

    /**
        Set forward or reverse direction
    */
    void setReversed(bool reverse) {
        m_reversed = reverse;
    }

    void init() {
        m_state = kInit;
    }

    void run()
    {
        if(m_strip == NULL || !isEnabled() || m_state == kDone) return;
        switch(m_state)
        {
        case kInit:
            if(m_clearBackground)
                m_strip->clear();
            m_curPixel = m_reversed ? m_strip->numPixels() - 1 : 0;
            m_state = kSetPixels;
            if(cbInit) cbInit(this);
            // no break
        case kSetPixels:
            if(!m_reversed) {
                if(m_curPixel < m_strip->numPixels()) {
                    m_strip->setPixelColor(m_curPixel, m_colorFG);
                }
                m_curPixel++;
            } else {
                if(m_curPixel >= 0) {
                    m_strip->setPixelColor(m_curPixel, m_colorFG);
                }
                m_curPixel--;
            }
            m_timerPixel.reset();
            m_state = kTimerPixel;
            break;
        case kTimerPixel:
            if(!m_timerPixel.hasPeriodPassed())
                break;
            if(!m_reversed)
                m_state = (m_curPixel < m_strip->numPixels()) ? kSetPixels : kFilled;
            else
                m_state = (m_curPixel == 0) ? kFilled : kSetPixels;
            m_timerFilled.reset();
            m_state = kTimerFilled;
            break;
        case kTimerFilled:
            if(!m_timerFilled.hasPeriodPassed())
                break;
            m_state = kInit;
            if(cbFinished && !cbFinished(this))
                break;
            m_strip->clear();
        }
    }
};
#endif // PIXELEFFECT_COLORWIPE_H_INCLUDED
