// ctrl+shift+p to select board

/*
1) make joystick x and y objects
2) ues analogRead(obj) method to get values
left and down are 0; right and up are 1023; home position is 511

set analog thresholds for U/D/R/L directions
use servo.write() method to set angles : 0-180, with 90 being 'middle' according to analog values/thresholds
    - may execute turns by offsetting the 'middle' angle in a while loop (while analog </> val_thres) so that one side of robot lifts up
    
auto forward or should a button need to be depressed?
    - (hardware?) issue: turns need to take precedence over forward motion

integrate RF or ESP32 into this : joystick --> analog --> motor
- need two arduino micros - one local controller; one flight board
*/
#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
int LEFT_SERVO = 2;
int RIGHT_SERVO = 3;

int x_val = 0;
int y_val = 0;

//////////////////////////////////

void setup() {
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);

}

void loop() {
    
    x_val = analogRead(X);
    y_val = analogRead(Y);

    // 7/13/2022: need to figure out analog thresholds for servo direction changes

    // stationary or forward
    while ((is_within_thres(x_val, 0, 15) && is_within_thres(y_val, 0, 15)) || x_val > 512) {
        int x_uint8 = map(x_val, 0, 1023, 0, 255); // maps 0-1023 to 0-255
        int y_uint8 = map(y_val, 0, 1023, 0, 255);

        analogWrite(LEFT_SERVO, get_uint8(30));
        analogWrite(RIGHT_SERVO, get_uint8(30));
        analogWrite(LEFT_SERVO, get_uint8(-30));
        analogWrite(RIGHT_SERVO, get_uint8(-30));

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

    int desired_uint8 = get_uint8(desired_deg);
    int desired_uint8_thres = get_uint8(deg_thres);

    return (uint8_val > desired_uint8 - desired_uint8_thres) && (uint8_val < desired_uint8 + desired_uint8_thres);
}


