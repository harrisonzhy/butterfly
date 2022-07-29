
/* TRANSMITTER TEST PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1
const int Z = 10;

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

  pinMode(Z, INPUT_PULLUP);
  Serial.begin(19200);
  Serial.flush();
  Serial.println("hi");

  Radio.begin();
  Radio.openWritingPipe(address); // 250 kbps receiving
  Radio.setDataRate(RF24_250KBPS); // 250 kbps receiving
  Radio.setPALevel(RF24_PA_MAX); // test max transceiving distance
  Radio.stopListening(); // sets as transmitter

}

int analog[3] = {0,0,0};

void loop() {

  //  if (Radio.available()) {
        // read analog joystick
        analog[0] = (int)analogRead(X);
        analog[1] = (int)analogRead(Y);
        analog[2] = (int)(!digitalRead(Z));
    
        Serial.println(analog[0]);
        Serial.print("  ");
        Serial.print(analog[1]);
        Serial.print("  ");
        Serial.print(analog[2]);
        Serial.println("-------");
    
        //const char msg_in[] = "";
        //Radio.write(&msg_in, sizeof(msg_in));
        Radio.write(&analog, sizeof(analog));
   // }
    
    delay(100);


}
