// SERVO
#include <Servo.h>

// WIND VANE
#include <Wire.h> //This is for i2C

// i2C OLED
#define I2C_ADDRESS 0x3C
#define RST_PIN -1
//I2C pins:
//STM32: SDA: PB7 SCL: PB6
//Arduino: SDA: A4 SCL: A5

//---------------------------------------------------------------------------
//Magnetic sensor things
int magnetStatus = 0; //value of the status register (MD, ML, MH)

int lowbyte; //raw angle 7:0
word highbyte; //raw angle 7:0 and 11:8
int rawAngle; //final raw angle
float degAngle; //raw angle in degrees (360/4096 * [value between 0-4095])

float numberofTurns = 0; //number of turns
float correctedAngle = 0; //tared angle - based on the startup value
float startAngle = 0; //starting angle


// ANEMOMETER
volatile byte revolutions;

float rpmilli;
float speed;

unsigned long timeold = 0 ;
int Sensor = 3; // Declaration of the Hall sensor input pin

// YAW CONTROLLER
Servo bigservo;
int yawAttach = 11;
float nacellePos = 150;
float currentNacellePos;

// PA controller
Servo myservo1;
Servo myservo2;
Servo myservo3;

int pa1Attach = 8;
int pa2Attach = 9;
int pa3Attach = 10;

float pa = 0;

// Rotary Encoder Inputs
#define CLK 2
#define DT 4
#define SW 5

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
unsigned long timeold_rotor = 0 ;
float currentRotorRPM;


void setup()
{
    Serial.begin(115200);

  bigservo.attach(yawAttach);  // attaches the servo on pin 11 to the servo object
  
  myservo1.attach(pa1Attach);  // attaches the servo on pin 10 to the servo object
  myservo2.attach(pa2Attach);  // attaches the servo on pin 9 to the servo object
  myservo3.attach(pa3Attach);  // attaches the servo on pin 8 to the servo object

  //
  Serial.println("Welcome!"); //print a welcome message
  delay(300);

  // Initialize I2C transmission
  Wire.begin(); //start i2C
  Wire.setClock(800000L); //fast clock

  // Initialize wind vane values.
  checkMagnetPresence(); //check the magnet (blocks until magnet is found)

  ReadRawAngle(); //make a reading so the degAngle gets updated
  startAngle = degAngle; //update startAngle with degAngle - for taring

  //------------------------------------------------------------------------------
  Serial.println("AS5600"); //print a welcome message
  delay(3000);

  // attachInterrup to Hall Sensor.
  attachInterrupt(digitalPinToInterrupt(Sensor), rpm_fun, FALLING);

  // Initialize anemometer values.
  revolutions = 0;
  rpmilli = 0;
  timeold = 0;

  currentNacellePos = bigservo.read();
  
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  attachInterrupt(digitalPinToInterrupt(CLK), rotorRpm_fun, CHANGE);

  timeold_rotor = 0;
  currentRotorRPM = 0;

}

void loop() {

  // Anemometer
  if (revolutions >= 1)
  {
    //Update RPM every 20 counts, increase this for better RPM resolution,
    //decrease for faster update

    // calculate the revolutions per milli(second)
    rpmilli = ((float)revolutions) / (millis() - timeold);

    timeold = millis();
    revolutions = 0;
    // WHEELCIRC = 2 * PI * radius (in meters)
    // speed = rpmilli * WHEELCIRC * ???milliseconds per hour??? / ???meters per kilometer???

    // simplify the equation to reduce the number of floating point operations
    // speed = rpmilli * WHEELCIRC * 3600000 / 1000
    // speed = rpmilli * WHEELCIRC * 3600

    speed = rpmilli * 1.288053600;

    // Serial.print("SpeedRPM:");
    // Serial.println(rpmilli * 60000 , DEC);
    Serial.print("Speed:");
    Serial.println(speed, DEC);
    // Serial.println(" kph");
  }
  // Wind Vane
  ReadRawAngle(); //ask the value from the sensor
  correctAngle(); //tare the value

  // Yaw Control
  nacellePos = correctedAngle;
  bigservo.write(nacellePos);              // tell servo to go to position in variable 'pos'

  // PA Control
  
  pa = Serial.readString().toFloat();
  Serial.print("PA:");
  Serial.println(pa);
  
  myservo1.write(pa);              // tell servo to go to position in variable 'pos'
//  myservo2.write(pa);
//  myservo3.write(pa);
//  }
//  
//  Serial.println("PA Control: Success!");
  
  delay(100); //wait a little - adjust it for "better resolution"

  // send Nacelle Position
  currentNacellePos = bigservo.read();
  Serial.print("Nacelle Pos:");
  Serial.println(currentNacellePos);
  

  // send Rotor RPM
  if (counter >= 1)
  {
    //Update RPM every 20 counts, increase this for better RPM resolution,
    //decrease for faster update

    // calculate the revolutions per milli(second)
    currentRotorRPM = ((float)counter) / (millis() - timeold_rotor);

    timeold_rotor = millis();
    counter = 0;

    Serial.print("RPM:");
    Serial.println(currentRotorRPM, DEC);
    
    delay(200);
  }
  
   // send Rotor RPM
  if (counter <= -1)
  {
    //Update RPM every 20 counts, increase this for better RPM resolution,
    //decrease for faster update

    // calculate the revolutions per milli(second)
    currentRotorRPM = ((float)counter) / (millis() - timeold_rotor);

    timeold_rotor = millis();
    counter = 0;

    Serial.print("RotorRPM:");
    Serial.println(currentRotorRPM, DEC);

    delay(200);
  }

  else 
  {
    currentRotorRPM = 0;
    Serial.print("RPM:");
    Serial.println(currentRotorRPM, DEC);

    delay(200);
  }

  // send Active Power output
  
}

