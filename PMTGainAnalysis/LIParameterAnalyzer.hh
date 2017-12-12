/* File LIParameterAnalyzer.hh
 * Header File for LIParameterAnalyzer.cpp 
 * Consolidating parameters to be pass into LIAnalysis 
 *
 * This file was produced under the employ of the United States Government,
 * and is consequently in the PUBLIC DOMAIN, free from all provisions of
 * US Copyright Law (per USC Title 17, Section 105). 
 *
 * Date Created: Sept 18, 2017
 * Date Modified:
 * 
 * Ken Trinh, 2017
 */

#ifndef LIPARAMETERANALYZER_HH
#define LIPARAMETERANALYZER_HH

//Root Include 
#include"TROOT.h"
#include"TObject.h"
#include"TFile.h"
#include"TTree.h"
#include"TH1.h"
#include"TF1.h"
#include"TCanvas.h"
#include"THistPainter.h"
#include"TChain.h"
	
//C++ class
#include<iostream>
#include<cmath>
#include<string>
#include<sstream>
#include<vector>
#include<fstream>
#include<assert.h>
#include<stdlib.h> 

using namespace std;

void LIParameterAnalyzer(string LIA_cfg, string &PCPath, string &H5toROOTPath, string &WFinput, string &PCoutput, string &CHinput, string &FOutput){
  //Extract Parameters from config file and pass it into LIAnalysis tool
  string line = "";

  //Mapping input category
  string PCPName, CVTName, WFFName, PCFName, CHIName, OPTName;
  PCPName = "Path_to_PulseCruncher:";
  CVTName = "Path_to_H5_to_ROOT_Converter:";
  WFFName = "Path_to_Raw_Waveform_File:";
  PCFName = "PulseCrunched_File_Name:";
  CHIName = "Number_of_Channel:";
  OPTName = "Path_to_LIAnalysis_Output:";

  //Size of input map
  int PCSize = PCPName.size();
  int CVSize = CVTName.size();
  int WFSize = WFFName.size();
  int PFSize = PCFName.size();
  int CHSize = CHIName.size();
  int OPSize = OPTName.size();
  
  //Locate input map settings
  size_t foundComment;
  size_t foundPCPName;
  size_t foundCVTName;
  size_t foundWFFName; 
  size_t foundPCFName; 
  size_t foundCHIName; 
  size_t foundOPTName; 

  int lineSize=0;
  //Load config file and begin reading
  ifstream Read_cfg (LIA_cfg);
  while(!Read_cfg.eof()) {
    getline(Read_cfg,line);
    foundComment = line.find("");
    foundPCPName = line.find(PCPName);
    foundCVTName = line.find(CVTName);
    foundWFFName = line.find(WFFName); 
    foundPCFName = line.find(PCFName); 
    foundCHIName = line.find(CHIName); 
    foundOPTName = line.find(OPTName); 

    //ignore commented line
    if(foundComment != string::npos) {
      cout << ".";
    }
    //Find input map category
    //Pulse Cruncher
    
    if(foundPCPName != string::npos) {
      lineSize = line.size();
      PCPath = line.substr(PCSize+1,lineSize);
    }
    //Converter
    if(foundCVTName != string::npos) {
      lineSize = line.size();
      H5toROOTPath = line.substr(CVSize+1,lineSize);
    }
    //Waveform File
    if(foundWFFName != string::npos) {
      lineSize = line.size();
      WFinput = line.substr(WFSize+1,lineSize);
    }
    //PulseCrunched File
    if(foundPCFName != string::npos) {
      lineSize = line.size();
      PCoutput = line.substr(PFSize+1,lineSize);
    }
    //Number of channels  
    if(foundCHIName != string::npos) {
      lineSize = line.size();
      CHinput = line.substr(CHSize+1,lineSize);
    }
    //Output Folder Path
    if(foundOPTName != string::npos) {
      lineSize = line.size();
      FOutput = line.substr(OPSize+1,lineSize);
    }
  }
}

#endif
