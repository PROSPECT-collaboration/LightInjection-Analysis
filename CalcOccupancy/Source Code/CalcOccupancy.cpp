/* Determine Occupancy of large pulses
 * Calc Occupancy File
 * Created on: Dec 19, 2017
 * Last Modified: Dec 19, 2017
 */

/* For Edit Purpuses:
 * Comfile by g++ ../CalcOccupancy CalcOccupancy.cpp
 */
#include "CalcOccupancy.hh"

//Determine Occupancy from large pulse
void CalcOccupancy::CalcOccu(float LargePulse_Area, float snevent) {
  //Expected Area of Single Photo Electron Pulse
  //PMT Gain Scale Calculation
  float GainScale = (Digitizer_Max_Voltage/Digitizer_Bits_Use/Resistance)*(time_per_sample/eC);
  float SPEmean = Gain/GainScale;    //unitless SPEmean from fitting function
  float Expected_SPEArea = (SPEmean*snevent);  //ADCSum
  
  float occu = LargePulse_Area/Expected_SPEArea; 
  cout << "The occupancy value is: " << occu << endl;
}

int main(){
  cout << "What is the Large Pulse Area Value?: ";
  float LargePulse_Area;
  cin >> LargePulse_Area;
  cout << "How many events per PMT channel does small pulse run have?: ";
  float snevent;
  cin >> snevent;
  //Calculate Occupancy
  CalcOccupancy Occu;
  Occu.CalcOccu(LargePulse_Area, snevent);
  return 0;

}
