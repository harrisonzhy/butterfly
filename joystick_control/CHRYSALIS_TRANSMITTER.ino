
/* CHRYSALIS TRANSMITTER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
int Z = 10;

RF24 Radio(5,6); // CE, CSN
const byte address[6] = "37412";

////////////////////////////////////////////////////////////////////

struct control {

    int x_tc;
    int y_tc;
    int z_tc;

};
typedef struct control Control;
Control ctrl_data;

void setup() {

    Serial.begin(9600);
    pinMode(Z, INPUT_PULLUP);

    Radio.begin();
    Radio.openWritingPipe(address);
    Radio.setDataRate(RF24_250KBPS);
    Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
    Radio.stopListening(); // sets as transmitter

}

void loop() {

    x_tc = (int)analogRead(X);
    y_tc = (int)analogRead(Y);
    z_tc = (int)(!digitalRead(Z));

    //const char msg_out[] = "";
    //Radio.write(&msg_out, sizeof(msg_out));
    Radio.write(&ctrl_data, sizeof(Control));

}