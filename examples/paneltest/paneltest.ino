#include <Adafruit_NeoPixel.h>
#include <PixelEffect.h>
#include <PixelEffectStack.h>
#include <PixelEffectWithCallback.h>
#include <PixelEffect_Alternating.h>
#include <PixelEffect_ColorWipe.h>
#include <PixelEffect_Fade.h>
#include <PixelEffect_Fire.h>
#include <PixelEffect_Flash.h>
#include <PixelEffect_KnightRider.h>
#include <PixelEffect_Solid.h>
#include <PixelEffect_TheatreChase.h>
#include <PixelPanel.h>
#include <PixelStrip.h>
#include <Timer.h>

// Change these to match your strip
// PANEL_ONE_SIZE + PANEL_TWO_SIZE should equal PIXELS
#define PIN 6
#define PIXELS 16
#define PANEL_ONE_SIZE 8
#define PANEL_TWO_SIZE 8

// the NeoPixel physical strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);
// The first pixel panel
PixelPanel panel1 = PixelPanel(&strip, 0, PANEL_ONE_SIZE);
// a second pixel panel
PixelPanel panel2 = PixelPanel(&strip, PANEL_ONE_SIZE, PANEL_TWO_SIZE);

// create an effect on the first panel
PixelEffect_KnightRider peKR = PixelEffect_KnightRider(&panel1,16,3);
// create an effect on the second panel
PixelEffect_TheatreChase peTC = PixelEffect_TheatreChase(&panel2,0x00ff00,300,true);
  
void setup() {
  strip.begin();
  strip.clear();
  
  // initialize the effects. Just like calling a begin() function
  peKR.init();
  peTC.init();
  
  // panels can have individual brightness levels
  panel1.setBrightness(80);
  panel2.setBrightness(160);
}

void loop() {
  // run each effect. Changes will not occur until
  // strip.show() is called
  peKR.run();
  peTC.run();

  // update the physical device
  strip.show();
}


