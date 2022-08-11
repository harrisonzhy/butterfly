/* MOTOR CALIBRATION PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>

Servo LEFT_SERVO;
Servo RIGHT_SERVO;

void setup() {
    delay(5000);
    Serial.begin(9600);

    LEFT_SERVO.attach(2);
    RIGHT_SERVO.attach(3);
    LEFT_SERVO.write(get_angle(0));
    RIGHT_SERVO.write(get_angle(0));
    Serial.println("bye");

}

void loop() {
    LEFT_SERVO.write(get_angle(-10));
    RIGHT_SERVO.write(get_angle(-10));
    delay(110);
    LEFT_SERVO.write(get_angle(10));
    RIGHT_SERVO.write(get_angle(10));
    delay(110);
}

int get_angle (int displacement) {
    if (displacement < -90) {
        displacement = -90;
    }
    else if (displacement > 90) {
        displacement = 90;
    }

    return 90 + displacement;
}
