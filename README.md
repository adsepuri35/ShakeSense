# ShakeSense
Wearable sensor that detects muscle contraction to diagnose tremors, a major symptom of Parkinson’s disease. The device uses an Inertial Measurement Unit (IMU) to detect rapid movement and MyoWare muscle sensors to detect muscle contractions in the forearm.

<p float="left">
<img src="images\shake1.jpg" width=450>
<img src="images\shake2.jpg" width=450>
</p>

## Components

| Component  | Quantity |
| ------------- | ------------- |
| Arduino Nano  | 1  |
| Adafruit BNO055 Absolute Orientation Sensor  | 1  |
| Male-to-Male wire  | 6  |
| REX Qualis 400 Point Solderless Breadboard  | 1  |
| MyoWare Muscle Sensor  | 2  |
| Arduino Box (3D-printed)  | 1  |

## Code

The Arduino Code is almost entirely based on a data structure created called IMU_Delta. 

An IMU_Delta object essentially functions as a 3D vector, containing only three values: x,y, and z. Each iteration of the main loop, 10 x,y, and z coordinates are read into an array, 10 ms apart, representing the position of the IMU in 3D space. Then, the array is iterated through, and the difference between one element and the next is recorded in a separate array as an IMU_Delta object. This change in position is treated as velocity in the code.

This array is iterated through again, and the difference between these IMU_Deltas is stored in an “acceleration” array. This represents how much the x,y,and z velocities change between readings. The standard deviation operation is performed on the array of IMU_Deltas and the array of “accelerations”.

The standard deviation of the velocity and acceleration arrays is calculated. These standard deviations contain a value for the x, y, and z, standard deviations. If more than 4 of these standard deviations reach above preset thresholds, then the code outputs that a tremor is detected.

## CAD
