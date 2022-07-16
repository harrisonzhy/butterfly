
#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0            // RF24
#define Y A1            // RF24
#define Z 2             // RF24
int LEFT_SERVO = 3;
int RIGHT_SERVO = 4;

// 2.4g radio
RF24 Radio(5,6); // CE, CSN
const byte address[6] = "31412";

// timekeeping
unsigned long current_time = 0;
unsigned long prev_time = 0;

// joystick analog
int xyz_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;

// servo direction thresholds
    const int LEFT_THRES_ANLG = 400;
    const int RIGHT_THRES_ANLG = 511 * 2 - LEFT_THRES_ANLG;
    const int DOWN_THRES_ANLG = 400;
    const int UP_THRES_ANLG = 511 * 2 - LEFT_THRES_ANLG;

/////////////////////////////////////////////////////

void setup() {

    Serial.begin(9600); 
    pinMode(3, OUTPUT); // left servo
    pinMode(4, OUTPUT); // right servo
}

void loop() {
    
    current_time = millis();
    const int delay_time = 200;
    
    char data_in[32] = "";

    // stationary or forward
    analogWrite(LEFT_SERVO, get_uint8(30));
    analogWrite(RIGHT_SERVO, get_uint8(30));
    analogWrite(LEFT_SERVO, get_uint8(-30));
    analogWrite(RIGHT_SERVO, get_uint8(-30));

    // turn left
    while (x_val < LEFT_THRES_ANLG && y_val > UP_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            analogWrite(LEFT_SERVO, get_uint8(20));
            analogWrite(RIGHT_SERVO, get_uint8(40));   
            analogWrite(LEFT_SERVO, get_uint8(-40));   
            analogWrite(RIGHT_SERVO, get_uint8(-20));
        }
    }

    // turn right
    while (x_val > RIGHT_THRES_ANLG && y_val > UP_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            analogWrite(LEFT_SERVO, get_uint8(40));
            analogWrite(RIGHT_SERVO, get_uint8(20));   
            analogWrite(LEFT_SERVO, get_uint8(-20));   
            analogWrite(RIGHT_SERVO, get_uint8(-40));
        }
    }   

    // drop altitude
    while (z_val == 1) {
        for (int i = 0; i < 3; i++) {
            if (current_time - prev_time >= delay_time) {
                analogWrite(LEFT_SERVO, get_uint8(30));
                analogWrite(RIGHT_SERVO, get_uint8(30));
                prev_time = current_time;
            }
            if (current_time - prev_time >= delay_time) {
                analogWrite(LEFT_SERVO, get_uint8(-30));
                analogWrite(RIGHT_SERVO, get_uint8(-30));
                prev_time = current_time;
            }
        }
    }
}

int get_uint8 (float degrees) {
// maps desired servo rotation to float voltage control (0-255)
    if (degrees > 90.0) {degrees = 90.0;}
    else if (degrees < -90.0) {degrees = -90.0;}

    int uint8_val = degrees/180.0 * 256.0 + 127.0;

    if (uint8_val > 255) {uint8_val = 255;}
    else if (uint8_val < 0) {uint8_val = 0;}

    return uint8_val;
}

int get_thres_deg (int thres_anlg) {
    if (thres_anlg < 0) {thres_anlg = 0;}
    else if (thres_anlg > 1023) {thres_anlg = 1023;}

    return (int)((511 - thres_anlg) / 511 * 180);
}