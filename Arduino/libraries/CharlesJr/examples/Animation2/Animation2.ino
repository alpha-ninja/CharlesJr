#include <charlesjr.h>

// Define the CharlesJr board.
// The offsets are 2 and 5, the port is D for both.
CharlesJr cjr(2, DDRD, PORTD);
CharlesJr cjr2(5, DDRD, PORTD);

// Just a counter
byte i = 0;

// Define the animation.
// This animation goes like:
// 00  (padding)
// XXXXXX defines the LED states.
// animation[0] to animation[3] for display 1
// animation[4] to animation[7] for display 1
byte animation[8] = {
  0b00100010,
  0b00010001,
  0b00001000,
  0b00000100,
  0b00001000,
  0b00000100,
  0b00100010,
  0b00010001};

// millis is used to know when to switch to the next frame
unsigned long lastMillis = millis();

// the duration of a frame in millis
int frameDuration = 100;

void setup() {
  //nNothing to do here.
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
  cjr2.setLeds(animation[i + 4]); // add 4 to the index - see above
  
  // Display the frame on both displays
  cjr.display();
  cjr2.display();
}
