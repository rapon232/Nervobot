float read = 0;

void setup() {
  // enable serial output
  Serial.begin(9600);
  Serial.println("Arduino Frequency Counter");
  Serial.println();

  pinMode(4, INPUT);
}

void loop() {
  //read = analogRead(0);
  read = digitalRead(5);

  Serial.println(read);

  //delay(100);
}