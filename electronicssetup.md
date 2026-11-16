# Electronic Schematic Assembly Guide

This guide summarizes the electronic connections used in the mirror box project. Follow the schematic when wiring the Arduino, servos, LED strips, stepper motor, driver board, and power system.

## Arduino and Pin Assignments

The project uses an **Arduino Nano**. The pin assignments below match the current schematic, but they can be changed in the control code if needed. If you change a connection, update the corresponding pin definition in the code as well.

**9g Blue Servos**
- Servo 1 signal: D8
- Servo 2 signal: D9

**Addressable LEDs**
- LED strip 1 : D7
- LED strip 2 : D6
- LED strip 3 & 4 : D5 (connected together since it was added last minute) 

**Stepper Motor** -  connected through a driver board. Connect the driver's coil inputs to the Arduino's analog pins as follows:

- IN1 → A0
- IN2 → A1
- IN3 → A2
- IN4 → A3

E
## Power Regulation
A power delivery board (represented by the power and ground symbols in the schematic) is connected to the battery pack to provide 5V to the system. 
Ensure the board as well as all of the components share a common ground, and components are not being powered by the Arduino directly. 

A **100 µF capacitor** was added to help regulate power delivery. The capacitor is not shown in the schematic. Connect it across the relevant power and ground rails. 


## Wiring and Cable Routing

Long jumper wires were made to allow flexible routing between the Arduino and the components. These were soldered together due to resource constraints. **Crimped connections are recommended where possible**.

## Final Checks

Before turning on the system:

- Confirm every signal wire is connected to the correct pin
- Verify that the capacitor polarity is correct
- Confirm the battery pack is compatible with the power delivery board and components
- Make sure all grounds are connected together
- (Optional) tape together and label wires for easy troubleshooting
