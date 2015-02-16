#ifndef PIXELEFFECT_SOLID_H_INCLUDED
#define PIXELEFFECT_SOLID_H_INCLUDED

/**
 Create a solid color that does not change. Every call to run will
 recreate the color, so in most cases, this effect can be run() once
 then disabled, if no other display is running on the panel.
*/
class PixelEffect_Solid : public PixelEffect {

protected:
    uint32_t m_color;

public:
    /**
      Create a solid fill.
      @param strip Physical strip or logical Panel pointer
      @param color Solid RGB color to display
    */
    PixelEffect_Solid(PixelStrip *strip, uint32_t color) :
        PixelEffect(strip), m_color(color)
    {
    }

    void init() {
    }

    void run()
    {
        if(m_strip == NULL || !isEnabled()) return;
        for(uint16_t i = 0; i < m_strip->numPixels(); i++)
            m_strip->setPixelColor(i, m_color);
    }
};

#endif // PIXELEFFECT_SOLID_H_INCLUDED
