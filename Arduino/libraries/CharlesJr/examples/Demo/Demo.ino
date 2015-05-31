#include <charlesjr.h>

// Define the two CharlesJr boards.
// The offsets are 2 and 5, the port is D for both.
CharlesJr cjr(2, 0b000, DDRD, PORTD);
CharlesJr cjr2(5, 0b000, DDRD, PORTD);

void setup() {
  // Set the LEDs on display 2 to a triangle shape.
  cjr2.setLeds(0b00111010);
  // unset one of those LEDs
  cjr2.setPixel(1, 1, false);
}

void loop() {
  //Flash all LEDs on CJR1
  cjr.flash();
  //Display the shape we set earlier on CJR2
  cjr2.display();
}
