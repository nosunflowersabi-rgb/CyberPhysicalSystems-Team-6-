
Cyber Physical Systems - Imperial College London IDE 1
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Overview
------------------------------------------------
The KoE allows two people to understand each others emotions. As one indiividual makes a facial expression, the other is able to look inside and visually see origami structures moving inside. The KoE has four origami structures: two operated by servos, one by a stepper motor and one which remains stationary. The colours of the LED along with the speed of movement changes with each emotion made. Ideally the KoE would track various emotions but three classes have been made for now which are happy, sad and suprised in respect to cases 1, 2 and 3.


**Project Title: Kalaidescope of Emotions (KoE)**

<img width="800" height="600" alt="image" src="https://github.com/user-attachments/assets/ac950dec-d421-4f3d-885d-4ccbd54060e9" />  

**Figure 1**: Showing KoE in Default Mode 

<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/4de31dca-1d05-4533-a39a-54328518a1f5" />  

**Figure 2**: Showing KoE in Case 1 - **Happy**

<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/2fff4ed0-6f9c-498e-abc5-ada362f936f2" /> 

**Figure 3**: Showing KoE in Case 2 - **Sad**

<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/44d7bd9b-7af5-4db0-b149-3c5ef41735f4" />

**Figure 4**: Showing KoE in Case 3 - **Surprised**



Project Concept
-------------------------------------------------------
This device helps people communicate their feelings when they find it difficult to express them in words. It recognises facial expressions and translates them into origami movements and LED lighting.

We focused on three expressions: smiling, sadness and surprise. We initially considered anger, but the variation in our collected examples made it difficult to train consistently, so we replaced it with surprise.

The origami movements express emotion, while the LEDs add atmosphere. A box focuses the viewer’s attention on the experience, and mirrored acrylic makes the interior appear larger and deeper.

Mechanism Design
---------------------------------------------------------------------------------
We aimed to use a different mechanism for each of the three origami structures.

- Vertical movement: A stepper motor wound in a fishing line to move the paper structure up and down. As the initial movement was too slow, we increased the diameter of the circular component attached to the motor to pull in more line per revolution.

- Clamshell movement: A servo motor, wire and 3D-printed parts allowed both sides to open and close together. The parts were designed to print without supports.

- Gripper movement: We tested a servo-driven gripper, but play in the joints prevented smooth movement, so we excluded it from the final outcome.

Assembly
------------------------------------------------------------------------------------

The box was assembled using interlocking joints. We positioned the laser-cut holes around the mechanisms and wiring routes to keep the wires out of sight. A two-way mirror at the viewing opening was intended to make the viewer’s eyes less visible from inside, keeping the focus on the movement and light.
