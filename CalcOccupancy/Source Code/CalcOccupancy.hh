/* Determine Occupancy of large pulses
 * Header file for Calc Occupancy
 * Created on: Dec 19, 2017
 * Last Modified:
 */

#ifndef CALCOCCUPANCY_HH
#define CALCOCCUPANCY_HH

#include <iostream>
#include <cmath>
#include <string>
using namespace std;

class CalcOccupancy{
public:
  //Constructor
  CalcOccupancy() {
    cout << "Constructor Called\n" << endl;
    //Declaration of known variables
    Gain = 3e6;                     //Gain
    Digitizer_Max_Voltage = 2.0;    //Voltz
    Digitizer_Bits_Use = pow(2,14); //Bits
    Resistance = 50;                //Ohms
    eC = 1.6e-19;                   //Coulomb
    time_per_sample = 4e-9;         //second
  }
  //Destructor
  ~CalcOccupancy() {
    cout << "Destructor Called\n" << endl;
  }
  float Gain;                     //Gain
  float Digitizer_Max_Voltage;    //Voltz
  float Digitizer_Bits_Use;       //Bits
  float Resistance;               //Ohms
  float eC;                       //Coulomb
  float time_per_sample;          //second

  void CalcOccu(float, float); 
};


#endif
