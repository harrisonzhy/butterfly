
/* SERVO TEST PROGRAM */

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

// joystick analog
int xyz_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;

// servo direction thresholds
const int LEFT_THRES_ANLG = 400;
const int DOWN_THRES_ANLG = 400;
const int RIGHT_THRES_ANLG = 511 * 2 - LEFT_THRES_ANLG;
const int UP_THRES_ANLG = 511 * 2 - DOWN_THRES_ANLG;
 
////////////////////////////////////////////////////////////////////

void setup() {

    Serial.begin(9600);
    LEFT_SERVO.attach(3);
    RIGHT_SERVO.attach(4);

    Radio.begin();
    Radio.openReadingPipe(0,address);
    Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
    Radio.startListening();

}

void loop() {
    
    current_time = millis();
    const int delay_time = 200;


    if (Radio.available()) {
        char msg_in[32] = "";
        Radio.read(&msg_in, sizeof(msg_in));
        Radio.read(&xyz_val, sizeof(&xyz_val));

        x_val = (int)xyz_val[0];
        y_val = (int)xyz_val[1];
        z_val = (int)xyz_val[2];
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
    while (z_val == 1) {
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


int get_angle (int displacement) {
    if (displacement < -90) {displacement = -90;}
    else if (displacement > 90) {displacement = 90;}
    
    return 90 + displacement;
}
