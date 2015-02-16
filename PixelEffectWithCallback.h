#ifndef PIXELEFFECTWITHCALLBACK_H_INCLUDED
#define PIXELEFFECTWITHCALLBACK_H_INCLUDED

#include <PixelEffect.h>

typedef bool (* effectCallback)(PixelEffect *);

/**
  Pixel effects that can fire callback events to handler functions.
  The use of the various callbacks is entirely up to the effect itself,
  so check the documentation of the effect.
*/
class PixelEffectWithCallback : public PixelEffect {

public:
    /** Generally called each time the effect starts over */
    volatile effectCallback cbInit;
    /** Callback slot */
    volatile effectCallback cbOne;
    /** Callback slot */
    volatile effectCallback cbTwo;
    /** Callback slot */
    volatile effectCallback cbThree;
    /** Callback slot */
    volatile effectCallback cbFour;
    /** Generally called if an effect actually has a 'finished' state */
    volatile effectCallback cbFinished;

protected:
    /** Base constructor */
    PixelEffectWithCallback(PixelStrip *strip) : PixelEffect(strip) {}
};


#endif // PIXELEFFECTWITHCALLBACK_H_INCLUDED
