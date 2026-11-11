/*
   Controls the origami structures based on facial expression inputs
   1. Happy
   2. Sad
   3. Surprised

   To be used with the Processing sketch:
   processingwithdelay.pde

   This code defines behavior functions for each component:
   - 2 Servos
   - Stepper 
   - Addressable LEDs

   This code uses a case and switch structure to change the behavior of the system based on Wekinator class inputs. 
   To simplify the code structure, the motors and LEDs are controlled using helper functions that are located after 
   the main case and switch body of the code. 
   
   To tune the behaviors, edit the motor speed values in the case/switch section or LED behavior section
*/

// VARIABLES + OBJECTS -----------------------------------------------------------------------------------------------------------------

//Include Libraries
#include <Servo.h>
#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>

// Pin setup
const int servo1_pin = 9;
const int servo2_pin = 8;

const int IN1 = A0;
const int IN2 = A1;
const int IN3 = A2;
const int IN4 = A3;

const int NUM_GROUPS = 3;
const int LEDS_PER_GROUP = 3;
const int TOTAL_LEDS = NUM_GROUPS * LEDS_PER_GROUP;
const int LED_PINS[NUM_GROUPS] = {5, 6, 7};

// Servos variables 
Servo servo1;
Servo servo2;

// Shared servo range used in all expression cases
const int SERVO_START_POSITION = 00;
const int SERVO_END_POSITION = 180;

int servoPosition = SERVO_START_POSITION;
int servoDirection = 1;
int previousServoSpeed = -1;
unsigned long lastServoUpdate = 0;

// Stepper movement: 180 degrees forward, then 160 degrees back in all cases
// Movement is relative; no start or end position is recorded.
const long STEPS_PER_REVOLUTION = 4096; // total steps per rev. for 4 wire stepper
const long STEPS_FORWARD = STEPS_PER_REVOLUTION / 2;
const long STEPS_BACK = (STEPS_PER_REVOLUTION * 160L) / 360L;

// Stepper movement uses a speed input and one fixed acceleration value.
const long STEPPER_ACCELERATION = 400; // lower if stepper is having issues with load
long previousStepperSpeed = -1;
bool stepperMovingForward = true;

AccelStepper stepper(
  AccelStepper::HALF4WIRE,
  IN1, IN3, IN2, IN4
);

// NeoPixel LED groups
Adafruit_NeoPixel pixels[NUM_GROUPS] = {
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(LEDS_PER_GROUP, LED_PINS[2], NEO_GRB + NEO_KHZ800)
};

int ledMode = -1;
unsigned long lastLEDUpdate = 0;

// Default soft white pulse
const unsigned long DEFAULT_UPDATE_MS = 25;
const unsigned long DEFAULT_CYCLE_MS = 4000;
const int DEFAULT_MIN_BRIGHTNESS = 12;
const int DEFAULT_MAX_BRIGHTNESS = 128;

// Happy mode
const unsigned long HAPPY_UPDATE_MS = 50;
const int HAPPY_BRIGHTNESS = 255;
const int HAPPY_SPEED = 3;
const int HAPPY_SPREAD = 256 / TOTAL_LEDS;
int rainbowOffset = 0;

// Sad mode
const unsigned long SAD_UPDATE_MS = 25;
const unsigned long SAD_CYCLE_MS = 6000;
const int SAD_MIN_BRIGHTNESS = 20;
const int SAD_MAX_BRIGHTNESS = 180;
const uint8_t SAD_RED[NUM_GROUPS] = {0, 132, 0};
const uint8_t SAD_GREEN[NUM_GROUPS] = {0, 0, 220};
const uint8_t SAD_BLUE[NUM_GROUPS] = {255, 255, 209};

// Surprised mode
const unsigned long RUNNING_LIGHT_UPDATE_MS = 100;
const int IDLE_RED_BRIGHTNESS = 200;
const int NUM_COLORS = 6;

const uint8_t PALETTE_R[NUM_COLORS] = {180, 255, 255, 255, 230, 150};
const uint8_t PALETTE_G[NUM_COLORS] = {0, 35, 85, 145, 65, 12};
const uint8_t PALETTE_B[NUM_COLORS] = {8, 5, 10, 35, 95, 18};

int runningLightPosition = 0;

int expression = 0; // expression = class 

// PRE RUN SETUP ---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600); // check the baud rate 

  servo1.attach(servo1_pin); // attach servos 
  servo2.attach(servo2_pin);

  for (int group = 0; group < NUM_GROUPS; group++) {
    pixels[group].begin(); //initializes LED strip
    pixels[group].clear(); // clears all previous commands + turns the strip off
    pixels[group].show();
  }
}

