/* Determine Occupancy of large pulses
 * Calc Occupancy File
 * Created on: Dec 19, 2017
 * Last Modified:
 */


#include "CalcOccupancy.hh"

//Determine Occupancy from large pulse
void CalcOccupancy::CalcOccu(float LargePulse_Area) {
  //Expected Area of Single Photo Electron Pulse
  //PMT Gain Scale Calculation
  float GainScale = (Digitizer_Max_Voltage/Digitizer_Bits_Use/Resistance)*(time_per_sample/eC);
  float SPEmean = Gain/GainScale;    //unitless SPEmean "Average Area of Single Photoelectron"
  
  float occu = LargePulse_Area/SPEmean; 
  cout << "The occupancy value is: " << occu << endl;
}

int main(){
  cout << "What is the Large Pulse Area Value?: ";
  float LargePulse_Area;
  cin >> LargePulse_Area;
  CalcOccupancy Occu;
  Occu.CalcOccu(LargePulse_Area);
  return 0;

}
