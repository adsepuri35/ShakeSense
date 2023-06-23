/*
ShakeSense
Team Members: Vibhav Gaka, Andrew Gerchak, Patrick Keenan, Advait Sepuri
Date Completed: 5/4/22
IMU set up and test file
*/
// Necessary includes to run the project

// TODO: Write down steps to include libraries

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

// IMU Sensor

Adafruit_BNO055 bno = Adafruit_BNO055(55);

typedef sensors_event_t imu_reading;

// Structure to represent change in X,Y, and Z directions

class IMU_Delta {

    private:

        // 3 variables to detect changes in all 3 dimensions

        double x,y,z;

    public:

    // CONSTRUCTORS:

        // Constructor from 3 separate values

        IMU_Delta(const double _x, const double _y, const double _z): x(_x), y(_y), z(_z) {}

        // Constructor from 2 imu_readings

        IMU_Delta(const imu_reading& cur, const imu_reading& prev):
            x(cur.orientation.x - prev.orientation.x),
            y(cur.orientation.y - prev.orientation.y),
            z(cur.orientation.z - prev.orientation.z) {}

        // Constructor from default

        IMU_Delta(): IMU_Delta(0.0, 0.0, 0.0){}

    // GETTERS

        double getX(){
          return x;
        }

        double getY(){
          return y;
        }

        double getZ(){
          return z;
        }

    // "OPERATORS"

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

        // Addition operator for what is essentially a 3D vector

        IMU_Delta operator + (const IMU_Delta& other) const {
            return IMU_Delta(x + other.x, y + other.y, z + other.z);
        }

        // Subtraction operator for what is essentially a 3D vector

        IMU_Delta operator - (const IMU_Delta& other) const {
            return IMU_Delta(x - other.x, y - other.y, z - other.z);
        }

        // Multiplication operator, scaling up x, y, and z values

        IMU_Delta operator * (const double factor) const {
            return IMU_Delta(x * factor, y * factor, z * factor);
        }

        // Division operator, scaling up x, y, and z values

        IMU_Delta operator / (const double factor) const {
            return IMU_Delta(x / factor, y / factor, z / factor);
        }

        IMU_Delta square() const {
            return IMU_Delta(x * x, y * y, z * z);
        }

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
    
    // Sample code taken from website

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
        c) the difference between IMU_delta is acceleration
    3. Perform various checks on both the imu readings and imu deltas
        a) a high acceleration or a high stddev indicates presence of a tremor

*/

constexpr int IMU_READING_NUMS = 10; // called X in outline

constexpr int IMU_DELTA_NUMS = IMU_READING_NUMS - 1;

constexpr int IMU_ACCEL_NUMS = IMU_READING_NUMS - 2;

constexpr int TIME_DELAY = 10; // in milliseconds

// We take in position, velocities, and accelerations to determine if there are tremors present

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

// struct containing number of "checks" for each axis

struct IMU_checks {
    int x;
    int y;
    int z;

    void printToSerial(){
        Serial.print("X: ");
        Serial.print(x, 4);
        Serial.print("\tY: ");
        Serial.print(y, 4);
        Serial.print("\tZ: ");
        Serial.print(z, 4);
        Serial.println("");
    }

};

// Count number of values for each axis which differ from the standard deviation more than a constant

IMU_checks delta_dev_check();

IMU_checks accel_dev_check();

void printVals();

/*

    MAIN LOOP

*/

constexpr double X_delta_standard_dev_diff = 0.3;

constexpr double Y_delta_standard_dev_diff = 0.1;

constexpr double Z_delta_standard_dev_diff = 0.2;

constexpr double X_accel_standard_dev_diff = 0.2;

constexpr double Y_accel_standard_dev_diff = 0.7;

constexpr double Z_accel_standard_dev_diff = 0.7;

void loop(){

    int count = 0;

    getReadings(); // Read in values

    getStandardDeviations(); // Calculate standard deviations

    //printVals(); // Display values to Serial

    // Now we have to call functions to see how much the values deviate from the average

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

    if (count >= 4) {
      Serial.println("Tremor Detected");
      Serial.println("---------------");
    }

    delta_avg = IMU_Delta(0,0,0);

    accel_avg = IMU_Delta(0,0,0);
    
    delta_std = IMU_Delta(0,0,0);
    
    accel_std = IMU_Delta(0,0,0);

}

constexpr int EVENT_LOOP_DELAY = 10;

// Gets readings from IMU

void getReadings(){

    for(int i = 0; i < IMU_READING_NUMS; i++){
        bno.getEvent(readings + i); // "Sus raw pointer" math, its perfectly safe in this case
        delay(EVENT_LOOP_DELAY);
    }

    //Calculates delta average
    for(int i = 0; i < IMU_DELTA_NUMS; i++){
        deltas[i] = IMU_Delta(readings[i+1], readings[i]);
        delta_avg = delta_avg + deltas[i];
    }

    delta_avg = delta_avg / (IMU_DELTA_NUMS);

    //Calculates average acceleration
    for(int i = 0; i < IMU_ACCEL_NUMS; i++){
        accels[i] = deltas[i + 1] - deltas[i];
        accel_avg = accel_avg + accels[i];
    }

    accel_avg = accel_avg / (IMU_ACCEL_NUMS - 2);

}

//Gets standard deviations of 6 consts
void getStandardDeviations(){

    //Calculates delta standard deviation
    for(int i = 0; i < IMU_DELTA_NUMS; i++){
        delta_std = delta_std + (deltas[i] - delta_avg).square();
    }

    delta_std = delta_std / IMU_DELTA_NUMS;

    delta_std = delta_std.squareRoot();

    //Calculates accel standard deviations
    for(int i = 0; i < IMU_ACCEL_NUMS; i++){
        accel_std = accel_std + (accels[i] - accel_avg).square();
    }

    accel_std = accel_std/ IMU_ACCEL_NUMS;

    accel_std = accel_std.squareRoot();

}

//Calculates changes in delta
IMU_checks delta_dev_check(){

    //initializes count variables
    int x_count = 0, y_count = 0, z_count = 0;

    //Calculates current difference in IMU Deltas
    for(int i = 0; i < IMU_DELTA_NUMS; i++){

        auto curDiff = deltas[i] - delta_avg;
 
        if(fabs(curDiff.getX()) > X_delta_standard_dev_diff){

            x_count++;

        }

        if(fabs(curDiff.getY()) > Y_delta_standard_dev_diff){

            y_count++;

        }

        if(fabs(curDiff.getZ()) > Z_delta_standard_dev_diff){

            z_count++;

        }

    }

    return {x_count, y_count, z_count};

}

//Calculates changes in acceleration
IMU_checks accel_dev_check(){

    //Initializes count variables
    int x_count = 0, y_count = 0, z_count = 0;

    //Calculates current differences in acceleration in x,y,z directions
    for(int i = 0; i < IMU_ACCEL_NUMS; i++){

        auto curDiff = accels[i] - accel_avg;

        if(fabs(curDiff.getX()) > X_accel_standard_dev_diff){

            x_count++;

        }

        if(fabs(curDiff.getY()) > Y_accel_standard_dev_diff){

            y_count++;

        }

        if(fabs(curDiff.getZ()) > Z_accel_standard_dev_diff){

            z_count++;

        }

    }

    return {x_count, y_count, z_count};

}

//Prints averages and standard deviations for test purposes
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
