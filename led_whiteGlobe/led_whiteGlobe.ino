// Harris Christiansen (harrischristiansen.com)
// File Created: January 28, 2017

#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 19
#define NUM_LEDS_TOTAL 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS_TOTAL, 6, NEO_RGB + NEO_KHZ400);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int start = 0;
int totalcount = 0;

int red = 255;
int green = 0;
int blue = 0;

int color = 0;
int currentDelay = 10;
void loop() {
  totalcount++;

  if (totalcount < 100) { // Blackness With Flashes
    currentDelay = 100;
    int flashled = random(0,NUM_LEDS);
    for (int x=0; x<NUM_LEDS; x++) {
      if (x == flashled) {
        strip.setPixelColor(x, strip.Color(200, 200, 200));
      } else {
        strip.setPixelColor(x, strip.Color(0, 0, 0));
      }
    }
  } else if (totalcount < 200) { // Fade In
    currentDelay = 100;
    for (int x=0; x<NUM_LEDS; x++) {
      int adjustby = random(0, 60) - 30;
      int brightness = ((totalcount-100)*255/100) + adjustby;
      if (brightness < 0) { brightness = 0; }
      if (brightness > 255) { brightness = 255; }
      strip.setPixelColor(x, strip.Color(brightness, brightness, brightness));
    }
  } else if (totalcount < 4000) { // Color Fader
    currentDelay = 10;
    if ((red!=0 && blue!=0) || blue==255) {
      blue--;
      red++;
    }
    if ((red!=0 && green!=0) || red==255) {
      red--;
      green++;
    }
    if ((green!=0 && blue!=0) || green==255) {
      green--;
      blue++;
    }
    for (int x=0; x<NUM_LEDS; x++) {
      strip.setPixelColor(x, strip.Color(red+x, green, blue));
    }
  } else { // Rainbow Wheel
    currentDelay = 100;
    for (int x=0; x<NUM_LEDS; x++) {
      color = (x+start) % NUM_LEDS;
      color = (int) color/5;
      if (color==0) {
        strip.setPixelColor(x, strip.Color(255, 0, 0));
      } else if (color==1) {
        strip.setPixelColor(x, strip.Color(0, 255, 0));
      } else {
        strip.setPixelColor(x, strip.Color(0, 0, 255));
      } 
    }
    start++; // Step To Next Step
    if (start == NUM_LEDS) { start=0; }
  }

  if (totalcount > 6000) {
    totalcount = 0;
  }
  
  strip.show();
  delay(currentDelay); // 100=chaser, 10=fader
}
