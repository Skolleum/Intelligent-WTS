float pa; 

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("Welcome!"); //print a welcome message
  delay(300);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available()){
  pa = Serial.readString().toFloat();
  Serial.print("PA:");
  Serial.println(pa);
}
}
