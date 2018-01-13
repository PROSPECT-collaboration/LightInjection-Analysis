/* Header File for LIAnalysis Module
 * Contains PMTGain Analysis for Light Injection Module
 * Contains C++/CERN ROOT includes essentials and all macros
 * Date Created: Aug 28, 2017
 * Date Last Modified: Dec 16, 2017
 */

#include "GainAnalysis.hh"

void GainAnalysis::ExtractArea(string PC_Output, string Output_Dir, int Ch){
  //Open PCOutput File and called in DetPulse Tree and gets all of its parameters
  TFile *ReadFile = TFile::Open(PC_Output.c_str());
  TTree *ReadTree = (TTree*)ReadFile->Get("DetPulse");
  int n = ReadTree->GetEntries();                        //Get the amount of entries
  int number_of_channels = Ch;                           //Store the amount of Channels
 
  //path to output
  ostringstream AreaCH;
  AreaCH << Output_Dir << "/PulseArea.root";
  string AreaCHName = AreaCH.str();
  //Create a new Root file and a new tree for refernces
  TFile *WriteFile = new TFile(AreaCHName.c_str(),"RECREATE");
  TTree *WriteTree = new TTree("DATA","");
  WriteTree->SetEntries(n/(number_of_channels-1));      //Ignore Trigger Channel
  
//Declare a sample with an array corresponding to the amount of values in that array.
  //Set the array to the branch of interested.
  float area=0;
  float baseline=0;
  int detector=0;
  Long64_t event=0;
  ReadTree->SetBranchAddress("a", &area);
  ReadTree->SetBranchAddress("b", &baseline);
  ReadTree->SetBranchAddress("det", &detector);
  ReadTree->SetBranchAddress("evt", &event);

  //Create values to store the historgram array
  float A;

  //declaration of variables to locate spe trigger
  Long64_t evtmap=0, evtprev=0;
  float Amap=0;
  int Dmap=0;
  int flag =0;

  //EXTRACT SPE PARAMETERS
  //Create all fillable histogram for each channel
  for(int l = 0; l < number_of_channels; l++) {
    ostringstream areabranch;
    areabranch << "areac" << l;
    string areabranchname = areabranch.str();
    TBranch *AreaBranch = WriteTree->Branch(areabranchname.c_str(), &A);

    for(int j = 0; j < n; j++) {
      ReadTree->GetEntry(j);

        //Look for SPE trigger (Filter System) (NOTE: Change this part for AD1)
      evtmap = event;
      Dmap = detector;
      if (Dmap == 4) {
        flag = 1;
        evtprev = evtmap;
      }
      else flag = 0;
      if (evtmap == evtprev) flag = 1 ;

      if (flag == 1) {
        if(detector == l) {
          A = area;
          AreaBranch->Fill();
        }
      }
      //Set Abitrary value for the filtered out value
      else if (flag == 0) {
        if(detector == l) {
          A = -100000;
          AreaBranch->Fill();
        }
      }
    }
    cout << (l + 1) << "/" << number_of_channels << endl;
  }
  //Close the write/read file.
  WriteTree->Write();
  WriteFile->Close();
  ReadFile->Close();
}

