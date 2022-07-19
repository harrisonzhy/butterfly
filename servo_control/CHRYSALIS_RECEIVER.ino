
/* CHRYSALIS RECEIVER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>
  
#define X A0            // RF24
#define Y A1            // RF24
#define Z 10            // RF24
Servo LEFT_SERVO;
Servo RIGHT_SERVO;

// 2.4g radio
RF24 Radio(5,6); // CE, CSN
const byte address[6] = "31412";

// timekeeping
unsigned long current_time = 0;
unsigned long prev_time = 0;

// analog readings
int xyz_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;
bool z_vals[10];
bool is_on = false;

// analog thresholds
const int LEFT_THRES_ANLG = 400;
const int DOWN_THRES_ANLG = 400;
const int RIGHT_THRES_ANLG = 511 * 2 - LEFT_THRES_ANLG;
const int UP_THRES_ANLG = 511 * 2 - DOWN_THRES_ANLG;
const int DBL_PRESS_TIME = 500;

////////////////////////////////////////////////////////////////////

void setup() {

    Serial.begin(9600);
    LEFT_SERVO.attach(3);
    RIGHT_SERVO.attach(4);
    LEFT_SERVO.write(get_angle(0));
    RIGHT_SERVO.write(get_angle(0));

    Radio.begin();
    Radio.openReadingPipe(0,address);
    Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
    Radio.startListening();

    for (byte c = 0; c < sizeof z_vals; ++c) {
        z_vals[c] = false;
    }

}

byte num_vals = 0;

void loop() { // 7/19/22 find a way to implement timer for button delay timer
    
    const int delay_time = 200;

    if (Radio.available()) {
        char msg_in[32] = "";
        Radio.read(&msg_in, sizeof(msg_in));
        Radio.read(&xyz_val, sizeof(&xyz_val));

        x_val = (int)xyz_val[0];
        y_val = (int)xyz_val[1];
        z_val = (int)xyz_val[2];

        if (num_vals < sizeof(z_vals)) {
            z_vals[num_vals] = xyz_val[2];
            if(is_double_pressed(z_val, millis(), 500)) {
                is_on = !is_on;
            }
            ++num_vals;
        }
        else {
            for (byte c = 0; c < sizeof(z_vals); ++c) {
                z_vals[c] = false;
            }
        }
    }

    //x_val = analogRead(X); // joystick X
    //y_val = analogRead(Y); // joystick Y

    int x_uint8 = map(x_val, 0, 1023, 0, 255); // maps 0-1023 to 0-255
    int y_uint8 = map(y_val, 0, 1023, 0, 255);

    // stationary or forward
    LEFT_SERVO.write(get_angle(30));
    RIGHT_SERVO.write(get_angle(30));
    LEFT_SERVO.write(get_angle(-30));
    RIGHT_SERVO.write(get_angle(-30));

    // turn left
    while (x_val < LEFT_THRES_ANLG && y_val > UP_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            LEFT_SERVO.write(get_angle(20));
            RIGHT_SERVO.write(get_angle(40));
            LEFT_SERVO.write(get_angle(-40));
            RIGHT_SERVO.write(get_angle(-20));
        }
    }

    // turn right
    while (x_val > RIGHT_THRES_ANLG && y_val > UP_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            LEFT_SERVO.write(get_angle(40));
            RIGHT_SERVO.write(get_angle(20));
            LEFT_SERVO.write(get_angle(-20));
            RIGHT_SERVO.write(get_angle(-40));
        }
    }   

    // drop altitude
    while (y_val < DOWN_THRES_ANLG) {
        for (int i = 0; i < 3; i++) {
            if (current_time - prev_time >= delay_time) {
                LEFT_SERVO.write(get_angle(30));
                RIGHT_SERVO.write(get_angle(30));
                prev_time = current_time;
            }
            if (current_time - prev_time >= delay_time) {
                LEFT_SERVO.write(get_angle(-30));
                RIGHT_SERVO.write(get_angle(-30));
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

int get_angle (int displacement) {
    if (displacement < -90) {displacement = -90;}
    else if (displacement > 90) {displacement = 90;}
    
    return 90 + displacement;
}

bool is_double_pressed (int z_val, int prev_time, int time_thres) {
    byte instances = 0;
    if (millis() - prev_time <= time_thres) {
        for (byte c = 0; c < sizeof(z_vals); ++c) {
            if (z_vals[c] == true)  {
                ++instances;
            }
        }
        if (instances > 1) { // double click
            return true;
        }
    }
    return false;
}