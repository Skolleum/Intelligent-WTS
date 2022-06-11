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

  Serial.println("Welcome!"); //print a welcome message
  delay(300);

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

    Serial.print("RPM:");
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
