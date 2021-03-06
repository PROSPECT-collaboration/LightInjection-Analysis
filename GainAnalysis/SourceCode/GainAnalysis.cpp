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
  //TTree *WriteTree = new TTree("DATA","");
  //WriteTree->SetEntries(n/(number_of_channels-1));      //Ignore Trigger Channel
  
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
      cout << Pulse.back() << endl;
    }
    //self-checking module 
    //if (evtmap == evtprev) flag = 1 ;                   //if the next event is equal to the previous event flag = 1.
    if ((evtmap == evtprev) && (Dmap == -100)) continue;  //Get rid of repeating -100 Ch
  }
  //Create n amount of histogram corresponding to number of PMT to be fill by SPE Pulse Integrated Area
  int range_low=-50, range_high=500;         //I think this could be move to constructor of the class...
  TH1F *Areahis[number_of_channels];
  for(int l = 0; l < number_of_channels; l++) {
    ostringstream areabranch;
    areabranch << "areac" << l;
    string areabranchname = areabranch.str();
    ostringstream his;
    his << "his" << l;
    string hisname = his.str();
    Areahis[l] = new TH1F(hisname.c_str(),areabranchname.c_str(),110,range_low,range_high);
  }
  //Begin Filtering Events to its respected PMT
  cout << "Begin Processing Run Event\n[";
  Dmap = 0;
  evtmap = 0;
  int w = 0;
  int counter =0;
  int traceback = 0;
  for(int j = 0; j < n; j++) {
    ReadTree->GetEntry(j);
    evtmap = event;
    Dmap = detector;
    evtmap = event;
    A = area;
    if ((Dmap != -100) && (Pulse[w] == evtmap)) Areahis[Dmap]->Fill(A);  //If channel isn't -100
    else if (Pulse[w] < evtmap){
      w++;
      traceback +=1;
      if (Pulse[w] == evtmap) {
        j = j - traceback;
        traceback = 0;
      }
    } else continue;
    if (j%1000000==0) cout << "*";  //Sent Wright a Printout so that it doesnt kick you out
  } 
  cout << "]" <<endl;
  for(int m = 0; m < number_of_channels; m++) {     
    Areahis[m]->Write();
    cout << "Finished Processing Channel: " << m << " of " << number_of_channels << endl;
  }
  
  WriteFile->Close();
  ReadFile->Close();
}
//This is for combine Voltage
void GainAnalysis::ExtractAreaR2(string PC_Output, string Output_Dir, TFile* WriteFile, int Ch){
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
  WriteFile = TFile::Open(AreaCHName.c_str(),"UPDATE");
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
      cout << Pulse.back() << endl;
    }
    //self-checking module 
    //if (evtmap == evtprev) flag = 1 ;                   //if the next event is equal to the previous event flag = 1.
    if ((evtmap == evtprev) && (Dmap == -100)) continue;  //Get rid of repeating -100 Ch
  }
  //Create n amount of histogram corresponding to number of PMT to be fill by SPE Pulse Integrated Area
  int range_low=-50, range_high=500;         //I think this could be move to constructor of the class...
  TH1F *Areahis[number_of_channels];
  for(int l = 0; l < number_of_channels; l++) {
    ostringstream areabranch;
    areabranch << "areac" << l;
    string areabranchname = areabranch.str();
    ostringstream his;
    his << "his" << l;
    string hisname = his.str();
    Areahis[l] = new TH1F(hisname.c_str(),areabranchname.c_str(),110,range_low,range_high);
  }
  //Begin Filtering Events to its respected PMT
  cout << "Begin Processing Run Event\n[";
  Dmap = 0;
  evtmap = 0;
  int w = 0;
  int counter =0;
  int traceback = 0;
  for(int j = 0; j < n; j++) {
    ReadTree->GetEntry(j);
    evtmap = event;
    Dmap = detector;
    evtmap = event;
    A = area;
    if ((Dmap != -100) && (Pulse[w] == evtmap)) Areahis[Dmap]->Fill(A);  //If channel isn't -100
    else if (Pulse[w] < evtmap){
      w++;
      traceback +=1;
      if (Pulse[w] == evtmap) {
        j = j - traceback;
        traceback = 0;
      }
    } else continue;
    if (j%1000000==0) cout << "*";  //Sent Wright a Printout so that it doesnt kick you out
  } 
  cout << "]" <<endl;
  for(int m = 0; m < number_of_channels; m++) {    
    if (Areahis[m]->GetEntries() != 0) Areahis[m]->Write();
    cout << "Finished Processing Channel: " << m << " of " << number_of_channels << endl;
  }
  WriteFile->Close();
  ReadFile->Close();
}

