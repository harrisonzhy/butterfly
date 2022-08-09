
/* CHRYSALIS TRANSMITTER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
int Z = 9;

RF24 Radio(7,8); // CE, CSN
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
    delay(5000);
    
    pinMode(Z, INPUT_PULLUP);
    
    Radio.begin();
    Radio.openWritingPipe(address);
    Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
    Radio.stopListening(); // sets as transmitter

}

void loop() {

    ctrl_data.x_tc = (int)analogRead(X);
    ctrl_data.y_tc = (int)analogRead(Y);
    ctrl_data.z_tc = (int)(!digitalRead(Z));

    Serial.println(ctrl_data.x_tc);
    Serial.println(ctrl_data.y_tc);
    Serial.println(ctrl_data.z_tc);

    //const char msg_out[] = "";
    // Radio.write(&msg_out, sizeof(msg_out));
    Radio.write(&ctrl_data, sizeof(control));

    delay(90);
}
