//Root Class
#include"TROOT.h"
#include"TFile.h"
#include"TObject.h"
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

//Create array of string
void CreateAoS(vector<string> &File, string AddFile) {
  //open .txt file containing a list of file to combine
  ifstream fin;
  fin.open(AddFile.c_str());
  ostringstream AddDir;
  string AddDirName="";
  //Read File and store string to a string of file
  string line;
  int i = 0;
  while (!fin.eof()){
    getline(fin, line);
    AddDir << line;
    AddDirName = AddDir.str();
    File.push_back(AddDirName.c_str());
    AddDir.str("");
    AddDir.clear();
    cout << "processing: " << File[i] << endl;
    i++;    
  }
  //return File;
}

int main(int argc, char** argv) {
  //program fail if arguments are incorrect.
  if(argc < 4) {
    printf("Arguments: <Input_Dir> <Output_Dir> <Number of Channel>");
    return EXIT_FAILURE;
  }
  string Input_Dir = argv[1];     // InputDir
  string Output_Dir = argv[2];    // OutputDir
  string Ch = argv[3];            // Channel


  int iCH = stoi(Ch);
  cout << "How many OCS files are in this series?: ";
  int NoF;
  cin >> NoF;
  vector<string>File;
  string AddFile = "TotalRun.txt";
  CreateAoS(File, AddFile);

  string OpenFile[NoF];

  float A1[NoF];
  float A2[NoF];
  float Voltage[]={9.6, 10.0, 10.8, 11.0, 11.2, 11.5, 12.0, 13.0, 14.0};
  float PIA[iCH];
  TFile *ReadFile[NoF];
  TFile *WriteFile[NoF];
  ostringstream LocateFile;
  for (int i = 0; i < NoF; i++){
    LocateFile << Input_Dir << File[i];
    OpenFile[i] = LocateFile.str();
    //Open PCOutput File and called in DetPulse Tree and gets all of its parameters
    ReadFile[i] = TFile::Open(OpenFile[i].c_str());
    TTree *ReadTree = (TTree*)ReadFile[i]->Get("DetPulse");
    int n = ReadTree->GetEntries();                        //Get the amount of entries
    //Declare a sample with an array corresponding to the amount of values in that array.
    //Set the array to the branch of interested.
    float area=0;
    int detector=0;
    Long64_t event=0;
    ReadTree->SetBranchAddress("a", &area);
    ReadTree->SetBranchAddress("det", &detector);
    ReadTree->SetBranchAddress("evt", &event);

    //Create values to store the historgram array
    float A;

    //declaration of variables to locate spe trigger
    Long64_t evtmap=0, evtprev=0;
    float Amap=0;
    int Dmap=0;
    //Create a new Root file and a new tree for refernces
    //WriteFile[i] = new TFile(File[i].c_str(),"RECREATE");

    //create a flag one of events (Filtering Flag system)
    //THis is map to PulseCruncher output. Change this is PulseCruncher change somehow... always a good a idea to look at the .h5 files first

    vector<long> Pulse;                           //initialize a trigger array  
    for (int p = 0; p < n; p++){                  //loop through each events in the file
      ReadTree->GetEntry(p);             
      evtmap = event;
      Dmap = detector;
      if (Dmap == -100) {                         //run to each set of PMT event to locate a counter, add a counter to Remember this event
        evtprev = evtmap;
        Pulse.push_back(event);
        //cout << Pulse.back() << endl;
      }
      //self-checking module 
      //if (evtmap == evtprev) flag = 1 ;                   //if the next event is equal to the previous event flag = 1.
      if ((evtmap == evtprev) && (Dmap == -100)) continue;  //Get rid of repeating -100 Ch
    }
    /*cout << "break";
    //Create n amount of histogram corresponding to number of PMT to be fill by SPE Pulse Integrated Area
    int range_low=-50, range_high=500;         //I think this could be move to constructor of the class...
    TH1F *Areahis[2];
    for(int l = 0; l < 1; l++) {
      ostringstream areabranch;
      areabranch << "areac" << (l+1000);
      string areabranchname = areabranch.str();
      ostringstream his;
      his << "his" << (l+1000);
      string hisname = his.str();
      cout << hisname;
      Areahis[l] = new TH1F(hisname.c_str(),areabranchname.c_str(),100,range_low,range_high);
    }*/
    float PulseArea[iCH];
    //Begin Filtering Events to its respected PMT
    //cout << "Begin Processing Run Event\n[";
    Dmap = 0;
    evtmap = 0;
    int w = 0;
    int counter =0;
    int traceback = 0;
    int c[iCH];
    for(int j = 0; j < n; j++) {
      ReadTree->GetEntry(j);
      evtmap = event;
      Dmap = detector;
      evtmap = event;
      A = area;
      if (Pulse[w] == evtmap) {
        if (Dmap != -100){
          PulseArea[Dmap] += A;
          c[Dmap] +=1;
        } 
        if (Dmap == 1000) {
          //Areahis[0]->Fill(A);
          PulseArea[309] += A; 
          c[309] += 1;
        }
        if (Dmap == 1001) {
          //Areahis[1]->Fill(A);
          PulseArea[310] += A;
          c[310] += 1;
        }
      }
      else if (Pulse[w] < evtmap){
        w++;
        traceback +=1;
        if (Pulse[w] == evtmap) {
          j = j - traceback;
          traceback = 0;
        }
      } else continue; 
    } 
    //cout << "]" <<endl;
    for(int m = 0; m < iCH; m++) { 
      //cout << m; 
      PIA[m] = PulseArea[m]/100000; 
      //cout << "PMT: " << m << " Area: " << PIA[m] <<endl;
      //cout << "Finished Processing Channel: " << m << " of " << iCH << endl;
    }
    //Areahis[0] -> SaveAs("F1_Plt.pdf");
    //Areahis[1] -> SaveAs("F2_Plt.pdf");   
    //cout << "cow";
    A1[i] = PIA[309];
    A2[i] = PIA[310];
    //cout << A1[i] << " " << A2[i] <<endl;
    LocateFile.str("");
    LocateFile.clear();
    cout << Voltage[i] << " " << A1[i] <<endl;
    cout << Voltage[i] << " " << A2[i] <<endl; 
    //WriteFile[i]->Close();
    ReadFile[i]->Close();
  }

  //Voltage vs AveragePulseArea Monitor 1
  TCanvas* c1 = new TCanvas();
  TGraph* graph = new TGraph(iCH,Voltage,A1);
  graph->SetTitle("Monitor CH 1: Area vs Voltage");
  graph->GetXaxis()->SetTitle("Voltage");
  graph->GetYaxis()->SetTitle("Monitor 1 Area");
  graph->GetXaxis()->SetRangeUser(0.,15.0);
  graph->SetMarkerStyle(8);
  graph->SetDrawOption("P");
  graph->Draw("AP");
  c1->SaveAs("Monitor1(1000).pdf");
  delete(c1);
  delete(graph);

  //Voltage vs AveragePulseArea Monitor 2
  TCanvas* c2 = new TCanvas();
  TGraph* graph2 = new TGraph(iCH,Voltage,A2);
  graph2->SetTitle("Monitor CH 2: Area vs Voltage");
  graph2->GetXaxis()->SetTitle("Voltage");
  graph2->GetYaxis()->SetTitle("Monitor 2 Area");
  graph2->GetXaxis()->SetRangeUser(0.,15.0);
  graph2->SetMarkerStyle(8);
  graph2->SetDrawOption("P");
  graph2->Draw("AP");
  c2->SaveAs("Monitor2(1001).pdf");
  delete(c2);
  delete(graph2);

  return 0;

}
