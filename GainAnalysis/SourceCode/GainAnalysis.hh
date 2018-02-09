/* Header File for LIAnalysis Module
 * Contains PMTGain Analysis for Light Injection Module
 * Contains C++/CERN ROOT includes essentials and all macros
 * Date Created: Aug 28, 2017
 * Date Last Modified: Sep 11, 2017
 */

#ifndef GAINANALYSIS_HH
#define GAINANALYSIS_HH

//Root Include 
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
	
//C++ class
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

class GainAnalysis{
public:
  //Isolate area branch from DetPulse Tree of PulseCruncher Output
  void ExtractArea(string, string, int);
  //For Combine File Run
  void ExtractAreaR2(string, string, TFile*, int);
  //Run Fitting Function through area branch to obtain SPEMean
  void SPEFit(string, int);
  //Calculate PMTGain from obtained SPEMean
  void CalcGain(string);
};

#endif

