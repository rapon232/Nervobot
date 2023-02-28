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

class Neuron{
  private:

  public:
    double vm, threshold, vReset, vPeak, vMin;
    double C;
    double dt, currentTime, previousTime, lastSpike, refracTime;
    bool isSpike = false;   // AP firing status

    Neuron(){
      vm = -70;         // Resting membrane potential
      vReset = -80;     // Hyperpolarized value from membrane reset
      vPeak = 40;       // Peak voltage during the action potential
      vMin = -100;      // Lower bound of hyperpolarized state
      threshold = -55;  // Threshold voltage at which activate the AP.
      C = 1;            // Membrane capacitance
      isSpike = false;
      lastSpike = millis();
    }

    void LIFmodelUpdate(double i){
      
      ///////////////////// Integrate aNd Fire Model ////////////////////
      currentTime = millis();
      dt = currentTime - previousTime;
      previousTime = currentTime;

      if(isSpike){
        vm = vReset;
        isSpike = false;
        }

      vm = max(vm + dt * (i/C), vMin); // numerical update : Vm

      if(((millis() - lastSpike) > refracTime) && (vm > threshold)){
            vm = vPeak;
            lastSpike = millis();
            isSpike = true;
      }
    }

};

/////////////DECLARATION/////////////////////////
double sensorOffset;
double i, gyroCoef;

float s = 0.1;          // voltage output scaling factor

// MeBlock Gyro - based on MPU-6050 
MeGyro gyro;
double rollAngle;

// Declare neurons
Neuron neuron1, neuron2;

/////////////// SETUP INITIAL PARAMETERS //////////////////////
void setup()
{
  Serial.begin(115200);
  gyro.begin();
  sensorOffset = -1.3;
  gyroCoef = 0.05;  // Injecting current scaling factor
}

//////////////////////// MAIN PROGRAM //////////////////////////
void loop()
{ 
  // Input Current from sensors:
  gyro.update();
  rollAngle = gyro.getAngleX() - sensorOffset; // depends on hardware setup.
  i = rollAngle*gyroCoef;

  ///////////////////// Integrate aNd Fire Model ////////////////////
  neuron1.LIFmodelUpdate(i);
  neuron2.LIFmodelUpdate(-i);
  ///////////////////////////////////////////////////////////////////

  Serial.read();
  //Serial monitor for DEBUGGING
  Serial.print("V1:");        Serial.print(neuron1.vm*s);
  Serial.print("     V2:");        Serial.print(neuron2.vm*s);
  Serial.print("     I:");    Serial.println(i);
  
  //////////////////////////////////////////////////////////////////
}





