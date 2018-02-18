// Harris Christiansen (harrischristiansen.com)
// 2018-01-11

#include <Adafruit_NeoPixel.h>

#define PIN         6
#define NUMLEDS     100
#define CHASERCOUNT NUMLEDS*7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_KHZ800 + NEO_GRB);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int loopCount = 0;
int start = 0;

uint32_t colorYellow = strip.Color(210, 250, 0);
uint32_t colorGreen = strip.Color(255, 0, 0);
uint32_t colorPurple = strip.Color(0, 130, 130);
uint32_t colorWhiteBlue = strip.Color(125, 160, 251);
uint32_t colorBlue = strip.Color(0, 0, 255);
uint32_t colorPink = strip.Color(20, 200, 195);

const int LIGHTMAP[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 81, 77, 86, 82, 78,
                87, 83, 79, 88, 84, 80, 89, 85, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

int keepChaser[NUMLEDS];
void renderLightChaser() {
  for (int x=0; x<NUMLEDS; x++) {
    keepChaser[LIGHTMAP[x]] = 0;

    if (x <= start-20) {
      strip.setPixelColor(LIGHTMAP[x], strip.Color(0, 0, 0));
    } else if (x < start-10) {
      int blueColor = 255 * (x-start-10) / 10;
      strip.setPixelColor(LIGHTMAP[x], strip.Color(0, 0, blueColor));
      keepChaser[LIGHTMAP[x]] = 1;
    } else if (x < start-5) {
      strip.setPixelColor(LIGHTMAP[x], strip.Color(0, 255, 0));
      keepChaser[LIGHTMAP[x]] = 1;
    } else if (x <= start) {
      strip.setPixelColor(LIGHTMAP[x], strip.Color(255, 0, 0));
      keepChaser[LIGHTMAP[x]] = 1;
    }
  }
}


int red = 255;
int green = 0;
int blue = 0;
void renderFader() {
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
  for(int x=0; x<NUMLEDS; x++) {
    if (keepChaser[x] != 1) {
      if (x % 2 == 0) {
        strip.setPixelColor(x, strip.Color(red, green, blue));
      } else {
        strip.setPixelColor(x, strip.Color(255-red, 255-green, 255-blue));
      }
    }
  }
  for(int x=77; x<90; x++) {
    if (keepChaser[x] != 1) {
      strip.setPixelColor(x, strip.Color(red, green, blue));
    }
  }
}

void renderFrozen() {
  for (int x=0; x<NUMLEDS; x++) {
    if (keepChaser[LIGHTMAP[x]] != 1) {
      int modVal = ((start/6)+x) % 2;
      if ((start/3+x) % 40 >= 39) {
        strip.setPixelColor(LIGHTMAP[x], strip.Color(20, 200, 195));
      } else if (modVal == 0) {
        strip.setPixelColor(LIGHTMAP[x], colorBlue);
      } else {
        strip.setPixelColor(LIGHTMAP[x], colorWhiteBlue);
      }
    }
  }
}

void renderCactus() {
  for (int x=0; x<NUMLEDS; x++) {
    if (keepChaser[LIGHTMAP[x]] != 1) {
      int modVal = ((start/8)+x) % 2;
      if ((start/4+x) % 40 >= 39) {
        strip.setPixelColor(LIGHTMAP[x], colorYellow);
      } else if (modVal == 0) {
        strip.setPixelColor(LIGHTMAP[x], colorGreen);
      } else {
        strip.setPixelColor(LIGHTMAP[x], colorPurple);
      }
    }
  }
}

void renderBirthday() {
  for (int x=0; x<NUMLEDS; x++) {
    if (keepChaser[LIGHTMAP[x]] != 1) {
      int modVal = ((start/8)+x) % 2;
      if ((start/4+x) % 40 >= 39) {
        strip.setPixelColor(LIGHTMAP[x], colorPink);
      } else if (modVal == 0) {
        strip.setPixelColor(LIGHTMAP[x], colorBlue); 
      } else {
        strip.setPixelColor(LIGHTMAP[x], colorPurple);
      }
    }
  }
}

void loop() {
  
  // Light Chaser
  renderLightChaser();
  
  // Main Theme
  if (loopCount % 2 == 0) {
    renderFrozen();
  } else {
    renderBirthday();
  }
  
  start++; // Step To Next Step
  if (start>=CHASERCOUNT) {
    start=0;
    loopCount++;
    }
  
  strip.show();
  delay(100); // 100=chaser, 10=fader
}
