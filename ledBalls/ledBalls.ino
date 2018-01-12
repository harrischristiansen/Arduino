// Harris Christiansen (harrischristiansen.com)
// 2018-01-11

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMLEDS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_KHZ800 + NEO_GRB);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int start=0;
int color=0;

int red=255;
int green=0;
int blue=0;

int keepChaser[NUMLEDS];

void loop() {
  
  // Rainbow Wheel
  for (int x=0; x<NUMLEDS; x++) {
    color = (x+start) % NUMLEDS;
    color = (int) color/5;
    if(color==0) {
      strip.setPixelColor(x, strip.Color(255, 0, 0));
      keepChaser[x] = 1;
    } else if(color==1) {
      strip.setPixelColor(x, strip.Color(0, 255, 0));
      keepChaser[x] = 1;
    } else {
      strip.setPixelColor(x, strip.Color(0, 0, 255));
      keepChaser[x] = 0;
    }
    
  }
  start++; // Step To Next Step
  if (start==NUMLEDS) { start=0; }
  
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
  for(int x=77; x<90; x++) {
    if (keepChaser[x] != 1) {
      strip.setPixelColor(x, strip.Color(red, green, blue));
    }
  }
  /*
  for(int x=0; x<NUMLEDS; x++) {
    if (x % 2 == 0) {
      strip.setPixelColor(x, strip.Color(red, green, blue));
    } else {
      strip.setPixelColor(x, strip.Color(255-red, 255-green, 255-blue));
    }
  }
  */
  
  strip.show();
  delay(100); // 100=chaser, 10=fader
}
