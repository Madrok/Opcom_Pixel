#ifndef PIXELEFFECT_NIGHTRIDER_H_INCLUDED
#define PIXELEFFECT_NIGHTRIDER_H_INCLUDED


#include "PixelEffect.h"
#include "Timer.h"

/*
// Based on NeoPixel-KnightRider
//
// A highly configurable Knight Rider (larson display) routine for your NeoPixels
// (WS2812 RGB LED)
//-----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2013 Technobly - technobly@gmail.com - August 13th 2013
*/

/**
  A Kinght Rider effect of pixels moving back and forth across the panel
*/
class PixelEffect_KnightRider : public PixelEffect
{
private:
    /** run states */
    typedef enum {kInit, kStep1, kTimer1, kStep2, kTimer2} State;
    /** foreground color */
    uint32_t m_color;
    /** width of effect */
    uint8_t m_width;
    /** pixel timer */
    Opcom::Timer m_timer;
    /** current run state */
    State m_state;

    /** pixel buffer for old values */
    uint32_t *m_pixelBuf;
    /** number of pixels in strip */
    uint16_t m_numPix;

public:
    /**
      Does a display like the car Kit in the Knight Rider TV show
      @param strip Physical strip or logical Panel pointer
      @param speed how fast one cycle is (32 with 16 pixels is default KnightRider speed)
      @param width how wide the trail effect is on the fading out LEDs.  The original display used
             light bulbs, so they have a persistance when turning off.  This creates a trail.
             Effective range is 2 - 8, 4 is default for 16 pixels.  Play with this.
      @param color 32-bit packed RGB color value. Defaults to TV series color
    */
    PixelEffect_KnightRider(PixelStrip *strip, uint16_t speed=32, uint8_t width=4, uint32_t color=0xFF1000) :
        PixelEffect(strip), m_timer(speed), m_width(width), m_color(color)
    {
        if(strip && strip->numPixels() > 0) {
            m_pixelBuf = (uint32_t *)malloc(strip->numPixels());
            m_numPix = strip->numPixels();
        }
        else {
            m_pixelBuf = NULL;
            m_numPix = 0;
        }
    }

    ~PixelEffect_KnightRider() {
        if(m_pixelBuf) free(m_pixelBuf);
    }

    void init() {
        m_state = kInit;
    }

    void run()
    {
        static int16_t count = 0;
        if(m_strip == NULL || !isEnabled() || !m_pixelBuf) return;
        switch(m_state)
        {
        case kInit:
            m_strip->clear();
            memset(m_pixelBuf, 0, m_numPix);
            m_state = kStep1;
            count = 1;
            // no break
        case kStep1:
            m_strip->setPixelColor(count, m_color);
            m_pixelBuf[count] = m_color;
            for(int x = count; x>0; x--)
            {
                m_pixelBuf[x-1] = dimColor(m_pixelBuf[x-1], m_width);
                m_strip->setPixelColor(x-1, m_pixelBuf[x-1]);
            }
            m_state = kTimer1;
            break;

        case kTimer1:
            if(!m_timer.hasPeriodPassed())
                break;
            count++;
            if(count < m_numPix) {
                m_state = kStep1;
            } else {
                count = m_numPix-1;
                m_state = kStep2;
            }
            break;

        case kStep2:
            //for (int count = m_numPix-1; count>=0; count--)
            m_strip->setPixelColor(count, m_color);
            m_pixelBuf[count] = m_color;
            for(int x = count; x<=m_numPix ; x++)
            {
                if(x>0) m_pixelBuf[x-1] = dimColor(m_pixelBuf[x-1], m_width);
                m_strip->setPixelColor(x+1, m_pixelBuf[x+1]);
            }
            m_state = kTimer2;
            break;

        case kTimer2:
            if(!m_timer.hasPeriodPassed())
                break;
            count--;
            if(count >=0) {
                m_state = kStep2;
            } else {
                m_timer.reset();
                count = 1;
                m_state = kStep1;
            }
            break;
        }
    }

private:
    /**
      Reduce the brightness of a color over a certain distance in pixels
      @param color 32 bit color value
      @param width distance from centre pixel
    */
    static uint32_t dimColor(uint32_t color, uint8_t width) {
       return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
    }
};


#endif // PIXELEFFECT_NIGHTRIDER_H_INCLUDED
