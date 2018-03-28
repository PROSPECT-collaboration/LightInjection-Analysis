/* Determine Occupancy of large pulses
 * Calc Occupancy File
 * Created on: Dec 19, 2017
 * Last Modified:
 */

// Compile with g++ -o CalcOccupancy CalcOccupancy.cpp `root-config --cflags --libs`


#include "CalcOccupancy.hh"


//Determine Occupancy from large pulse
float CalcOccupancy::CalcOccu(float LargePulse_Area) {
  //Expected Area of Single Photo Electron Pulse
  //PMT Gain Scale Calculation
  float GainScale = 0;
  GainScale = (Digitizer_Max_Voltage/Digitizer_Bits_Use/Resistance)*(time_per_sample/eC);
  float SPEmean = 0;
  SPEmean = Gain/GainScale;    //unitless SPEmean "Average Area of Single Photoelectron

  float occu = 0;
  occu = LargePulse_Area/SPEmean; 
  return occu;
  //cout << "The occupancy value is: " << occu << endl;
}

void TrigFilter(string PC_Output, string Output_Dir, string title, CalcOccupancy Occu) {
  //Open PulseCrunched OCS File and called in DetPulse Tree and gets all of its parameters
  TFile *ReadFile = TFile::Open(PC_Output.c_str());
  TTree *ReadTree = (TTree*)ReadFile->Get("DetPulse");
  int n = ReadTree->GetEntries();                        //Get the amount of entries
  
  //Declare a sample with an array corresponding to the amount of values in that array.
  //Set the array to the branch of interested.
  Float_t area=0;
  Int_t detector=0;
  Long64_t event=0;

  ReadTree->SetBranchAddress("a", &area);
  ReadTree->SetBranchAddress("det", &detector);
  ReadTree->SetBranchAddress("evt", &event);
  

  //Create a new Root file and a new tree to store Pulse Integrated Area Plot per triggered PMT
  ostringstream AreaCH;
  AreaCH << Output_Dir << "/Pulse_Integrated_Area_" << title << ".root";  string AreaCHName = AreaCH.str();  TFile *WriteFile = new TFile(AreaCHName.c_str(),"RECREATE");  TTree *WriteTree = new TTree("Pulse_Integrated_Area","");

  //declaration of variables to locate spe trigger
  Long64_t evtmap=0, evtprev=0;     //event map for trigger filtering
  float Amap = 0;                   //area map for trigger filtering
  int PMTmap = 0;                   //PMT map for trigger filtering
  int flag = 0;                     //abitrary flag value to indicate when trigger is on
  
  //plotting variables 
  int nch = 33;               //set of PMT of interest
  float counter[nch];         //counter of pulse seen by ech PMT after trigger 
  float PMT[nch];             //array of PMT
  float Area_Total[nch];      //total of area of the PMT of interest
  float PLT_PMT[nch];         //array of PMT
  float PLT1_OccuV[nch];      //array of Occupancy per pulse seen for plot mapped to each PMT of interest
  float PLT2_OccuV[nch];      //array of Occupancy per total pulse sent plot mapped to each PMT of interest 
  TH1F *his[nch];             //array of histogram

  //Scan these specific set of nch channels
  for (int j = 0; j < (nch-1); j++) {
    PMT[j] = j+228;  //Do j+50, j+100, etc to shift the ch by 50.
    ostringstream areahis, nhis;
    areahis << "PMT_" << PMT[j] << "_PIA";
    nhis << "his_" << j;    string areahisname = areahis.str();
    string nhisname = nhis.str();    his[j] = new TH1F(nhisname.c_str(), areahisname.c_str(), 250, 0, 5000);
  }
  //EXTRACT Area
  //Create all fillable histogram for each channel
  for(int i = 0; i < n; i++) {
    ReadTree->GetEntry(i);
    //Look for SPE trigger (Filter System)
    evtmap = event;
    PMTmap = detector;
    Amap = area;
    //trigger is turns on. Begin scanning events
    if (PMTmap == -100) {
      flag = 1;
      evtprev = evtmap;
      PMT[52] = detector;
    } else flag = 0;
    if (evtmap == evtprev) flag = 1 ;
    //if the flag is set to 1, begin calculating occupancy per channel dictated by value of PMT_PLOT;
    if (flag == 1) {
      for (int k = 0; k < nch; k++){
        if (PMTmap == PMT[k]) {
          Area_Total[k] += Amap;
          his[k]->Fill(Amap);
          counter[k] += 1.0;
          //cout << PMT[k] << " " << Area_Total[k] << endl;
        } else if (PMTmap != PMT[k]) continue; // ignore 
      } 
    } else if (flag == 0) continue;
  }
  for (int l =0; l < (nch-1); l++) {
    //perform plotting and calculate occupancy
    PLT_PMT[l] = PMT[l];
    PLT1_OccuV[l] = Occu.CalcOccu(Area_Total[l]/counter[l]);
    PLT2_OccuV[l] = Occu.CalcOccu(Area_Total[l]/300000);        
    his[l] -> Write();
    cout << "PMT: " << PMT[l] << " Occu per pulse seen: " <<  PLT1_OccuV[l] << "    Occu per pulse sent: " << PLT2_OccuV[l] << endl;
  }

  //Plot Occupancy value based on amount of Pulse Seen by each PMT
  ostringstream PulseSeen;
  PulseSeen << "Total PulseSeen " << title << " Occupancy";
  string PulseSeenName = PulseSeen.str();
  TCanvas *c1 = new TCanvas("c1","Pulse Seen",600,500);
  TGraph *g1_1 = new TGraph(50,PLT_PMT,PLT1_OccuV);
  g1_1->SetTitle(PulseSeenName.c_str());
  g1_1->GetXaxis()->SetTitle("Ch");
  g1_1->GetYaxis()->SetTitle("OccuV");
  g1_1->GetYaxis()->SetRangeUser(0.0, 5.0);    //change y axis window
  g1_1->SetMarkerStyle(8);
  g1_1->SetDrawOption("P");
  g1_1->Draw("AP");
  c1->Modified();
  c1->Update();
  c1->Write();

  //Plot Occupancy value based on amount of Pulse sent to detector
  ostringstream PulseSent;
  PulseSent << "Total PulseSent " << title << " Occupancy";
  string PulseSentName = PulseSent.str();
  TCanvas *c2 = new TCanvas("c2", "Pulse Sent",600,500);
  TGraph *g2_1 = new TGraph(50,PLT_PMT,PLT2_OccuV);
  g2_1->SetTitle(PulseSentName.c_str());
  g2_1->GetXaxis()->SetTitle("Ch");
  g2_1->GetYaxis()->SetTitle("OccuV");
  g2_1->GetYaxis()->SetRangeUser(0.0, 5.0);      //change Yaxis window
  g2_1->SetMarkerStyle(8);
  g2_1->SetDrawOption("P");
  g2_1->Draw("AP");
  c2->Modified();
  c2->Update();
  c2->Write();

  
/*
  ostringstream name;
  name << Output_Dir << title << ".pdf";
  string filename = name.str();
  c1->SaveAs(filename.c_str());
*/

  //Close the write/read file.
  WriteTree->Write();  WriteFile->Close();
  ReadFile->Close();
}

int main(int argc, char** argv) {
  //program fail if arguments are incorrect.
  if(argc < 3) {
    printf("Arguments: <PulseCruncher_Output> <Output_Dir> <Voltage>");
    return EXIT_FAILURE;
  }

  string InputFile = argv[1];    // Pulse Cruncher Input File
  string Output = argv[2];        // Output Directory
  string Voltage = argv[3];     // laser Voltage for Run

  CalcOccupancy Occu;
  TrigFilter(InputFile, Output, Voltage, Occu);
  
  return 0;

}
