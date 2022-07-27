
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
  Serial.begin(9600);

  Radio.begin();
  Radio.openWritingPipe(address); // 250 kbps receiving
  Radio.setDataRate(RF24_250KBPS); // 250 kbps receiving
  Radio.setPALevel(RF24_PA_MAX); // test max transceiving distance
  Radio.stopListening(); // sets as transmitter

}

void loop() {

    if (Radio.available()) {
        // read analog joystick
        control.x_tc = (int)analogRead(X);
        control.y_tc = (int)analogRead(Y);
        control.z_tc = (int)(!digitalRead(Z));
    
        Serial.println(control.x_tc);
        Serial.print("  ");
        Serial.print(control.y_tc);
        Serial.print("  ");
        Serial.print(cobtrol.z_tc);
        Serial.println("-------");
    
        //const char msg_in[] = "";
        //Radio.write(&msg_in, sizeof(msg_in));
        Radio.write(&ctrl_data, sizeof(ctrl_data));
    }
    
    delay(200);


}
