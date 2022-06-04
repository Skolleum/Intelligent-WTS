#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
Servo myservo4;  // create servo object to control a servo

// twelve servo objects can be created on most boards

int PA1 = 8;
int PA2 = 9;
int PA3 = 10;
int YAW = 11;

float pos1 = 0;    // variable to store the servo position
float pos2 = 0;    // variable to store the servo position
float pos3 = 0;    // variable to store the servo position
float pos4 = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
 
  myservo1.attach(PA1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(PA2);  // attaches the servo on pin 9 to the servo object
  myservo3.attach(PA3);  // attaches the servo on pin 9 to the servo object
  myservo4.attach(YAW);  // attaches the servo on pin 9 to the servo object
  Serial.println("PA Control Success!")

}

void loop() {
   while (!Serial.available());
   pos1 = Serial.readString().toInt();
   myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
   while (!Serial.available());
   pos2 = Serial.readString().toInt();
   myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
   delay(15);                       // waits 15ms for the servo to reach the position   while (!Serial.available());
   while (!Serial.available());
   pos3 = Serial.readString().toInt();
   myservo3.write(pos3);              // tell servo to go to position in variable 'pos'
   delay(15);                       // waits 15ms for the servo to reach the position
   while (!Serial.available());
   pos4 = Serial.readString().toInt();
   myservo4.write(pos4);              // tell servo to go to position in variable 'pos'
   delay(15);    
}
