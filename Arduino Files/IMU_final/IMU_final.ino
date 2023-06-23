/*
ShakeSense
Team Members: Vibhav Gaka, Andrew Gerchak, Patrick Keenan, Advait Sepuri
Date Completed: 5/24/22
Calculates standard deviations and acceleration in the 3 axes through the IMU
*/
// Necessary includes to run the project

// https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code
// This website contains the necessary information to install libraries to run this code

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

// Construct imu object

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Rename the sensors_event_t to imu_reading for reader clarification

typedef sensors_event_t imu_reading;

// Structure to represent the changes in X, Y, and Z directions

class IMU_Delta {

    private:

        // 3 variables to detect changes in all 3 dimensions

        double x,y,z;

    public:

    // CONSTRUCTORS:

        // Constructor from 3 separate values

        IMU_Delta(const double _x, const double _y, const double _z): 
            x(_x), y(_y), z(_z) {}

        // Constructor from 2 imu_readings

        IMU_Delta(const imu_reading& cur, const imu_reading& prev):
            x(cur.orientation.x - prev.orientation.x),
            y(cur.orientation.y - prev.orientation.y),
            z(cur.orientation.z - prev.orientation.z) {}

        // Default Constructor

        IMU_Delta(): IMU_Delta(0.0, 0.0, 0.0){}

    // GETTERS

        double getX() const {
          return x;
        }

        double getY() const {
          return y;
        }

        double getZ() const {
          return z;
        }

    // "OPERATORS"
    // All of the operators are defined as const, meaning they do not affect the object which they are called on 

        // Usually the << operator would be used, but printing to Serial has different rules

        void printToSerial() const {
            Serial.print("X: ");
            Serial.print(x, 4);
            Serial.print("\tY: ");
            Serial.print(y, 4);
            Serial.print("\tZ: ");
            Serial.print(z, 4);
            Serial.println("");
        }

        // Addition operator for what is essentially a 3D vector, returning new IMU_Delta object

        IMU_Delta operator + (const IMU_Delta& other) const {
            return IMU_Delta(x + other.x, y + other.y, z + other.z);
        }

        // Subtraction operator for what is essentially a 3D vector, returning new IMU_Delta object

        IMU_Delta operator - (const IMU_Delta& other) const {
            return IMU_Delta(x - other.x, y - other.y, z - other.z);
        }

        // Multiplication operator, scaling up x, y, and z values, returning new IMU_Delta object

        IMU_Delta operator * (const double factor) const {
            return IMU_Delta(x * factor, y * factor, z * factor);
        }

        // Division operator, scaling down x, y, and z values, returning new IMU_Delta object

        IMU_Delta operator / (const double factor) const {
            if(factor == 0){
                Serial.println("Cannot divide by a factor of 0");
                exit(0); // Locks the program
            }
            return IMU_Delta(x / factor, y / factor, z / factor);
        }

        // Squares all 3 values, returning new IMU_Delta object

        IMU_Delta square() const {
            return IMU_Delta(x * x, y * y, z * z);
        }
        
        // Takes the square root of all 3 values, returning new IMU_Delta object

        IMU_Delta squareRoot() const {
            return IMU_Delta( sqrt(x), sqrt(y), sqrt(z) );
        }

};

// Setup code

void setup(void) {

    Serial.begin(9600); // Start serial monitor printing

    // Print out that calibration will begin

    Serial.println("Orientation Sensor Test"); 
    Serial.println("Setup will begin in 3 seconds, make sure that the IMU is not moving until serial indicates otherwise");
    
    // Sample code taken from website - https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code

        /* Initialise the sensor */ 
        if(!bno.begin())
        {
            /* There was a problem detecting the BNO055 ... check your connections */
            Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
            while(1);
        }
        
        Serial.println("Sensor Connected!");
        
    bno.setExtCrystalUse(true);

}

/*

General Loop Outline:

    X = number of readings

    1. Take in X imu readings
        a) make sure they are spaced apart a few milliseconds to avoid overloading processor
    2. Calculate X-1 imu deltas
        a) all calculations will be performed on this list 
        b) an average IMU_delta can be performed
        c) the difference between 2 IMU_delta objects over time is the acceleration
    3. Perform various checks on the standard deviations
        a) a high standard deviation indicates the presence of a tremor

*/

constexpr int IMU_READING_NUMS = 10; // called X in outline

constexpr int IMU_DELTA_NUMS = IMU_READING_NUMS - 1;

constexpr int IMU_ACCEL_NUMS = IMU_READING_NUMS - 2;

constexpr int TIME_DELAY = 10; // in milliseconds

// We take in position, velocities, and pseudo-accelerations to determine if there are tremors present

imu_reading readings[IMU_READING_NUMS]; 

IMU_Delta deltas[IMU_DELTA_NUMS];

