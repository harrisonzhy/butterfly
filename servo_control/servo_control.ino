// ctrl+shift+p to select board

/*
1) make joystick x and y objects
2) ues analogRead(obj) method to get values
left and down are 0; right and up are 1023; home position is 511

set analog thresholds for U/D/R/L directions
use servo.write() method to set angles : 0-180, with 90 being 'middle' according to analog values/thresholds
    - may execute turns by offsetting the 'middle' angle in a while loop (while analog </> val_thres) so that one side of robot lifts up
    
auto forward or should a button need to be depressed?
    - (hardware?) issue: turns need to take precedence over forward motion

integrate RF or ESP32 into this : joystick --> analog --> motor
- need two arduino micros - one local controller; one flight board
*/

void setup() {

}

void loop() {
    
}


