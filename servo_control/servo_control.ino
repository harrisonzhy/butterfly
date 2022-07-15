// ctrl+shift+p to select board
/*
auto forward or should a button need to be depressed?
    - (hardware?) issue: turns need to take precedence over forward motion

integrate RF or ESP32 into this : joystick --> analog --> motor
- need two arduino micros - one local controller; one flight board
*/

/* CHRYSALIS RECEIVER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0            // RF24
#define Y A1            // RF24
#define Z 2;            // RF24
int LEFT_SERVO = 3;
int RIGHT_SERVO = 4;

// 2.4g radio
RF24 Radio(5,6); // CE, CSN
const byte address[6] = "31412";

// joystick analog
int xy_val[2];
int x_val = 0;
int y_val = 0;
bool z_val = false; // 7/15/2022 figure out how to transmit/receive this
char data_in[32] = "";


// analog direction change thresholds
const int LEFT_THRES_ANLG = 400;
const int RIGHT_THRES_ANLG = 511 * 2 - LEFT_THRES_ANLG;
const int DOWN_THRES_ANLG = 400;

const int LEFT_THRES_DEG = (511 - LEFT_THRES_ANLG) / 511 * 180; // needs revision 7/13/2022
const int RIGHT_THRES_DEG = (511 - RIGHT_THRES_ANLG) / 511 * 180;
const int DOWN_THRES_DEG = DOWN_THRES_ANLG / 511 * 180;
 
////////////////////////////////////////////////////////////////////

void setup() {

    Serial.begin(9600); 
    pinMode(3, OUTPUT); // left servo
    pinMode(4, OUTPUT); // right servo

    Radio.begin();
    Radio.openReadingPipe(0,address);
    Radio.setPALevel(RF24_PA_MAX); // max distance
    Radio.startListening();

}

void loop() {
    
    if (Radio.available()) {
        Radio.read(&data_in, sizeof(data_in));
        Radio.read(&xy_val, sizeof(&xy_val));

        y_val  = xy_val[0];
        x_val = xy_val[1];
    }

    x_val = analogRead(X); // joystick X
    y_val = analogRead(Y); // joystick Y
    z_val = digitalRead(2); // boolean Z



    int x_uint8 = map(x_val, 0, 1023, 0, 255); // maps 0-1023 to 0-255
    int y_uint8 = map(y_val, 0, 1023, 0, 255);

    // stationary or forward
    analogWrite(LEFT_SERVO, get_uint8(30));
    analogWrite(RIGHT_SERVO, get_uint8(30));
    analogWrite(LEFT_SERVO, get_uint8(-30));
    analogWrite(RIGHT_SERVO, get_uint8(-30));

    // turn left
    while (x_val < LEFT_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            analogWrite(LEFT_SERVO, get_uint8(20));
            analogWrite(RIGHT_SERVO, get_uint8(40));   
            analogWrite(LEFT_SERVO, get_uint8(-40));   
            analogWrite(RIGHT_SERVO, get_uint8(-20));
        }
    }

    // turn right
    while (x_val > RIGHT_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            analogWrite(LEFT_SERVO, get_uint8(40));
            analogWrite(RIGHT_SERVO, get_uint8(20));   
            analogWrite(LEFT_SERVO, get_uint8(-20));   
            analogWrite(RIGHT_SERVO, get_uint8(-40));
        }
    }   

    // drop altitude
    while (z_val == true) { // button is pressed
        for (int i = 0; i < 3; i++) {
            analogWrite(LEFT_SERVO, get_uint8(30));
            analogWrite(RIGHT_SERVO, get_uint8(30));
            delay(200);   
            analogWrite(LEFT_SERVO, get_uint8(-30));
            analogWrite(RIGHT_SERVO, get_uint8(-30));
            delay(200);
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

bool is_within_thres (int uint8_val, int desired_deg, int deg_thres) {
// sets joystick threshold
    int desired_uint8 = get_uint8(desired_deg);
    int desired_uint8_thres = get_uint8(deg_thres);

    return (uint8_val > desired_uint8 - desired_uint8_thres) 
        && (uint8_val < desired_uint8 + desired_uint8_thres);
}


