#include <SoftwareSerial.h>

#define rxPin 12
#define txPin 13

int EN_A  = 9;      // to 1,2EN of L293D
int EN_B  = 10;     // to 3,4EN of L293D
int IN_A1 = 5;      // to 1A of L293D
int IN_A2 = 6;      // to 2A of L293D
int IN_B1 = 7;      // to 3A of L293D
int IN_B2 = 8;      // to 4A of L293D

int speed = 100;
boolean dir = 1;  // 1 (true) indicates forward, 0 indicates reverse
int left = 0;
int right = 0;


SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin (9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);  
  
  init_motors();
  
  Serial.println("Commencing Transmission");
  
  mySerial.begin(9600);
  mySerial.setTimeout(2000); // Set time out 10 sec
  mySerial.flush();           // Flush MySerial buffer
}

void loop() {
  int lim = 80;
  char rbuf[lim];
  char str[] = "Hi from Arduino";
  
  //Serial.print("Sending String: ");
  //Serial.println(str);
  //mySerial.println(str);
  
  for ( int i = 0; i < 80; i++ )
    rbuf[i] = 0;
    
  if ( mySerial.available() > 0 ) {
    mySerial.readBytesUntil('\n', rbuf, lim);
    mySerial.flush();          // Flush MySerial buffer
    mySerial.print("Got : \"");
    mySerial.print(rbuf);
    mySerial.println("\"");
    Serial.print("Got String: ");
    Serial.println(rbuf);  
  
    String command = rbuf;
    command.trim();
    if ( command.equalsIgnoreCase("forward") ) {
      Serial.println("moving forward"); 
      dir = 1;
      set_left_motor(speed, dir);
      set_right_motor(speed, dir);
    }
    else if ( command.equalsIgnoreCase("left") ) {
      Serial.println("moving left"); 
      set_left_motor(speed, ! dir);       // turn off left motor
      set_right_motor(speed, dir);  // right motor turned on
    }
    else if ( command.equalsIgnoreCase("right") ) {
      Serial.println("moving right");
      set_left_motor(speed, dir);   // left motor turned on
      set_right_motor(speed, ! dir);      // turn off right motor
    }
    else if ( command.equalsIgnoreCase("stop") ) {
      Serial.println("stopping");
      set_left_motor(0, dir);
      set_right_motor(0, dir); 
    }
    else if ( command.equalsIgnoreCase("reverse") ) {
      Serial.println("Reverse");
      dir = 0;
      set_left_motor(speed, dir);
      set_right_motor(speed, dir); 
    }
  } 

}

void init_motors() {
  // set output modes
  pinMode(IN_A1, OUTPUT);
  pinMode(IN_A2, OUTPUT);
  pinMode(IN_B1, OUTPUT);
  pinMode(IN_B2, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
  
  // initialize ports to safely turn off the motors
  analogWrite(EN_A, 0);
  analogWrite(EN_B, 0);
  digitalWrite(IN_A1, 1);
  digitalWrite(IN_A2, 0);
  digitalWrite(IN_B1, 1);
  digitalWrite(IN_B2, 0);
}

void set_left_motor(int speed, boolean dir) {
  analogWrite(EN_A, speed);     // PWM on enable lines
  digitalWrite(IN_A1, dir);
  digitalWrite(IN_A2, ! dir);
}

void set_right_motor(int speed, boolean dir) {
  analogWrite(EN_B, speed);
  digitalWrite(IN_B1, dir);
  digitalWrite(IN_B2, ! dir);
}
