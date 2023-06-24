# ShakeSense
Wearable sensor that detects muscle contraction to diagnose tremors, a major symptom of Parkinson’s disease. The device uses an Inertial Measurement Unit (IMU) to detect rapid movement and MyoWare muscle sensors to detect muscle contractions in the forearm. The device also uses Google Firebase to provide real-time data to users.

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

## CAD Models
<p float="left">
<img src="images\cad1.jpg" width=450>
<img src="images\cad2.jpg" width=450>
</p>

## IMU Circuit Diagram
<img src="images\circuit_diagram.jpg" width=750>

## Tremor Data

### Non-Tremor Data
<img src="images\tremor_table.jpg" width=550>

### Simulated Tremor Data
<img src="images\tremor_table_2.jpg" width=550>

Delta STD:
- The Delta STD is based of the IMU_Delta data class
  - Stored in this class are 3 values - the change in x, y, and z values from the IMU between two separate readings
- The standard deviation can be applied to the list of x-values, y-values, and z-values from the IMU Deltas
- The higher the standard deviation is, the more likely the user is having a tremor

Accel STD:
- The Accel STD is based of the IMU_Delta data class
  - Stored in this class are 3 values - the change in x, y, and z values between two separate IMU_Delta instances
- The standard deviation can be applied to the list of x-values, y-values, and z-values from the IMU Deltas
- The higher the standard deviation is, the more likely the user is having a tremor

Each standard deviation which reaches above a predetermined threshold increments the “tremor-count” by 1. If the tremor count reaches above 4, then the program outputs to the serial that a tremor is detected. This check is not required for all 6 standard deviations, as if it was, then tremors would be much more difficult and inconsistent to detect. Having only 4 checks achieves a compromise between ease of detection and not being too easy to trigger.

These 6 thresholds were determined experimentally. They needed to work for both tremor cases and non tremor cases. The tester moved his hand around in a fashion mimicking a tremor, if the serial printed out that a tremor was detected, then these values would move onto the next phase of testing, seeing whether or not they detected a tremor when moving the hand in a standard fashion, such as straight forward or up. These 6 final values, present in the code, allow for the user to move their hand in a uniform fashion, lift things up, and write, all without a tremor being detected.




