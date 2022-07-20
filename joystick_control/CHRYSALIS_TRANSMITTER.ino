
/* CHRYSALIS TRANSMITTER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
#define Z 10

RF24 Radio(5,6); // CE, CSN
const byte address[6] = "37412";

int xyz_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;

void setup() {

  Serial.begin(9600);

  Radio.begin();
  Radio.openWritingPipe(address);
  Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
  Radio.stopListening(); // sets as transmitter

}

void loop() {

    x_val = (int)analogRead(X);
    y_val = (int)analogRead(Y);
    z_val = (int)digitalRead(10);

    xyz_val[0] = x_val;
    xyz_val[1] = y_val;
    xyz_val[2] = z_val;

    const char msg_out[] = "";
    Radio.write(&msg_out, sizeof(msg_out));
    Radio.write(&xyz_val, sizeof(xyz_val));

}