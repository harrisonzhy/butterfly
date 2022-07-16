

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
#define Z 2
Servo LEFT_SERVO;

void setup() {

    Serial.begin(9600);
    LEFT_SERVO.attach(3);
    LEFT_SERVO.write(90);
}

void loop() {
    LEFT_SERVO.write(45);
    delay(500);
    LEFT_SERVO.write(135);
    delay(500);
    Serial.println("-----");
    
}










int get_uint8 (float degrees) {
// maps desired servo rotation to float voltage control (0-255)
    //if (degrees > 90.0) {degrees = 90.0;}
    //else if (degrees < -90.0) {degrees = -90.0;}

    int uint8_val = degrees/180.0 * 256.0 + 127.0;

    if (uint8_val > 255) {uint8_val = 255;}
    else if (uint8_val < 0) {uint8_val = 0;}

    return uint8_val;
}
