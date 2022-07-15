
#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1

int x_val = 0;
int y_val = 0;

void setup() {
  Serial.begin(9600); 
  
}

void loop() {
    x_val = analogRead(X);
    y_val = analogRead(Y);
    Serial.println(x_val);
    Serial.println("\n");
    Serial.println(y_val);
    Serial.println("-------");

}