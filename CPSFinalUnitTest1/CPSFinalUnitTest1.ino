/*
  Systems test! Runs each mechanical system in a continuous loop while changing the LED colors
  to test functionality + indicate which system is supposed to be moving

  The loop repeats continuously:
    Red   - stepper moves one quarter-turn forward and backward
    Green - servo 1 sweeps from 0 to 180 and back
    Blue  - servo 2 sweeps from 0 to 180 and back
*/

#include <Servo.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>

// Same pinout as the main code
const int SERVO1_PIN = 9;
const int SERVO2_PIN = 8;

const int IN1 = A0;
const int IN2 = A1;
const int IN3 = A2;
const int IN4 = A3;

const int NUM_GROUPS = 3;
const int LEDS_PER_GROUP = 3;
const int LED_PINS[NUM_GROUPS] = {5, 6, 7};

Servo servo1;
Servo servo2;

// Variable number of steps per motor revolution.
const int stepsPerRevolution = 2048;

// Stepper library pin order, using A0-A3.
Stepper motor(stepsPerRevolution, A0, A2, A1, A3);

Adafruit_NeoPixel pixels[NUM_GROUPS] = {
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[2], NEO_GRB + NEO_KHZ800)
};

void setup() { //setup code before the loop runs
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(90);
  servo2.write(90);
  delay(500);

  for (int group = 0; group < NUM_GROUPS; group++) {
    pixels[group].begin();
    pixels[group].setBrightness(255);
    pixels[group].clear();
    pixels[group].show();
  }
   motor.setSpeed(8);

}

void loop() { // Main loop that it cycles through 
  // RED: stepper, one quarter-turn in each direction
  setAllLEDs(255, 0, 0);

  motor.step(4096);
  delay(1000);
  motor.step(-4096);
  delay(1000);

  // GREEN: servo 1 only, full sweep forward and backward
  setAllLEDs(0, 255, 0);

  for (int position = 0; position <= 180; position++) {
    servo1.write(position);
    delay(15);
  }

  for (int position = 180; position >= 0; position--) {
    servo1.write(position);
    delay(15);
  }

  // BLUE: servo 2 only, full sweep forward and backward
  setAllLEDs(0, 0, 255);

  for (int position = 0; position <= 180; position++) {
    servo2.write(position);
    delay(15);
  }

  for (int position = 180; position >= 0; position--) {
    servo2.write(position);
    delay(15);
  }
}

// helper function that sets all LEDs to the same color
// this allows us to create the function once and all it multiple times in the main loop (above) 
void setAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
  for (int group = 0; group < NUM_GROUPS; group++) {
    for (int led = 0; led < LEDS_PER_GROUP; led++) {
      pixels[group].setPixelColor(
        led,
        pixels[group].Color(red, green, blue)
      );
    }
    pixels[group].show();
  }
}
