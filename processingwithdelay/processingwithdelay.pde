/*
  processing_osc_to_arduino

  Receives Wekinator OSC messages and sends a class to Arduino only
  after the same class has been received for  a set # consecutive counts.
  This stabilizes the output so hardware doesn't respond to random 
  variance in the class read. 
  
  to be used with face osc + mirror box arduino code
*/

import oscP5.*;
import processing.serial.*;

OscP5 oscP5;
Serial port;

int arduinoMessage = -1;
int candidateClass = -1; // set random non-class # to fill integer spot
int candidateCount = 0;

final int OSC_PORT = 12000;
final int REQUIRED_COUNTS = 20; 
// where ton change the # of counts in a row before it passes onto wekintaor

void setup() {
  size(256, 150);

  println("Available serial ports:");
  printArray(Serial.list());

  // Change index of your Arduino port.
  port = new Serial(this, Serial.list()[4], 9600);

  // Wekinator must send OSC to this Processing sketch on port 12000.
  oscP5 = new OscP5(this, OSC_PORT);
  println("Listening for Wekinator OSC on port " + OSC_PORT);
}

void draw() {
  for (int i = 0; i < 256; i++) {
    stroke(i);
    line(i, 0, i, 150);
  }

  // The stability filter is count-based and runs when OSC messages arrive.
}

void oscEvent(OscMessage theOscMessage) {
  // original Wekinator OSC parsing 
  if (theOscMessage.checkAddrPattern("/wek/outputs") == true) {
    if (theOscMessage.checkTypetag("f")) {
      float firstValue = theOscMessage.get(0).floatValue();
      int receivedClass = (int)firstValue;

      if (receivedClass == candidateClass) {
        candidateCount++;
      } else {
        candidateClass = receivedClass;
        candidateCount = 1;
      }
// print candidate class + # of instances in a row
      println("Candidate class: " + candidateClass +
              "  count: " + candidateCount + "/" + REQUIRED_COUNTS);

      // Send once after [REQUIRED_COUNRS] consecutive identical class values.
      if (candidateCount == REQUIRED_COUNTS) {
        updateArduino(candidateClass);
      }
    }
  }
}
// what's sent to arduino
void updateArduino(int newMessage) {
  if (newMessage != arduinoMessage) {
    arduinoMessage = newMessage;
    println("Sending stable class to Arduino: " + arduinoMessage);
    port.write(arduinoMessage);
  }
}
