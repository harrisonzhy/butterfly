
#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

#define X A0
#define Y A1

RF24 Radio(5,6); // CE, CSN
const byte address[6] = "31412";

int xy_val[2];
int x_val = 0;
int y_val = 0;
bool press_val = false;

void setup() {
  Serial.begin(9600);

  Radio.begin();
  Radio.openReadingPipe(0,address);
  Radio.setPALevel(RF24_PA_MAX); // max distance
  Radio.startListening();
  
}

void loop() {

    if (Radio.available()) {
        char data_in[32] = "";
        Radio.read(&data_in, sizeof(data_in));
        Radio.read(&x_val, sizeof(&x_val));

        y_val  = xy_val[0];
        x_val = xy_val[1];
    }

    //x_val = analogRead(X);
    //y_val = analogRead(Y);

    
    Serial.println(x_val);
    Serial.println("\n");
    Serial.println(y_val);
    Serial.println("-------");

}