#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(7, 8);
const byte address[6] = "00001";
 
void setup() {
    Serial.begin(9600);
    delay(1000);

    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();

    delay(500);
    Serial.println(radio.isChipConnected()); // <-- This line prints "0 
    
}
 
void loop() {
    if (!radio.available()) {
      Serial.println("not available");
      radio.begin();
    }
    else if (radio.available()) {
       Serial.println("online");
       const char text[] = "Hello World";
       radio.write(&text, sizeof(text));
    }
    //Serial.println(radio.isChipConnected());
  
    //radio.printDetails();
    delay(1000);

}
