
/* RECEIVER TEST PROGRAM */

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

int xyz_val[3] = {0,0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;

void setup() {
  Serial.begin(9600);

  Radio.begin();
  Radio.openReadingPipe(0,address);
  Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
  Radio.startListening(); // sets as receiver
  
}

void loop() {

    if (Radio.available()) {
        char msg_in[32] = "";
        Radio.read(&msg_in, sizeof(msg_in));
        Radio.read(&xyz_val, sizeof(xyz_val));
        
        x_val = (int)xyz_val[0];
        y_val = (int)xyz_val[1];
        z_val = (int)xyz_val[2];
    }

    //x_val = analogRead(X);
    //y_val = analogRead(Y);
    
    Serial.println(x_val);
    Serial.println(y_val);
    Serial.println(z_val);
    Serial.println("-------");

}