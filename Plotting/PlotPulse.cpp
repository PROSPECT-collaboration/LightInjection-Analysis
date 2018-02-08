//Root Class
#include"TROOT.h"
#include"TFile.h"
#include"TObject.h"
#include"TString.h"
#include"TTree.h"
#include"TH1.h"
#include"TCanvas.h"
#include"THistPainter.h"
#include"TChain.h"

//C++ class
#include<math.h>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<fstream>
using namespace std;

void PlotPulse(){

    
  //Reset CERN Root and use X number of cores
  gROOT->Reset();
  //TProof::Open("workers=8");
    
  //Open the file and call in the Unpacked Tree
  TFile *ReadFile = TFile::Open("/projects/prospect/converted_data/Unpacked/DryCommissioning/series010/s010_f00004_ts1515865656_Unpacked.root");
  TTree *ReadTree = (TTree*)ReadFile->Get("Waveforms");
    
    
  //Declare a sample with an array corresponding to the amount of values in that array.
  //Set the array to the branch of interested.
  Short_t samps[300];
  UInt_t nsamp;
  ReadTree->SetBranchAddress("samps", &samps);
  ReadTree->SetBranchAddress("nsamp", &nsamp);
  //Declaring the end value for the loop to get all of the data in an array.
  int n = ReadTree->GetEntries();
  
  
  float time[149];
  float samples[149];
 

  //loop through the tree to read file
  for(int i = 0; i < 10; i++) {
    ReadTree->GetEntry(i);
    for(int j = 0; j < nsamp; j++) {
      //Store array to plots
      time[j] = j;
      samples[j] = samps[j];
    }
   
    //x-y plots of gain difference vs. channel
    TCanvas* c1 = new TCanvas();
    TGraph* g1_1 = new TGraph(148,time,samples);
    g1_1->SetTitle("Waveforms");
    g1_1->GetXaxis()->SetTitle("nsamps");
    g1_1->GetYaxis()->SetTitle("samps");
    g1_1->SetMarkerStyle(8);
    g1_1->SetDrawOption("P");
    g1_1->Draw("AP");
    //g1_1->SaveAs("~/LightInjection-Analysis/OCS/series010/-11.0V/");
    //delete(c1);
    //delete(g1_1);


    
  }
  //c1->SaveAs("xyPlot_timediff.pdf");
 



  //Close the write/read file
  ReadFile->Close();
}
