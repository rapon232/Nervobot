#include <FreqCount.h>

float read = 0;

void setup() {
  // enable serial output
  Serial.begin(9600);
  Serial.println("Arduino Frequency Counter");
  Serial.println();

  //pinMode(5, INPUT);

  FreqCount.begin(1000);
}

void loop() {
  //read = analogRead(0);
  //read = digitalRead(5);

  //Serial.println(read);

  if (FreqCount.available()) 
  {
    float count = FreqCount.read();
    Serial.println(count/100);
  }

  //unsigned long count = FreqCount.read();
  //Serial.println(count);

  //delay(100);
}