// MAIN CODE LOOP---------------------------------------------------------------------------------------------------------
void loop() {
  if (Serial.available() > 0) {
    expression = Serial.read();
  } // recieve expression class, check if it's >0 because the beginning value was -1 in processing 
  // program will continue the current behavior until a new expression value is received

  switch (expression) { // to tune motor behaviors, change the speed variables in moveServos and moveStepper
    case 1: //happy
      moveServos(60);
      moveStepper(600);
      controlLEDs(1); 
      break;

    case 2: //sad
      moveServos(20);
      moveStepper(200);
      controlLEDs(2);
      break;

    case 3: //surprised
      moveServos(120);
      moveStepper(800);
      controlLEDs(3);
      break;

    default: //behavior when no expression is received 
      stepper.stop(); //requests a stop that's executed with stepper.run
      controlLEDs(0); // case 0 is default, not off
      break;
  }
  
  stepper.run(); //call at the end of the loop to keep the motor moving
}

// MOTOR HELPER FUNCTIONs ------------------------------------------------------------------------------------------------

// moveServos function (speed is in degrees per second, servo1 and servo2 sweep in opposite directions)
void moveServos(int speed) {
  speed = constrain(speed, 1, 1000);

  unsigned long stepInterval = 1000UL / speed;
  unsigned long currentTime = millis();

  // Set the initial positions the first time this function is called.
  if (previousServoSpeed == -1) {
    servo1.write(servoPosition);
    servo2.write(180 - servoPosition); // opposite movement so opposite starting position
    lastServoUpdate = currentTime;
  }

  previousServoSpeed = speed;
 
  if (currentTime - lastServoUpdate >= stepInterval) {
    servoPosition += servoDirection;

    if (servoPosition >= SERVO_END_POSITION) {
      servoPosition = SERVO_END_POSITION;
      servoDirection = -1;
    } else if (servoPosition <= SERVO_START_POSITION) {
      servoPosition = SERVO_START_POSITION;
      servoDirection = 1;
    }

    servo1.write(servoPosition);
    servo2.write(SERVO_START_POSITION + SERVO_END_POSITION - servoPosition);

    lastServoUpdate += stepInterval;
  }
}

// moveStepper function (speed is in steps/second, motor moves 180 degrees forward, then 160 degrees back)
// Each movement is relative and does not record start or end positions. this is to account for variance in 
// forward vs. backward movement due to differing loads

void moveStepper(long speed) {
  speed = constrain(speed, 1, 2000);

  if (speed != previousStepperSpeed) {
    previousStepperSpeed = speed;
    stepper.setMaxSpeed(speed);
    stepper.setAcceleration(STEPPER_ACCELERATION);
  }

  if (stepper.distanceToGo() == 0) {
    if (stepperMovingForward) {
      stepper.move(STEPS_FORWARD);
    } else {
      stepper.move(-STEPS_BACK);
    }

    stepperMovingForward = !stepperMovingForward;
  }
}

// LED HELPER FUNCTIONs --------------------------------------------------------------------------------------------------
void setGroupColor(int group, uint32_t color) { //sets every LED in a group to the same color
  for (int led = 0; led < LEDS_PER_GROUP; led++) {
    pixels[group].setPixelColor(led, color);
  }
}

void showAllGroups() { // forces all LEDs to update
  for (int group = 0; group < NUM_GROUPS; group++) {
    pixels[group].show();
  }
}

uint32_t colorWheel(byte hue, Adafruit_NeoPixel &strip) { //gradually changes RGB values to create fading rainbow effect
  hue = 255 - hue;

  if (hue < 85) {
    return strip.Color(255 - hue * 3, 0, hue * 3);
  }

  if (hue < 170) {
    hue -= 85;
    return strip.Color(0, hue * 3, 255 - hue * 3);
  }

  hue -= 170;
  return strip.Color(hue * 3, 255 - hue * 3, 0);
}

