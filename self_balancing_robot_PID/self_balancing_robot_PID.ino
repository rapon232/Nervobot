/**
 * \par BME Paris M1 2022-2023, BioFabLab Group 7
 * @file    balancing_robot_test.ino
 * @author  Sirikorn Chalanunt, Dimitar Tahov, Polina Kostina
 * @version V1.0.0
 * @date    24.02.2023
 *
 * \par History:
 * <pre>
 * <Author>             <Time>        <Version>      <Descr>
 * Sirikorn Chalanunt   24.02.2023    1.0.0           PID Controller for wheel balancing robot
 * </pre>
 * This code is designed for MeBlock MegaPi or Arduino Mega 2560 board.
 */

#include "MeMegaPi.h" 
#include <Wire.h>

/////////////DECLARATION/////////////////////////
double currentTime, previousTime, elapsedTime;
double error, lastError, rateError, accumError;
double output, sensorOffset;

// MeBlock Encoded DC Motor Class, 25mm 9V 185rpm
MeMegaPiDCMotor motorL(PORT1B);
MeMegaPiDCMotor motorR(PORT2B);
uint8_t motorSpeed;

// MeBlock Gyro - based on MPU-6050 
MeGyro gyro;
double rollAngle;

//////////////// PID Controller Parameters ////////////////////
float Kp, kd, ki;
// Working version with minor wobbling - kp=35; kd=50; ki=0.25;
// kp=35; kd=10; ki=0.1;
// kp=32; kd=40k ki=0.15;
kp = 32; kd = 40; ki = 0.15;

/////////////// SETUP INITIAL PARAMETERS //////////////////////
void setup()
{
  Serial.begin(115200);
  gyro.begin();
  previousTime = millis();

  sensorOffset = -1.3;

  error = 0;
  lastError = 0;
  rateError = 0;
  accumError = 0;
}

//////////////////////// ROBOT STATES /////////////////////////
void forward()
{
  motorL.run(-motorSpeed); motorR.run(motorSpeed); 
}
void backward()
{
  motorL.run(motorSpeed);  motorR.run(-motorSpeed); 
}
void halt()
{  
  motorR.stop(); motorL.stop();
}

//////////////////////// MAIN PROGRAM //////////////////////////
void loop()
{ 
  currentTime = millis();
  elapsedTime = currentTime - previousTime;

  gyro.update();
  rollAngle = gyro.getAngleX(); // depends on hardware setup.

  ////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                    From Joop Brokking                              x
  //    Control calculations                                                                //                                                                                                                     From Joop Brokking                               x
  ////////////////////////////////////////////////////////////////////////////////////////////       
  // Error term
  error = sensorOffset - rollAngle;                         
  // Derivative term
  rateError  = (error - lastError)/elapsedTime;        
  // Integral term with limit to prevent intergral overflow.
  accumError = max(min(accumError + error * elapsedTime, 512), -512);   
  // PID controller output
  output = Kp * error + Ki * accumError + Kd * rateError;

  // For driving mBlock DC motor 9V 185rpm, input: signed int -255 t0 255
  motorSpeed = min(abs(output), 255);
  
  // Robot's state controller 
  if(output >= 0){
      forward();
  }    
  else{
      backward();
  }

  ///// SAFTY :: TURN OFF MOTORS when too much angle /////////////////////
  if(abs(error) > 30){
    halt();
  }
  ////////////////////////////////////////////////////////////////////////

  //Store current state values
  previousTime = currentTime;
  lastError = error;

  Serial.read();
  //Serial monitor for DEBUGGING
  Serial.print("Roll:");            Serial.print(rollAngle);
  Serial.print("   Error:");        Serial.print(error);
  Serial.print("   D:");            Serial.print(rateError);
  Serial.print("   I:");            Serial.print(accumError);
  Serial.print("   Controller:");   Serial.print(output);
  Serial.print("   motorSpeed:");   Serial.println(motorSpeed);
}





