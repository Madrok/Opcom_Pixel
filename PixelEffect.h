#ifndef OPCOM_PIXELEFFECT_H_INCLUDED
#define OPCOM_PIXELEFFECT_H_INCLUDED

/**
    The base class for any light effect. Light effects can be added to either a physical
    PixelStrip or a PixelPanel
 */
class PixelEffect {
protected:
    /** The panel or strip this effect is running on */
    PixelStrip *m_strip;
    /** true if effect is enabled and should be updated during run */
    bool m_enabled;

    /**
      Should be called by sub classes. Sets up the strip and enables the
      effect by default.
      @param strip Physical strip or logical Panel pointer
    */
    PixelEffect(PixelStrip *strip) {
        m_strip = strip;
        m_enabled = true;
    }

public:
    /**
      Disable running. This will prevent the light effect from updating during the call to run()
    */
    void disable() { m_enabled = false; }

    /**
      Enable running. This will cause the light effect to be updated during the call to run()
    */
    void enable() { m_enabled = true; }

    /**
      Check if this light effect is enabled or not.
      @returns True if enabled
    */
    bool isEnabled() { return m_enabled; }

    /**
      The init function, which must be called before the effect is run()
    */
    virtual void init() = 0;

    /**
      The run() function, which is called to update the state of the light effect
    */
    virtual void run() = 0;
};

#endif // OPCOM_PIXELEFFECT_H_INCLUDED