void rpm_fun()
{
  revolutions++;
}

void ReadRawAngle()
{
  //7:0 - bits
  Wire.beginTransmission(0x36); //connect to the sensor
  Wire.write(0x0D); //figure 21 - register map: Raw angle (7:0)
  Wire.endTransmission(); //end transmission
  Wire.requestFrom(0x36, 1); //request from the sensor

  while (Wire.available() == 0); //wait until it becomes available
  lowbyte = Wire.read(); //Reading the data after the request

  //11:8 - 4 bits
  Wire.beginTransmission(0x36);
  Wire.write(0x0C); //figure 21 - register map: Raw angle (11:8)
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);

  while (Wire.available() == 0);
  highbyte = Wire.read();

  //4 bits have to be shifted to its proper place as we want to build a 12-bit number
  highbyte = highbyte << 8; //shifting to left
  //What is happening here is the following: The variable is being shifted by 8 bits to the left:
  //Initial value: 00000000|00001111 (word = 16 bits or 2 bytes)
  //Left shifting by eight bits: 00001111|00000000 so, the high byte is filled in

  //Finally, we combine (bitwise OR) the two numbers:
  //High: 00001111|00000000
  //Low:  00000000|00001111
  //      -----------------
  //H|L:  00001111|00001111
  rawAngle = highbyte | lowbyte; //int is 16 bits (as well as the word)

  //We need to calculate the angle:
  //12 bit -> 4096 different levels: 360?? is divided into 4096 equal parts:
  //360/4096 = 0.087890625
  //Multiply the output of the encoder with 0.087890625
  degAngle = rawAngle * 0.087890625;

  Serial.print("Deg angle: ");
  Serial.println(degAngle, 2); //absolute position of the encoder within the 0-360 circle

}

void correctAngle()
{
  //recalculate angle
  correctedAngle = degAngle - startAngle; //this tares the position

  if (correctedAngle < 0) //if the calculated angle is negative, we need to "normalize" it
  {
    correctedAngle = correctedAngle + 360; //correction for negative numbers (i.e. -15 becomes +345)
  }
  else
  {
    //do nothing
  }
  Serial.print("Corrected angle: ");
  Serial.println(correctedAngle, 2); //print the corrected/tared angle
}

void checkMagnetPresence()
{
  //This function runs in the setup() and it locks the MCU until the magnet is not positioned properly
  while ((magnetStatus & 32) != 32) //while the magnet is not adjusted to the proper distance - 32: MD = 1
  {
    magnetStatus = 0; //reset reading
    Wire.beginTransmission(0x36); //connect to the sensor
    Wire.write(0x0B); //figure 21 - register map: Status: MD ML MH
    Wire.endTransmission(); //end transmission
    Wire.requestFrom(0x36, 1); //request from the sensor

    while (Wire.available() == 0); //wait until it becomes available
    magnetStatus = Wire.read(); //Reading the data after the request

    Serial.print("Magnet status: ");
    Serial.println(magnetStatus, BIN); //print it in binary so you can compare it to the table (fig 21)
  }

  //Status register output: 0 0 MD ML MH 0 0 0
  //MH: Too strong magnet - 100111 - DEC: 39
  //ML: Too weak magnet - 10111 - DEC: 23
  //MD: OK magnet - 110111 - DEC: 55

  Serial.println("Magnet found!");
  delay(1000);
}

void rotorRpm_fun() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If the DT state is different than the CLK state then
  // the encoder is rotating CCW so decrement
  if (digitalRead(DT) != currentStateCLK) {
    counter --;
    currentDir = "CCW";
  } else {
    // Encoder is rotating CW so increment
    counter ++;
    currentDir = "CW";
  }

  // Serial.print("Direction: ");
  // Serial.print(currentDir);
  // Serial.print(" | Counter: ");
  // Serial.println(counter);


  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}
