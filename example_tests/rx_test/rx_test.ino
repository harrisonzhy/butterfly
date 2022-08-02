#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(7, 8);
const byte address[6] = "00001";
 
void setup() {
    Serial.begin(9600);
    
    radio.begin();
    while(!radio.available()) {
      Serial.println("not available");
      radio.begin();
    }
    radio.openReadingPipe(0,address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    
    delay(500);
    Serial.println(radio.isChipConnected()); // <-- This line prints "0 
    
}

void loop() {

    if (radio.available()) {
      Serial.println("online");
      char text[32] = {0};
      radio.read(&text, sizeof(text));
      Serial.println(text);
     }
    //Serial.println(radio.isChipConnected());
    
    //radio.printDetails();
    delay(1000);
}
