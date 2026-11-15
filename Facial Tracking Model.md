Facial Tracking using FaceOSC and Wekinator 
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
For the Kalaidescope of Emotions FaceOSC is an input that must be used alongside Wekinator. 
This allows for 132 points on the individuals face to be tracked logging the motions for the three possible cases: happy, sad and suprised. FaceOSC captures the facial movements through a webcam and sends the data via OSC to Wekinator. 
Wekinator is an interactive machine learning tool that allows users to train computers to recognise and respond to different typeso of input in real time. 

https://github.com/mywdka/Processing_Wekinator_Arduino? by jxllp
https://github.com/kylemcdonald/ofxFaceTracker/releases by  kylemcdonald 
https://github.com/kylemcdonald/ofxFaceTracker/blob/master/FaceOSC/bin/readme.md

Use the github repository above to help install FaceOSC
Download FaceOSC and Download K-Lite Codec Pack

The FaceOSC software tracks facial input even when your hands are parialy covering your face. However, there is a maximum angle that your are able to turn before the input are no longer mapped to your face. 
Refer to screenshots provided. Face OSC struggles to track when face is turned too much to the left or right. You must look directly at the camera in order for the input to be read correctly. 


How Face OSC communicates to Wekinator 
1. Webcam - Captures facial movements
2. FaceOSC - Tracks facial landmarks and outputs OSC data
3. OSC Communication - Transfers numerical tracking data
4. Wekinator - Trains and applies a machine learning model


# ADD SCREENSHOTS #

Connecting Face OSC to Wekinator: Use the settings below for the Kalidescope of Emotions

Input type: OSC
Input port: 8338
Input message: /raw
Number of inputs: 132
Output: one classifier with three classes

Ensure inputs says /raw and not input/raw/ within seeting on Wekinator 

The three cases used are 1- happy 2- sad and 3- surprised 

Happy- exxagerated smile with teeth. Face OSC tracks lifting of cheek muscles
Sad - Pouty face with furrowed eyeborws. Face OSC also tracks wrinkling as a sad face is made
Surprised - Hands in front of face and open mouth exxagerated facial expressiosn are more easily recognised. 

For the Kalidescope of Emotions live models were used to train the Wekinator ML. Make sure to go to a new environment each time a person is making the facial expressions and ensure that various veiwpoints are recorded in sampling. 
Ensure each individual who is making the facial expressions moves closer and further away from the camera as sample recordings are being taken. Take approximately 500 samples for each facial expression and ensure at least 10 different 
peeople are used. 

Go to File - Input/Output to ensure that both are being recieved. Use the sample proccessing code within the robot arm repository to test functionality. 
Make sure the correct port is being recognised on the processing code as a number from 1-5 will be assigned for each COM (port) on your laptop. 

Useful Git Hub Repository Links 
-----------------------------------------------------------------------------------------------------------------
https://github.com/kylemcdonald/ofxFaceTracker (Face OSC installation)
https://github.com/fiebrink1/wekinator_examples (Using Wekinator)
https://github.com/IDE-GID-Cyberphysical-Systems/de-arm (Example Robot Arm - See processing code)



Notes: During the creation of KoE it was observed that angry was a difficult emotion to track as people made such a large variety of expressions from cross to teeth bared the Wekinator 
got confused between happy sad and angry. The emotion surprised is easier to track becuase of the open mouth movement. 

