
/* TRANSMITTER TEST PROGRAM */

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

int x_val = 0;
int y_val = 0;
int z_val = 0;

////////////////////////////////////////////////////////////////////

struct Control {

    int x_tc;
    int y_tc;
    int z_tc;

};
typedef struct Control Control;
Control ctrl_data;


void setup() {

  pinMode(Z, INPUT_PULLUP);
  Serial.begin(9600);

  Radio.begin();
  Radio.openWritingPipe(address);
  Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
  Radio.stopListening(); // sets as transmitter

}

void loop() {

    // read analog joystick
    ctrl_data.x_tc = (int)analogRead(X);
    ctrl_data.y_tc = (int)analogRead(Y);
    ctrl_data.z_tc = (int)(!digitalRead(Z));

    Serial.println(ctrl_data.x_tc);
    Serial.print("  ");
    Serial.print(ctrl_data.y_tc);
    Serial.print("  ");
    Serial.print(ctrl_data.z_tc);
    Serial.println("-------");
    delay(100);

/*
    Serial.println(ctrl_data.x_tc);
    Serial.print("  ");
    Serial.print(ctrl_data.y_tc);
    Serial.print("  ");
    Serial.print(ctrl_data.z_tc);
    Serial.println("-------");
    delay(100);
*/
    const char msg_in[] = "";
    Radio.write(&msg_in, sizeof(msg_in));
    Radio.write(&ctrl_data, sizeof(Control));

}
