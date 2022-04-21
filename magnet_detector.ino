int Led = 2 ;// Declaration of the LED output pin
int Sensor = 8; // Declaration of the sensor input pin
int val; // Temporary Variable

void setup ()
{
 pinMode (Led, OUTPUT) ; // Initialization of output pin
 pinMode (Sensor, INPUT) ; // Sensor pin initialization
 digitalWrite(Sensor, HIGH); // Activation of internal pull-up resistor
}

void loop ()
{
 val = digitalRead (Sensor) ; // The current signal at the sensor is read out

 if (val == HIGH) // If a signal could be detected, the LED is switched on.
 {
 digitalWrite (Led, LOW);
 Serial.println("detect");
 }
 else
 {
 digitalWrite (Led, HIGH);
 }
}
