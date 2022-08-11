
/* CHRYSALIS RECEIVER PROGRAM */

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo LEFT_SERVO;
Servo RIGHT_SERVO;

// 2.4 GHz radiog
RF24 Radio(4,5); // CE, CSN
const byte address[6] = "19211";

// timekeeping
unsigned long time_prev  = 0;
unsigned long time_curr  = 0;

unsigned long press_time = 0;
unsigned long min_time   = 0;
unsigned long max_time   = 0;
int clicks = 0;
const int FLAP_DELAY   = 100;
const int TURN_DELAY   = 140;
const int DROP_DELAY   = 160;

// analog readings
long z_times[2] = {0,0};
int x_val = 0;
int y_val = 0;
int z_val = 0;
bool is_on = false;

// analog thresholds
const int LEFT_THRES_ANLG  = 400;
const int DOWN_THRES_ANLG  = 400;
const int RIGHT_THRES_ANLG = 1023 - LEFT_THRES_ANLG;
const int UP_THRES_ANLG    = 1023 - DOWN_THRES_ANLG;

////////////////////////////////////////////////////////////////////

struct control {

    int x_tc;
    int y_tc;
    int z_tc;

};
typedef struct control Control;
Control ctrl_data;

void setup() {

    delay(5000);
    Serial.begin(9600);
    
    LEFT_SERVO.attach(2);
    RIGHT_SERVO.attach(3);
    LEFT_SERVO.write(get_angle(0));
    RIGHT_SERVO.write(get_angle(0));

    Radio.begin();
    Radio.openReadingPipe(0,address);
    Radio.setPALevel(RF24_PA_MAX); // max transceiving distance
    Radio.startListening(); // sets as receiver

}

void loop() {
    
    time_curr = millis();

    if (Radio.available()) {
        // char msg_in[32] = "";
        // Radio.read(&msg_in, sizeof(msg_in));
        Radio.read(&ctrl_data, sizeof(control));

        x_val = (int)ctrl_data.x_tc;
        y_val = (int)ctrl_data.y_tc;
        z_val = (int)ctrl_data.z_tc;

        //Serial.println(x_val);
        //Serial.println(y_val);
        //Serial.println(z_val);
        
        // checks if butterfly is switched on with thres [10,500] ms
        //swt_signal(z_val, 20, 5000); 
        swt_signal(z_val, 250, 1000);
        Serial.print("Butterfly is on (T/F): ");
        if (is_on) {
            Serial.println("TRUE");
        }
        else {
            Serial.println("FALSE"); 
        }
    }

    if (is_on) {
      Serial.print(x_val);
      Serial.print("    ");
      Serial.print(y_val);
      Serial.print("    ");
      Serial.println(z_val);
        
        int x_uint8 = map(x_val, 0, 1023, 0, 255); // maps 0-1023 to 0-255
        int y_uint8 = map(y_val, 0, 1023, 0, 255);

        // turn left
        if (x_val < LEFT_THRES_ANLG) {
            //for (byte i = 0; i < 3; i++) {
                servo_transmit(LEFT_SERVO, 20, TURN_DELAY, false);
                servo_transmit(RIGHT_SERVO, 50, TURN_DELAY, true);
                servo_transmit(LEFT_SERVO, -20, TURN_DELAY, false);
                servo_transmit(RIGHT_SERVO, -50, TURN_DELAY, true);                
            //}
        }

        // turn right
        else if (x_val > RIGHT_THRES_ANLG) {
            //for (byte i = 0; i < 3; i++) {
                servo_transmit(LEFT_SERVO, 50, TURN_DELAY, false);
                servo_transmit(RIGHT_SERVO, 20, TURN_DELAY, true);
                servo_transmit(LEFT_SERVO, -50, TURN_DELAY, false);
                servo_transmit(RIGHT_SERVO, -20, TURN_DELAY, true);                
            //}
        }   

        // drop altitude
        else if (y_val < DOWN_THRES_ANLG) {
            //for (byte i = 0; i < 3; i++) {
                servo_transmit(LEFT_SERVO, 30, DROP_DELAY, false);
                servo_transmit(RIGHT_SERVO, 30, DROP_DELAY, true);
                servo_transmit(LEFT_SERVO, -30, DROP_DELAY, false);
                servo_transmit(RIGHT_SERVO, -30, DROP_DELAY, true);
            //}
        }

        else {
          // stationary or forward
          servo_transmit(LEFT_SERVO, 30, FLAP_DELAY, false);
          servo_transmit(RIGHT_SERVO, 30, FLAP_DELAY, true);
          servo_transmit(LEFT_SERVO, -30, FLAP_DELAY, false);
          servo_transmit(RIGHT_SERVO, -30, FLAP_DELAY, true);
        }
    } /* if (is_on) {} */
}

