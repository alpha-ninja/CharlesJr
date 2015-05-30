#include <charlesjr.h>

// Define the CharlesJr board.
// The offset is 2, the port is D.
CharlesJr cjr(2, DDRD, PORTD);

// Just a counter
byte i = 0;

// Define the animation.
// This animation goes like:
// 00  (padding)
// XXXXXX defines the LED states.
byte animation[4] = {
  0b00100100,
  0b00010010,
  0b00001001,
  0b00000000};

// millis is used to know when to switch to the next frame
unsigned long lastMillis = millis();

// the duration of a frame in millis
int frameDuration = 50;

void setup() {
  // Nothing to do here.
}

void loop() {
  // When the frameDuration has passed, increment lastMillis by the frameduration
  // then increment i and and do a mod-4 operation so that it never exceeds 3
  if (millis() > lastMillis + frameDuration) {
    lastMillis += frameDuration;
    i = (i + 1) % 4;
  }
  // Set the display's LEDs to the current frame of the animation
  cjr.setLeds(animation[i]);
  // Display the frame on the display
  cjr.display();
}