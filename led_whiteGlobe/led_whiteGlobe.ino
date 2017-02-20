// Harris Christiansen (harrischristiansen.com)
// File Created: January 28, 2017

#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 6, NEO_RGB + NEO_KHZ400);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int start=0;

int red=255;
int green=0;
int blue=0;

void loop() {
  int color = 0;
  
  // Rainbow Wheel
  for(int x=0; x<NUM_LEDS; x++) {
    color = (x+start) % NUM_LEDS;
    color = (int) color/5;
    if(color==0) {
      strip.setPixelColor(x, strip.Color(10, 0, 0));
    } else if(color==1) {
      strip.setPixelColor(x, strip.Color(0, 10, 0));
    } else {
      strip.setPixelColor(x, strip.Color(0, 0, 10));
    }
    
  }
  start++; // Step To Next Step
  if(start == NUM_LEDS) { start=0; }
  
  
  /*
  // Fader
  if((red!=0&&blue!=0)||blue==255) {
    blue--;
    red++;
  }
  if((red!=0&&green!=0)||red==255) {
    red--;
    green++;
  }
  if((green!=0&&blue!=0)||green==255) {
    green--;
    blue++;
  }
  for(int x=0;x < NUM_LEDS;x++) {
    strip.setPixelColor(x, strip.Color(red+x, green, blue));
  }
  */
  
  strip.show();
  delay(1000); // 100=chaser, 10=fader
}