//Area Branch Fitting Module
void GainAnalysis::SPEFit(string Output_Dir, int Ch){
  printf("SPE analysis***********************************\n");
  //setting up fitting function for SPE
  int range_low=-50, range_high=500;
  TF1 *Func0 = new TF1("Func0","TMath::Gaus(x,[pedmean],[pedsig],1)");
  TF1 *Func1 = new TF1("Func1","TMath::Gaus(x,[pedmean]+[spemean],sqrt([spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func2 = new TF1("Func2","TMath::Gaus(x,([pedmean]+[spemean])*2,sqrt(2*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func3 = new TF1("Func3","TMath::Gaus(x,([pedmean]+[spemean])*3,sqrt(3*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func4 = new TF1("Func4","TMath::Gaus(x,([pedmean]+[spemean])*4,sqrt(4*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  TF1 *Func5 = new TF1("Func5","TMath::Gaus(x,([pedmean]+[spemean])*5,sqrt(5*[spesig]*[spesig]+[pedsig]*[pedsig]),1)",range_low, range_high);
  //TF1 *Func6 = new TF1("Func6","(x>0)/[decay]*TMath::Exp(-1.0*x/[decay]*(x>0))",range_low, range_high);

  //Recreate a fitting function
  TF1 *spectrumFunc = new TF1("spectrumFunc","([constant]*(TMath::Poisson(0,[occu])*Func0 + TMath::Poisson(1,[occu])*Func1 + TMath::Poisson(2,[occu])*Func2 +TMath::Poisson(3,[occu])*Func3+TMath::Poisson(4,[occu])*Func4+TMath::Poisson(5,[occu])*Func5))",range_low, range_high);

  //Extract pertient values from fitting function and set their limit
  float i_spesig = spectrumFunc->GetParNumber("spesig");
  float i_pedsig = spectrumFunc->GetParNumber("pedsig");
  float i_spemean= spectrumFunc->GetParNumber("spemean");
  float i_occu = spectrumFunc->GetParNumber("occu");
  float i_constant = spectrumFunc->GetParNumber("constant");
  float i_pedmean = spectrumFunc->GetParNumber("pedmean");
  //float i_amp = spectrumFunc->GetParNumber("amp");
  //float i_decay = spectrumFunc->GetParNumber("decay");
  assert(i_spesig>=0);assert(i_pedsig>=0);assert(i_spemean>=0);assert(i_occu>=0);assert(i_pedmean>=0);
  spectrumFunc->SetParameter(i_constant,150000);
  spectrumFunc->SetParLimits(i_spesig,0,50);
  spectrumFunc->SetParLimits(i_spemean,0,150);
  spectrumFunc->SetParLimits(i_pedsig,5,20);
  spectrumFunc->SetParLimits(i_occu,0,5);
  //spectrumFunc->SetParLimits(i_amp,0,1);
  spectrumFunc->SetParLimits(i_pedmean,-10,10);

  //path to oputput
  ostringstream AreaCH, HistoFile;
  AreaCH << Output_Dir << "/PulseArea.root";
  HistoFile << Output_Dir << "/Fitted_PulseArea.root";
  string AreaCHName = AreaCH.str();
  string HistoFileName = HistoFile.str();


  //Read in root File and extracted the area branch and separate it
  TFile *ReadFile = TFile::Open(AreaCHName.c_str(),"READ");
  TFile *WriteFile = new TFile(HistoFileName.c_str(),"RECREATE");

  int nchannels = Ch-1;
  ofstream MOF, FittedOccu, FittedSPEsig, FittedPEDsig, FittedPEDmean;
  MOF.open("GMean.txt");
  FittedOccu.open("FittedOccu.txt");
  FittedSPEsig.open("FittedSPEsig.txt");
  FittedPEDsig.open("FittedPEDsig.txt");
  FittedPEDmean.open("FittedPEDmean.txt");
  ostringstream histo;

  //loop through the extracted area data, store it into a histogram to perform fitting.
  TH1F *call_his ;
  float area_pulse;  
  for(int k = 0; k < nchannels; k++){
    TCanvas* c1 = new TCanvas();
    ostringstream his;
    his << "his" << k;				
    string hisname = his.str();
    
    ReadFile->GetObject(hisname.c_str(),call_his);
    //Set initial parameter for fitting
    spectrumFunc->SetParameter(i_spesig,25);
    spectrumFunc->SetParameter(i_spemean,50);    
    spectrumFunc->SetParameter(i_pedsig,20);
    spectrumFunc->SetParameter(i_occu,1);
    //spectrumFunc->SetParameter(i_amp,0);
    spectrumFunc->SetParameter(i_pedmean,0);

    //setting axis and begin fitting
    call_his->SetAxisRange(range_low,range_high);
    call_his->Fit("spectrumFunc","R");
    call_his->Write();

    //output .pdf of fitted histogram
    histo << Output_Dir << "PMT_" << k << "_.pdf";  
    string histoname = histo.str(); 
    call_his->Draw();
    c1->SaveAs(histoname.c_str());
    histo.str("");
    histo.clear();


    //extract fitting parameters
    TF1 *fit1 = call_his->GetFunction("spectrumFunc");
    double mean_gauss = fit1->GetParameter(i_spemean);
    double sigma_gauss = fit1->GetParameter(i_spesig);
    double occuOut = fit1->GetParameter(i_occu);
    double PEDOut = fit1->GetParameter(i_pedsig);
    double PEDMEANOut = fit1->GetParameter(i_pedmean);
    MOF << mean_gauss << endl;
    FittedOccu << "PMT: " << k << " Occupancy: " << occuOut << endl;
    FittedSPEsig << "PMT: " << k << " SPEsig: " << sigma_gauss << endl;
    FittedPEDsig << "PMT: " << k << " PEDsig: " << PEDOut << endl;
    FittedPEDmean << "PMT: " << k << " PEDmean: " << PEDMEANOut << endl;
  }
  MOF.close();
  FittedOccu.close();
  FittedSPEsig.close();
  FittedPEDsig.close();
  FittedPEDmean.close();
  WriteFile->Write();
  WriteFile->Close();
  ReadFile->Close();

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
  ostringstream StoreMeanOutput, StorePMTGain, StoreFittedOccu, StoreFittedSPEsig, StoreFittedPEDsig, StoreFittedDecay;
  StoreMeanOutput << "mv GMean.txt " << Output_Dir; 
  StorePMTGain << "mv PMTGain.txt " << Output_Dir;
  StoreFittedOccu << "mv FittedOccu.txt " << Output_Dir;
  StoreFittedSPEsig << "mv FittedSPEsig.txt " << Output_Dir;
  StoreFittedPEDsig << "mv FittedPEDsig.txt " << Output_Dir;
  StoreFittedDecay << "mv FittedPEDmean.txt " << Output_Dir;
  string StoreMeanOutputName = StoreMeanOutput.str();
  string StorePMTGainName = StorePMTGain.str();
  string StoreFittedOccuName = StoreFittedOccu.str();
  string StoreFittedSPEsigName = StoreFittedSPEsig.str();
  string StoreFittedPEDsigName = StoreFittedPEDsig.str();
  string StoreFittedDecayName = StoreFittedDecay.str();
  system(StoreMeanOutputName.c_str());
  system(StorePMTGainName.c_str());
  system(StoreFittedOccuName.c_str());
  system(StoreFittedSPEsigName.c_str());
  system(StoreFittedPEDsigName.c_str());
  system(StoreFittedDecayName.c_str());
}