// LED controls
void controlLEDs(int mode) { //overall LED control function 
  // checks if last requested mode is the same as new mode. this is important because it can recieve multiple of the 
  // same classes in a row and will prevent the LED sequence from restarting 
  if (mode != ledMode) { 
    ledMode = mode;
    lastLEDUpdate = 0;
    rainbowOffset = 0; // resets the rainbow position so it starts from the beginning 

    if (mode == 3) {
      runningLightPosition = 0; // since surprised using running lights, this restarts it at the first light 
    }

    for (int group = 0; group < NUM_GROUPS; group++) { // clears commands from the LEDs 
      pixels[group].clear();
      pixels[group].show();
    }
  }

  unsigned long currentTime = millis(); // sets currentTime to time since program started

  // Mode 1: Happy (rainbow fading lights) 
   if (mode == 1) {
    if (currentTime - lastLEDUpdate >= HAPPY_UPDATE_MS) { // Update the surprised animation every HAPPY_UPDATE_MS milliseconds
      for (int group = 0; group < NUM_GROUPS; group++) {
        pixels[group].setBrightness(HAPPY_BRIGHTNESS); // set consistent brightness

        for (int led = 0; led < LEDS_PER_GROUP; led++) {
          int overallLED = group * LEDS_PER_GROUP + led; //led group setup
          byte hue = (overallLED * HAPPY_SPREAD + rainbowOffset) & 255; // calculate colors across wheel 
          pixels[group].setPixelColor(led, colorWheel(hue, pixels[group])); // converts to RGB, stores it 
        }
      }

      showAllGroups(); // sends color info to LEDs
      rainbowOffset = (rainbowOffset + HAPPY_SPEED) & 255;
      lastLEDUpdate = currentTime;
    }
  }
  
  // Mode 2: Sad // cool colors, fade in and out 
   else if (mode == 2) {
    if (currentTime - lastLEDUpdate >= SAD_UPDATE_MS) { // Update the surprised animation every SAD_UPDATE_MS milliseconds
      unsigned long cyclePosition = currentTime % SAD_CYCLE_MS;
      unsigned long halfCycle = SAD_CYCLE_MS / 2;
      int brightness;

      if (cyclePosition < halfCycle) {
        brightness = map(cyclePosition, 0, halfCycle,
                         SAD_MIN_BRIGHTNESS, SAD_MAX_BRIGHTNESS);
      } else {
        brightness = map(cyclePosition, halfCycle, SAD_CYCLE_MS,
                         SAD_MAX_BRIGHTNESS, SAD_MIN_BRIGHTNESS);
      }

      unsigned long colorStep = currentTime / (SAD_CYCLE_MS / NUM_GROUPS);

      for (int group = 0; group < NUM_GROUPS; group++) {
        int colorIndex = (colorStep + group) % NUM_GROUPS;
        setGroupColor(group, pixels[group].Color(
          ((int)SAD_RED[colorIndex] * brightness) / 255, // set the red, green, and blue components after applying brightness scaling
          ((int)SAD_GREEN[colorIndex] * brightness) / 255, 
          ((int)SAD_BLUE[colorIndex] * brightness) / 255  
        ));
      }

      showAllGroups(); //pushes updates to LEDs
      lastLEDUpdate = currentTime;
    }
  }

  

  // Mode 3: Surprised running lights
  else if (mode == 3) {
    if (currentTime - lastLEDUpdate >= RUNNING_LIGHT_UPDATE_MS) { // Update the surprised animation every RUNNING_LIGHT_UPDATE_MS milliseconds
      for (int group = 0; group < NUM_GROUPS; group++) {
        pixels[group].setBrightness(255);

        for (int led = 0; led < LEDS_PER_GROUP; led++) {
          pixels[group].setPixelColor(
            led,
            pixels[group].Color(
              IDLE_RED_BRIGHTNESS, // leds that aren't "running" are idle red to fill space with color
              0,
              0
            )
          );
        }
      }

      int group = runningLightPosition / LEDS_PER_GROUP;
      int led = runningLightPosition % LEDS_PER_GROUP;
      int colorIndex = runningLightPosition % NUM_COLORS;

      pixels[group].setPixelColor(
        led,
        pixels[group].Color( // pull colors from colorIndex
          PALETTE_R[colorIndex],
          PALETTE_G[colorIndex],
          PALETTE_B[colorIndex]
        )
      );

      showAllGroups();

      runningLightPosition++;
      if (runningLightPosition >= TOTAL_LEDS) {
        runningLightPosition = 0;
      }

      lastLEDUpdate = currentTime;
    }
  }

  // Default: soft pulsing white
  else {
    if (currentTime - lastLEDUpdate >= DEFAULT_UPDATE_MS) {
      unsigned long cyclePosition = currentTime % DEFAULT_CYCLE_MS;
      unsigned long halfCycle = DEFAULT_CYCLE_MS / 2;
      int brightness;

      if (cyclePosition < halfCycle) { // brighten for first half of cycle
        brightness = map(cyclePosition, 0, halfCycle,
                         DEFAULT_MIN_BRIGHTNESS, DEFAULT_MAX_BRIGHTNESS);
      } else {
        brightness = map(cyclePosition, halfCycle, DEFAULT_CYCLE_MS, // darken for second half of cycle
                         DEFAULT_MAX_BRIGHTNESS, DEFAULT_MIN_BRIGHTNESS);
      }

      for (int group = 0; group < NUM_GROUPS; group++) {
        pixels[group].setBrightness(255);
        setGroupColor(group, pixels[group].Color(
          brightness, brightness, brightness
        ));
      }

      showAllGroups(); // push changes to LEDs 
      lastLEDUpdate = currentTime;
    }
  }
}
