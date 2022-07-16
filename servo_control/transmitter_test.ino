
#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
#define Z 2

RF24 Radio(5,6); // CE, CSN
const byte address[6] = "31412";

int xy_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;

void setup() {
  Serial.begin(9600);

  Radio.begin();
  Radio.openWritingPipe(address);
  Radio.setPALevel(RF24_PA_MAX); // max distance
  Radio.stopListening(); // sets as transmitter

}

void loop() {

    x_val = (int)analogRead(X);
    y_val = (int)analogRead(Y);
    z_val = (int)digitalRead(2);

    const char msg_in[] = "";
    Radio.write(&msg_in, sizeof(msg_in));
    Radio.write(&xy_val, sizeof(xy_val));

}