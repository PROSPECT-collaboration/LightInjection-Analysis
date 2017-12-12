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

double CHECKINTEGRAL(double integral, short samps[], int lower_integration_limit, int upper_integration_limit, double baseline)
{
    double check_integral = 0;
    
    for(int i = lower_integration_limit; i <= upper_integration_limit; i++)
    {
        double x = 0;
        
        while(x < 1)
        {
            check_integral += (baseline - static_cast<double>(samps[i]))*0.001;
            x += 0.001;
            
            if(check_integral > 0.20*integral)
            {
                return (static_cast<double>(i) + x);
            }
        }
    }
    
    return 0;
}

void TIMING_ANALYSIS()
{
    //Reset CERN Root
    gROOT->Reset();
    
    string filename;
    
    cout << "ENTER THE FILENAME FOR THE UNPACKED FILE: ";
    cin >> filename;
    
    //Open the file and call in the Unpacked Tree
    TFile* ReadFile1 = TFile::Open(filename.c_str());
    TTree* ReadTree1 = (TTree*)ReadFile1->Get("Waveforms");
    int n = ReadTree1->GetEntries();
    int number_of_samples, number_of_channels;
    
    cout << "ENTER THE NUMBER OF SAMPLES FOR THE DESIRED DATA SET: ";
    cin >> number_of_samples;
    
    short samps[number_of_samples];
    int chan;
    ReadTree1->SetBranchAddress("samps", &samps);
    ReadTree1->SetBranchAddress("chan", &chan);
    
    //check how many channels there are
    int channel = -1;
    int prev_channel;
    for(int o = 0; o <= n; o++)
    {
        ReadTree1->GetEntry(o);
        prev_channel = channel;
        channel = chan;
        
        if(channel > prev_channel)
        {
            number_of_channels = channel + 1;
        }
        
        else
        {
            break;
        }
    }
    
    cout << "Number of channels = " << number_of_channels << endl;
    
    int lower_baseline_limit;
    int upper_baseline_limit;
    int lower_integration_limit;
    int upper_integration_limit;
    
    //Create a new Root file and a new tree for references
    TFile* WriteFile = new TFile("TIMING.root","RECREATE");
    TTree* WriteTree = new TTree("DATA","");
    WriteTree->SetEntries(n/number_of_channels);

    double time;
    double integral;
    
    for(int m = 0; m < number_of_channels; m++)
    {
        ostringstream timebranch;
        timebranch << "timec" << m;
        string timebranchname = timebranch.str();
        
        TBranch* TimeBranch = WriteTree->Branch(timebranchname.c_str(), &time);
        for(int i = 0; i < n; i++)
        {
            ReadTree1->GetEntry(i);
            integral = 0;
            int counter = 0;
            int baseline = 0;
            int min = 100000;
            int peak_loc = 0;
            
            if(chan == m)
            {
                //find integration and baseline calculation ranges
                for(int j = 0; j < number_of_samples; j++)
                {
                    if(samps[j] < min)
                    {
                        min = samps[j];
                        peak_loc = j;
                    }
                }
                
                lower_baseline_limit = peak_loc - 40;
                upper_baseline_limit = peak_loc - 20;
                lower_integration_limit = peak_loc - 10;
                upper_integration_limit = peak_loc + 10;
                
                //calculate baseline
                for(int j = lower_baseline_limit; j <= upper_baseline_limit; j++)
                {
                    baseline += samps[j];
                    counter++;
                }
                
                baseline = baseline/static_cast<double>(counter);
                
                //calculate integral
                for(int j = lower_integration_limit; j <= upper_integration_limit; j++)
                {
                    integral += (baseline - samps[j]);
                }
                
                time = CHECKINTEGRAL(integral, samps, lower_integration_limit, upper_integration_limit, baseline)*4;
                
                if((lower_integration_limit < 0) || (upper_integration_limit > number_of_samples) || (min > (baseline - 30)))
                {
                    time = 0;
                }
                
                TimeBranch->Fill();
            }
        }
        cout << "Processing data for channel " << (m + 1) << endl;
    }
    
    //Close the write/read file.
    WriteTree->Write();
    WriteFile->Close();
    ReadFile1->Close();
    
    int upper_limit = number_of_samples*4;
    int lower_limit = 20;
    double total_reference_time;
    double PMT_reference_time;
    double cellgroup_reference_time;
    int reference_channel = 0;
    
    TFile* ReadFile2 = TFile::Open("TIMING.root");
    TTree* ReadTree2 = (TTree*)ReadFile2->Get("DATA");
        
    ofstream fout1;
    fout1.open("Mean.txt");
    ofstream fout2;
    fout2.open("Sigma.txt");
    ofstream fout3;
    fout3.open("Mean_Difference_Total.txt");
    ofstream fout4;
    fout4.open("Mean_Difference_PMT.txt");
    ofstream fout5;
    fout5.open("Mean_Difference_Cell_Group.txt");
    
    int m = ReadTree2->GetEntries();
    
    int pulse_maxbin;
    double pulse_maxvalue, scale, mean;
    double time_pulse;
    
    for(int k = 0; k < number_of_channels; k++)
    {
        ostringstream timebranch;
        timebranch << "timec" << k;
        string timebranchname = timebranch.str();
        
        TH1F* histogram_pulse = new TH1F("","",10000,lower_limit,upper_limit); //500,-100,300
        
        ReadTree2->SetBranchAddress(timebranchname.c_str(), &time_pulse);
        
        for(int i = 0; i < m; i++)
        {
            ReadTree2->GetEntry(i);
            
            histogram_pulse->Fill(time_pulse);
        }
        
        double mean = histogram_pulse->GetMean(1);
        double sigma = histogram_pulse->GetStdDev(1);
        
        if(k == reference_channel)
        {
            total_reference_time = mean;
        }
        
        if(k % 2 == 0)
        {
            PMT_reference_time = mean;
        }
        
        if(k % 8 == 0)
        {
            cellgroup_reference_time = mean;
        }
        
        fout1 << "PMTNumber: " << k << " PMTTimingMean: " << mean << endl;
        fout2 << "PMTNumber: " << k << " PMTTimingSigma: " << sigma << endl;
        fout3 << "PMTNumber: " << k << " PMTTimingReferenceTotal: " << mean - total_reference_time << endl;
        fout4 << "PMTNumber: " << k << " PMTTimingReferencePMT: " << mean - PMT_reference_time << endl;
        fout5 << "PMTNumber: " << k << " PMTTimingReferenceCellGroup: " << mean - cellgroup_reference_time << endl;
        
    }
    
    fout1.close();
    fout2.close();
    fout3.close();
    ReadFile2->Close();
}

int main()
{
    TIMING_ANALYSIS();
    return 0;
}
