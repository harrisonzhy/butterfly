

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
#define Z 2
Servo LEFT_SERVO;
Servo RIGHT_SERVO;

void setup() {

    Serial.begin(9600);
    LEFT_SERVO.attach(3);
    RIGHT_SERVO.attach(10);
    LEFT_SERVO.write(get_angle(0));
    RIGHT_SERVO.write(get_angle(0));
}

void loop() {
    //analogWrite(LEFT_SERVO, 255);
    LEFT_SERVO.write(get_angle(30));
    RIGHT_SERVO.write(get_angle(30));
    delay(100);
    
    //analogWrite(LEFT_SERVO, 255);
    LEFT_SERVO.write(get_angle(-30));
    RIGHT_SERVO.write(get_angle(-30));
    delay(100);
    
}

int get_angle (int displacement) {
    if (displacement < -90) {displacement = -90;}
    else if (displacement > 90) {displacement = 90;}
    
    return 90 + displacement;
}
