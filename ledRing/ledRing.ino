// Harris Christiansen (harrischristiansen.com)
// 2014-09-12

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int start=0;
int color=0;

int red=255;
int green=0;
int blue=0;

void loop() {
  // Rainbow Wheel
  for(int x=0;x<16;x++) {
    color=x+start;
    if(color>=16) { color = color-16; }
    color = (int) color/5;
    if(color==0) {
      strip.setPixelColor(x, strip.Color(255, 0, 0));
    } else if(color==1) {
      strip.setPixelColor(x, strip.Color(0, 255, 0));
    } else {
      strip.setPixelColor(x, strip.Color(0, 0, 255));
    }
    
  }
  start++; // Step To Next Step
  if(start==16) { start=0; }
  
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
  for(int x=0;x<16;x++) {
    strip.setPixelColor(x, strip.Color(red, green, blue));
  }
  */
  
  strip.show();
  delay(100); // 100=chaser, 10=fader
}
