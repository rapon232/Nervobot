#include<Wire.h>

const int unitsPerG = 163; // depends on your ADC and the range (1.5g vs 6g)
int axisPin[] = {A0, A1, A2};
int selfTestPin = 8;
int gSelectPin = 9;
int sleepPin = 10;
int startVal[3] = {0};
char result[8]; // for format of the output

void setup()
{
  Serial.begin(9600);

  pinMode(selfTestPin, OUTPUT);
  pinMode(gSelectPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
  digitalWrite(sleepPin, HIGH);
  digitalWrite(selfTestPin, LOW);
  digitalWrite(gSelectPin, LOW);
  getStartVals(); 

  Wire.begin(); 
}

void loop(){

  int angles[3] = {0};
  int raw[3] = {0};
  float gVal[3] = {0.0};

  getRawVals(raw);
  getAngles(angles,raw);
  getGValues(gVal, raw); 

  /*Serial.print("x-Raw:  "); sprintf(result,"%6d", raw[0]); // sprintf for format
  Serial.print(result);
  Serial.print("     ");
  Serial.print("y-Raw:  "); sprintf(result,"%6d", raw[1]); 
  Serial.print(result);
  Serial.print("     ");
  Serial.print("z-Raw:  "); sprintf(result,"%6d", raw[2]); 
  Serial.println(result);
  
  Serial.print("x-Angle:  "); sprintf(result,"%4d", angles[0]);
  Serial.print(result);
  Serial.print("     ");
  Serial.print("y-Angle:  "); sprintf(result,"%4d", angles[1]);
  Serial.print(result);
  Serial.print("     ");
  Serial.print("z-Angle:  "); sprintf(result,"%4d", angles[2]);
  Serial.println(result);

  Serial.print("x-g:      "); 
  Serial.print(gVal[0]); 
  Serial.print("     ");
  Serial.print("y-g:      "); 
  Serial.print(gVal[1]);
  Serial.print("     ");
  Serial.print("z-g:      ");
  Serial.println(gVal[2]);
  Serial.println(); */


  Wire.beginTransmission(9);
  Wire.write(angles[2]);
  Serial.println(angles[2]);
  Wire.endTransmission();
  
  //delay(1000);
}

void getGValues(float gValueArray[], int rawArray[]){
  for(int i=0; i<3; i++){
    int diff = rawArray[i] - startVal[i];
    gValueArray[i] = diff*1.0/unitsPerG;
  }
}

void getRawVals(int rawVals[]){
  for(int i=0; i<3; i++){
    rawVals[i] = readMMA7361(axisPin[i]);
  }
}

void getAngles(int angleArray[], int rawArray[]){
  bool positive = true;
  for(int i=0; i<3; i++){
    int diff = rawArray[i] - startVal[i];
    positive = (diff>=0);
    diff = abs(diff);
    if(diff > unitsPerG){
      diff = unitsPerG;
    }
    angleArray[i] = int((asin(diff*1.0/unitsPerG))*57.296);
    if(positive==false){
      angleArray[i] = -angleArray[i];
    }
  } 
}

int readMMA7361(int pin){
  long sum = 0;
  int result = 0;
  for(int i=0; i<50; i++){  // mean value of 50 measurements
    sum += analogRead(pin);
  }
  result = sum / 50;
  return result;
}

void getStartVals(){
  Serial.println("Your MMA71361 should now be placed flat on the ground, i.e.:");
  Serial.println("x/y-axis = 0 degrees, z-axis = 90 degrees ");
  Serial.print("Wait....");
  delay(2000); // to have enough time to position the MMA7361
  for(int i=0; i<3; i++){
    startVal[i] = readMMA7361(axisPin[i]);
  }
  startVal[2] -= unitsPerG; // Z-axis is at 90° in start position! 
  Serial.println("ready!");
}