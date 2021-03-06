/* Plotting Module for Monitor Channel
 * Header file for Monitor Plotting
 * Created on: Jan 29, 2018
 * Last Modified: Jan 29, 2018
 */

#ifndef PLTMONITOR_HH
#define PLTMONITOR_HH
//Root Class
#include"TROOT.h"
#include"TFile.h"
#include"TObject.h"
#include"TString.h"
#include"TTree.h"
#include"TH1.h"
#include"TCanvas.h"
#include"THistPainter.h"
#include"TGraph.h"

//C++ include
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class PLTMonitor{
public:
  //Constructor
  PLTMonitor() {
    cout << "Constructor Called\n" << endl;
    vector<string> File; 
  }
  //Destructor
  ~CalcOccupancy() {
    cout << "Destructor Called\n" << endl;
  }
  void CreateAoS(string, string);
  void CalcIntensity(string, string, int);
  
};