IMU_Delta accels[IMU_ACCEL_NUMS]; // techinically, this could be a seperate class, but this code would be more difficult to read and repititive

IMU_Delta delta_avg = IMU_Delta(0,0,0);

IMU_Delta accel_avg = IMU_Delta(0,0,0);

IMU_Delta delta_std = IMU_Delta(0,0,0);

IMU_Delta accel_std = IMU_Delta(0,0,0);

// Called at the start of the loop
void getReadings();

// Calculates standard deviations for x,y,z for the deltas and the accels
void getStandardDeviations();

// Constants representing how higb the standard deviation can be before a tremor is detected

constexpr double X_delta_standard_dev_diff = 0.3;

constexpr double Y_delta_standard_dev_diff = 0.1;

constexpr double Z_delta_standard_dev_diff = 0.2;

constexpr double X_accel_standard_dev_diff = 0.2;

constexpr double Y_accel_standard_dev_diff = 0.7;

constexpr double Z_accel_standard_dev_diff = 0.7;

// Prints the average IMU_Delta values for the delta array and the accel array, 
// and prints the standard deviation for both the delta array and the accel array

void printVals();

/*

    MAIN LOOP

*/

void loop(){

    // count is the variable which determines if a tremor is actually occuring

    int count = 0;

    Serial.print("pathy");

    getReadings(); // Read in values

    getStandardDeviations(); // Calculate standard deviations

    //printVals(); // Display values to Serial

    // The following if statements indicate if the standard deviation is too high

    if (delta_std.getX() > X_delta_standard_dev_diff) {
      count++;
    }
    if (delta_std.getY() > Y_delta_standard_dev_diff) {
      count++;
    }
    if (delta_std.getZ() > Z_delta_standard_dev_diff) {
      count++;
    }
    
    if (accel_std.getX() > X_accel_standard_dev_diff) {
      count++;
    }
    if (accel_std.getY() > Y_accel_standard_dev_diff) {
      count++;
    }
    if (accel_std.getZ() > Z_accel_standard_dev_diff) {
      count++;
    }

    // If 4 or more of the 6 standard deviations are too high, a tremor is detected

    if (count >= 4) {
      Serial.println("Tremor Detected");
    }

    // Reset these 4 IMU_Deltas back to the default value, representing no change for the new loop

    delta_avg = IMU_Delta(0,0,0);

    accel_avg = IMU_Delta(0,0,0);
    
    delta_std = IMU_Delta(0,0,0);
    
    accel_std = IMU_Delta(0,0,0);

}

//

constexpr int EVENT_LOOP_DELAY = 10;

void getReadings(){

    for(int i = 0; i < IMU_READING_NUMS; i++){
        bno.getEvent(readings + i); // "Sus raw pointer" math, its perfectly safe in this case
                                    // Usually this value would be stored in an address, but arrays
                                    // can be manipulated in such a way to be treated as memory addresses
        delay(EVENT_LOOP_DELAY); // Wait a number of milliseconds
    }

    for(int i = 0; i < IMU_DELTA_NUMS; i++){
        deltas[i] = IMU_Delta(readings[i+1], readings[i]); // Find the change various imu readings
        delta_avg = delta_avg + deltas[i]; // Sum the values
    }

    delta_avg = delta_avg / (IMU_DELTA_NUMS); // Divide by the number of values to find the average

    for(int i = 0; i < IMU_ACCEL_NUMS; i++){
        accels[i] = deltas[i + 1] - deltas[i]; // Using the - operator to find the change in the IMU_Deltas
        accel_avg = accel_avg + accels[i]; // Sum the values
    }

    accel_avg = accel_avg / (IMU_ACCEL_NUMS); // Divide by the number of values to find the average

}

void getStandardDeviations(){

    for(int i = 0; i < IMU_DELTA_NUMS; i++){
        delta_std = delta_std + (deltas[i] - delta_avg).square(); // Sum the square of the difference between each IMU_Delta and the average IMU_Delta
    }

    delta_std = delta_std / IMU_DELTA_NUMS;

    delta_std = delta_std.squareRoot();

    for(int i = 0; i < IMU_ACCEL_NUMS; i++){
        accel_std = accel_std + (accels[i] - accel_avg).square(); // Sum the square of the difference between each accleration IMU_Delta and the average acceleration IMU_Delta
    }

    accel_std = accel_std/ IMU_ACCEL_NUMS;

    accel_std = accel_std.squareRoot();

}

//Prints Delta and Acceleration values
void printVals(){
    Serial.print("Delta Average:");
    delta_avg.printToSerial();

    Serial.print("Accel Average:");
    accel_avg.printToSerial();

    Serial.print("Delta STD:");
    delta_std.printToSerial();

    Serial.print("Accel STD:");
    accel_std.printToSerial();

    Serial.println();
}