//Area Branch Fitting Module
void GainAnalysis::SPEFit(string Output_Dir, int Ch){
  printf("SPE analysis***********************************\n");
  //setting up fitting function for SPE
  int range_low=-100, range_high=500;

  TF1 *Func0 = new TF1("Func0","TMath::Gaus(x,0,[pedsig],1)");
  TF1 *Func1 = new TF1("Func1","TMath::Gaus(x,[spemean],sqrt([spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func2 = new TF1("Func2","TMath::Gaus(x,[spemean]*2,sqrt(2*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func3 = new TF1("Func3","TMath::Gaus(x,[spemean]*3,sqrt(3*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func4 = new TF1("Func4","TMath::Gaus(x,[spemean]*4,sqrt(4*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func5 = new TF1("Func5","TMath::Gaus(x,[spemean]*5,sqrt(5*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func6 = new TF1("Func6","(x>0)/[decay]*TMath::Exp(-1.0*x/[decay]*(x>0))",range_low, range_high);

  //Recreate a fitting function
  TF1 *spectrumFunc = new TF1("spectrumFunc","([constant]*((1-[amp])*(TMath::Poisson(0,[occu])*Func0 + TMath::Poisson(1,[occu])*Func1 + TMath::Poisson(2,[occu])*Func2 +TMath::Poisson(3,[occu])*Func3+TMath::Poisson(4,[occu])*Func4+TMath::Poisson(5,[occu])*Func5) + [amp]*Func6))", range_low,range_high);

  //Extract pertient values from fitting function and set their limit
  float i_spesig = spectrumFunc->GetParNumber("spesig");
  float i_pedsig = spectrumFunc->GetParNumber("pedsig");
  float i_spemean= spectrumFunc->GetParNumber("spemean");
  float i_occu = spectrumFunc->GetParNumber("occu");
  float i_constant = spectrumFunc->GetParNumber("constant");
  float i_amp = spectrumFunc->GetParNumber("amp");
  float i_decay = spectrumFunc->GetParNumber("decay");
  assert(i_spesig>=0);assert(i_pedsig>=0);assert(i_spemean>=0);assert(i_occu>=0);
  spectrumFunc->SetParLimits(i_spesig,0,50);
  spectrumFunc->SetParLimits(i_spemean,0,150);
  spectrumFunc->SetParLimits(i_pedsig,5,20);
  spectrumFunc->SetParLimits(i_occu,0,3);
  spectrumFunc->SetParLimits(i_amp,0,1);
  spectrumFunc->SetParLimits(i_decay,0,250);

  //path to oputput
  ostringstream AreaCH, HistoFile;
  AreaCH << Output_Dir << "/PulseArea.root";
  HistoFile << Output_Dir << "/Fitted_PulseArea.root";
  string AreaCHName = AreaCH.str();
  string HistoFileName = HistoFile.str();


  //Read in root File and extracted the area branch and separate it
  TFile *ReadFile = TFile::Open(AreaCHName.c_str(),"READ");
  TTree *ReadTree = (TTree*)ReadFile->Get("DATA");

  TFile *WriteFile = new TFile(HistoFileName.c_str(),"RECREATE");
  TTree *WriteTree = new TTree("FittedArea","");
    
  int n = ReadTree->GetEntries();
  int nchannels = Ch-1;

  ofstream MOF;
  MOF.open("GMean.txt");


  //loop through the extracted area data, store it into a histogram to perform fitting.
  float area_pulse;  
  for(int k = 0; k < nchannels; k++){
    ostringstream areabranch;
    areabranch << "areac" << k;
    string areabranchname = areabranch.str();
    ReadTree->SetBranchAddress(areabranchname.c_str(), &area_pulse);

    //Begin Filling Histogram with respected branch
    TH1F *histogram_pulse = new TH1F("","",800,range_low,range_high); //500,-100,300
    for(int i = 0; i < n; i++){
      ReadTree->GetEntry(i);
      histogram_pulse->Fill(area_pulse);
    }
    //Set initial parameter for fitting
    spectrumFunc->SetParameter(i_spesig,25);
    spectrumFunc->SetParameter(i_spemean,50);    
    spectrumFunc->SetParameter(i_pedsig,20);
    spectrumFunc->SetParameter(i_occu,1);
    spectrumFunc->SetParameter(i_amp,0.5);
    spectrumFunc->SetParameter(i_decay,25);

    //setting axis and begin fitting
    histogram_pulse->SetAxisRange(range_low,range_high);
    histogram_pulse->Fit("spectrumFunc");

    //extract fitting parameters
    TF1 *fit1 = histogram_pulse->GetFunction("spectrumFunc");
    double mean_gauss = fit1->GetParameter(i_spemean);
    double sigma_gauss = fit1->GetParameter(i_spesig);
    MOF << mean_gauss << endl;

  }
  WriteFile->Write();
  WriteFile->Close();
  ReadFile->Close();
  MOF.close();
}

//calculate PMTGain
void GainAnalysis::CalcGain(string Output_Dir){
  //Declaration of known variables
  float Digitizer_Max_Voltage = 2.0;  //Voltz
  float Digitizer_Bits_Use = pow(2,14);
  float Resistance = 50;              //Ohms
  float eC = 1.6e-19;                 //Coulomb
  float time_per_sample = 4e-9;       //second

  //PMT Gain Scale Calculation
  float GainScale = (Digitizer_Max_Voltage/Digitizer_Bits_Use/Resistance)*(time_per_sample/eC);

  //READ in values from txt file
  string line;
  ifstream ReadFile;
  ReadFile.open("GMean.txt");

  //Write out a txt file with PMT Gain and PMT
  ofstream WriteFile;
  WriteFile.open("PMTGain.txt");

  int i = 0;
  float PMTGain;
  while(!ReadFile.eof()) {
    getline(ReadFile, line);
    float mean = strtof(line.c_str(),0);
    PMTGain = mean * GainScale;
    WriteFile << /*"PMT: " << i << " PMTGain: " <<*/ PMTGain << endl;
    i++;
  }
  
  WriteFile.close();
  ReadFile.close();

  //path to oputput to output directory
  ostringstream StoreMeanOutput, StorePMTGain;
  StoreMeanOutput << "mv GMean.txt " << Output_Dir; 
  StorePMTGain << "mv PMTGain.txt " << Output_Dir;
  string StoreMeanOutputName = StoreMeanOutput.str();
  string StorePMTGainName = StorePMTGain.str();

  system(StoreMeanOutputName.c_str());
  system(StorePMTGainName.c_str());
}
