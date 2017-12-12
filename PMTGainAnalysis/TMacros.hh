/* Header File for LIAnalysis Module
 * Contains Timing Analysis for Light Injection Module
 * Contains C++/CERN ROOT includes essentials and all macros
 * Date Created: Sep 11, 2017
 * Date Last Modified: Sep 11, 2017
 */

#ifndef TMacros_H
#define TMacros_H

#include"LIParameterAnalyzer.hh"

//Check integral for 20% of the pulse integral value.
double CHECKINTEGRAL(double integral, short samps[], int lower_integration_limit, int upper_integration_limit, double baseline) {
  double check_integral = 0;
  for(int i = lower_integration_limit; i <= upper_integration_limit; i++) {
    double x = 0;
    while(x < 1) {
      check_integral += (baseline - static_cast<double>(samps[i]))*0.001;
      x += 0.001;
      if(check_integral > 0.20*integral) return (static_cast<double>(i) + x);
    }
  }
  return 0;
}

//Begin Timing Analysis
void AnalyzeTiming(string WFinput, string LIOutputPathName) {
  //Open the unpacked file and read the waveform tree
  TFile* ReadFile1 = TFile::Open(WFinput.c_str());            //Open Unpacked .root file
  TTree* ReadTree1 = (TTree*)ReadFile1->Get("Waveforms");     //Read the Waveform Tree
  int n = ReadTree1->GetEntries();
  int number_of_samples, number_of_channels;
  

  //Declare variables to store values of Waveforms.  
  short samps[300];
  int chan;
  ReadTree1->SetBranchAddress("samps", &samps);
  ReadTree1->SetBranchAddress("chan", &chan);

  int channel = -1;
  int prev_channel;
  for(int o = 0; o <= n; o++) {
    ReadTree1->GetEntry(o);
    prev_channel = channel;
    channel = chan;
    if(channel > prev_channel) number_of_channels = channel + 1;
    else break;
  }
  cout << "Number of channels = " << number_of_channels << endl;


  //Declare limit of integrations 
  int lower_baseline_limit;
  int upper_baseline_limit;
  int lower_integration_limit;
  int upper_integration_limit;


  ostringstream TimeOutput;
  TimeOutput << LIOutputPathName << "/Timing.root";
  string TimeOutputName = TimeOutput.str();
    
  //Create a new Root file and a new tree for references
  TFile* WriteFile1 = new TFile(TimeOutputName.c_str(),"RECREATE");
  TTree* WriteTree1 = new TTree("DATA","");
  WriteTree1->SetEntries(n/number_of_channels);

  double time, integral;
  for(int m = 0; m < number_of_channels; m++) {
    ostringstream timebranch;
    timebranch << "timec" << m;
    string timebranchname = timebranch.str();
        
    TBranch* TimeBranch = WriteTree1->Branch(timebranchname.c_str(), &time);
    for(int i = 0; i < n; i++) {
      ReadTree1->GetEntry(i);
      integral = 0;
      int counter = 0;
      int baseline = 0;
      int min = 100000;
      int peak_loc = 118;

      if(chan == m) {
        min = samps[peak_loc];

        lower_baseline_limit = peak_loc - 50;
        upper_baseline_limit = peak_loc - 10;
        lower_integration_limit = peak_loc - 8;
        upper_integration_limit = peak_loc + 8;
                
        //calculate baseline
        for(int j = lower_baseline_limit; j <= upper_baseline_limit; j++) {
          baseline += samps[j];
          counter++;
        }        
        baseline = baseline/static_cast<double>(counter);            //output baseline value
                
        //calculate integral
        for(int j = lower_integration_limit; j <= upper_integration_limit; j++) integral += (baseline - samps[j]);
        time = CHECKINTEGRAL(integral, samps, lower_integration_limit, upper_integration_limit, baseline)*4;
        if((lower_integration_limit < 0) || (upper_integration_limit > number_of_samples) || (min > (baseline - 30))) time = 0;
        TimeBranch->Fill();
      }
    }
    cout << "Processing data for channel " << (m + 1) << endl;
  }
    
  //Close the write/read file.
  WriteTree1->Write();
  WriteFile1->Close();
  ReadFile1->Close();

  //Declare variable for storing output values
  int upper_limit = number_of_samples*4;
  int lower_limit = 20;
  double total_reference_time;
  double PMT_reference_time;
  double cellgroup_reference_time;
  int reference_channel = 0;

  TFile* ReadFile2 = TFile::Open(TimeOutputName.c_str());
  TTree* ReadTree2 = (TTree*)ReadFile2->Get("DATA");

  //readout files 
  ofstream fout1;
  fout1.open("TMean.txt");
  ofstream fout2;
  fout2.open("TSigma.txt");
  ofstream fout3;
  fout3.open("TMean_Difference_Total.txt");
  ofstream fout4;
  fout4.open("TMean_Difference_PMT.txt");
  ofstream fout5;
  fout5.open("TMean_Difference_Cell_Group.txt");

  int m = ReadTree2->GetEntries();
  int pulse_maxbin;
  double pulse_maxvalue, scale, mean;
  double time_pulse;

  for(int k = 0; k < number_of_channels; k++) {
    ostringstream timebranch;
    timebranch << "timec" << k;
    string timebranchname = timebranch.str();
    TH1F* histogram_pulse = new TH1F("","",10000,lower_limit,upper_limit); //500,-100,300
    ReadTree2->SetBranchAddress(timebranchname.c_str(), &time_pulse);

    for(int i = 0; i < m; i++) {
      ReadTree2->GetEntry(i);
      histogram_pulse->Fill(time_pulse);
    }

    double mean = histogram_pulse->GetMean(1);
    double sigma = histogram_pulse->GetStdDev(1);

    if(k == reference_channel) total_reference_time = mean;
    if(k % 2 == 0) PMT_reference_time = mean;
    if(k % 8 == 0) cellgroup_reference_time = mean;

    fout1 << "PMTNumber: " << k << " PMTTimingMean: " << mean << endl;
    fout2 << "PMTNumber: " << k << " PMTTimingSigma: " << sigma << endl;
    fout3 << "PMTNumber: " << k << " PMTTimingReferenceTotal: " << mean - total_reference_time << endl;
    fout4 << "PMTNumber: " << k << " PMTTimingReferencePMT: " << mean - PMT_reference_time << endl;
    fout5 << "PMTNumber: " << k << " PMTTimingReferenceCellGroup: " << mean - cellgroup_reference_time << endl;
  }

  fout1.close();
  fout2.close();
  fout3.close();
  fout4.close();
  fout5.close();
  ReadFile2->Close();


  //path to oputput
  ostringstream StoreTimeMean, StoreTimeSigma, StoreTimeRefTotal, StoreTimeRefPMT, StoreTimRefCellGroup;
  StoreTimeMean << "mv TMean.txt " << LIOutputPathName; 
  StoreTimeSigma << "mv TSigma.txt " << LIOutputPathName;
  StoreTimeRefTotal << "mv TMean_Difference_Total.txt " << LIOutputPathName; 
  StoreTimeRefPMT << "mv TMean_Difference_PMT.txt " << LIOutputPathName;
  StoreTimRefCellGroup << "mv TMean_Difference_Cell_Group.txt " << LIOutputPathName; 
  string StoreTimeMeanName = StoreTimeMean.str();
  string StoreTimeSigmaName = StoreTimeSigma.str(); 
  string StoreTimeRefTotalName = StoreTimeRefTotal.str();  
  string StoreTimeRefPMTName = StoreTimeRefPMT.str();
  string StoreTimRefCellGroupName = StoreTimRefCellGroup.str(); 

  system(StoreTimeMeanName.c_str());
  system(StoreTimeSigmaName.c_str());
  system(StoreTimeRefTotalName.c_str());
  system(StoreTimeRefPMTName.c_str());
  system(StoreTimRefCellGroupName.c_str());
}

#endif