int get_angle (int displacement) {
    if (displacement < -90) {
        displacement = -90;
    }
    else if (displacement > 90) {
        displacement = 90;
    }

    return 90 + displacement;
}

void servo_transmit(Servo MOTOR, int angle, int time_delay, bool is_delayed) {
    if (is_delayed == false) {
        time_delay = 0;
    }
    MOTOR.write(get_angle(angle));
    delay(time_delay);
}


void swt_signal (int z, unsigned long min_thres, unsigned long max_thres) {
    if (z == 0) {
        return;
    }
    else if (z == 1) {
    
        if (clicks == 0) {
            press_time = millis();
            min_time = press_time - 10;
            max_time = press_time + max_thres;
            clicks = 1;
        }

        else if (clicks == 1 && millis() < max_time && millis() - min_time > min_thres) {
            Serial.println("pressed twice.");
            is_on = !is_on;
            press_time = 0;
            max_time = 0;
            clicks = 0;      
        }    
    }
    if (clicks == 1 && max_time != 0 && (millis() > max_time || millis() - min_time > min_thres)) {
        Serial.println("pressed once.");
        press_time = 0;
        max_time = 0;
        clicks = 0;
    }
}


////////////////////////////////////////////////////////////////////

/* CURRENTLY UNUSED */

/*
void dbl_press(int z, int min_thres, int max_thres) {
    min_thres = 200;
    max_thres = 4000;

    if (z == 0) {
        return;
    }
    // if z == 1 :
    if (z_times[0] == 0) {
      z_times[0] = millis();
    }
    else if (z_times[1] == 0) {
      z_times[1] = millis();
    }

       // if (z_times[0] != 0 && z_times[1] != 0) {
            Serial.println(z_times[0]);
            Serial.println(z_times[1]);
            Serial.println();
        //}
        
        bool has_zero = false;
        for (byte d = 0; d <= 1; ++d) {
            if (z_times[d] == 0) {
                has_zero = true;
            }
        }
        if (has_zero == false && abs(z_times[1] - z_times[0]) >= min_thres 
                              && abs(z_times[1] - z_times[0]) <= max_thres) {
            is_on = !is_on;
            // clear z_times[2]
            z_times[0] = 0;
            z_times[1] = 0;
        }
         if (abs(z_times[1] - z_times[0]) <= min_thres ||
               abs(z_times[1] - z_times[0]) >= max_thres) {
                Serial.println(abs(z_times[1] - z_times[0]));
          //   clear z_times[2]    
            z_times[0] = 0;
            //z_times[1] = 0;
        }
}

int get_uint8 (float degrees) {
    if (degrees > 90.0) {degrees = 90.0;}
    else if (degrees < -90.0) {degrees = -90.0;}

    int uint8_val = degrees/180.0 * 256.0 + 127.0;

    if (uint8_val > 255) {uint8_val = 255;}
    else if (uint8_val < 0) {uint8_val = 0;}

    return uint8_val;
}

int get_thres_deg (int thres_anlg) {
    if (thres_anlg < 0) {thres_anlg = 0;}
    else if (thres_anlg > 1023) {thres_anlg = 1023;}

    return (int)((511 - thres_anlg) / 511 * 180);
}
*/
