//Timing analysis code with detector map for full detector

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

//Checks at what time the integral of a pulse passes 20% of its total area
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

//Maps channels in the same PMT group to be in succession (this is important for the relative timing to be correct)
int CHANNELMAP(int real_channel, bool reverse)
{
    int mapped_channel, reverse_mapped_channel;
    
    if(real_channel <= 3 || real_channel > 275 || (real_channel > 51 && real_channel <= 59) || (real_channel > 107 && real_channel <= 115) || (real_channel > 163 && real_channel <= 171) || (real_channel > 219 && real_channel <= 227))
    {
        mapped_channel = real_channel;
        reverse_mapped_channel = real_channel;
    }
    
    else if((real_channel > 3 && real_channel <= 7) || (real_channel > 59 && real_channel <= 63) || (real_channel > 115 && real_channel <= 119) || (real_channel > 171 && real_channel <= 175) || (real_channel > 227 && real_channel <= 231))
    {
        mapped_channel = real_channel + 4;
        reverse_mapped_channel = real_channel + 24;
    }
    
    else if((real_channel > 7 && real_channel <= 11) || (real_channel > 63 && real_channel <= 67) || (real_channel > 119 && real_channel <= 123) || (real_channel > 175 && real_channel <= 179) || (real_channel > 231 && real_channel <= 235))
    {
        mapped_channel = real_channel + 8;
        reverse_mapped_channel = real_channel - 4;
    }
    
    else if((real_channel > 11 && real_channel <= 15) || (real_channel > 67 && real_channel <= 71) || (real_channel > 123 && real_channel <= 127) || (real_channel > 179 && real_channel <= 183) || (real_channel > 235 && real_channel <= 239))
    {
        mapped_channel = real_channel + 12;
        reverse_mapped_channel = real_channel + 20;
    }
    
    else if((real_channel > 15 && real_channel <= 19) || (real_channel > 71 && real_channel <= 75) || (real_channel > 127 && real_channel <= 131) || (real_channel > 183 && real_channel <= 187) || (real_channel > 239 && real_channel <= 243))
    {
        mapped_channel = real_channel + 16;
        reverse_mapped_channel = real_channel - 8;
    }
    
    else if((real_channel > 19 && real_channel <= 23) || (real_channel > 75 && real_channel <= 79) || (real_channel > 131 && real_channel <= 135) || (real_channel > 187 && real_channel <= 191) || (real_channel > 243 && real_channel <= 247))
    {
        mapped_channel = real_channel + 20;
        reverse_mapped_channel = real_channel + 16;
    }
    
    else if((real_channel > 23 && real_channel <= 27) || (real_channel > 79 && real_channel <= 83) || (real_channel > 135 && real_channel <= 139) || (real_channel > 191 && real_channel <= 195) || (real_channel > 247 && real_channel <= 251))
    {
        mapped_channel = real_channel + 24;
        reverse_mapped_channel = real_channel - 12;
    }
    
    else if((real_channel > 27 && real_channel <= 31) || (real_channel > 83 && real_channel <= 87) || (real_channel > 139 && real_channel <= 143) || (real_channel > 195 && real_channel <= 199) || (real_channel > 251 && real_channel <= 255))
    {
        mapped_channel = real_channel - 24;
        reverse_mapped_channel = real_channel + 12;
    }
    
    else if((real_channel > 31 && real_channel <= 35) || (real_channel > 87 && real_channel <= 91) || (real_channel > 143 && real_channel <= 147) || (real_channel > 199 && real_channel <= 203) || (real_channel > 255 && real_channel <= 259))
    {
        mapped_channel = real_channel - 20;
        reverse_mapped_channel = real_channel - 16;
    }
    
    else if((real_channel > 35 && real_channel <= 39) || (real_channel > 91 && real_channel <= 95) || (real_channel > 147 && real_channel <= 151) || (real_channel > 203 && real_channel <= 207) || (real_channel > 259 && real_channel <= 263))
    {
        mapped_channel = real_channel - 16;
        reverse_mapped_channel = real_channel + 8;
    }
    
    else if((real_channel > 39 && real_channel <= 43) || (real_channel > 95 && real_channel <= 99) || (real_channel > 151 && real_channel <= 155) || (real_channel > 207 && real_channel <= 211) || (real_channel > 263 && real_channel <= 267))
    {
        mapped_channel = real_channel - 12;
        reverse_mapped_channel = real_channel - 20;
    }
    
    else if((real_channel > 43 && real_channel <= 47) || (real_channel > 99 && real_channel <= 103) || (real_channel > 155 && real_channel <= 159) || (real_channel > 211 && real_channel <= 215) || (real_channel > 267 && real_channel <= 271))
    {
        mapped_channel = real_channel - 8;
        reverse_mapped_channel = real_channel + 4;
    }
    
    else if((real_channel > 47 && real_channel <= 51) || (real_channel > 103 && real_channel <= 107) || (real_channel > 159 && real_channel <= 163) || (real_channel > 215 && real_channel <= 219) || (real_channel > 271 && real_channel <= 275))
    {
        mapped_channel = real_channel - 4;
        reverse_mapped_channel = real_channel - 24;
    }
    
    if(reverse)
    {
        return reverse_mapped_channel;
    }
    
    else
    {
        return mapped_channel;
    }
}

void TIMING_ANALYSIS_FULL_DETECTOR()
{
    //Reset CERN Root
    gROOT->Reset();
    
    string filename;
    
    cout << "ENTER THE NAME OF THE .txt FILE CONTAINING ALL FILE NAMES: ";
    cin >> filename;
    
    ifstream inputfile;
    inputfile.open(filename);
    string current_word;
    TChain ReadTree1("Waveforms");
    
    while(!inputfile.eof())
    {
        inputfile >> current_word;
        ReadTree1.Add(current_word.c_str());
    }
    
    //Open the file and call in the Unpacked Tree
    int n = ReadTree1.GetEntries();
    int number_of_samples, number_of_channels;
    int triggerchannel;
    
    cout << "ENTER THE NUMBER OF SAMPLES PER WAVEFORM FOR THE DESIRED DATA SET: ";
    cin >> number_of_samples;
    
 //   cout << "ENTER THE CHANNEL NUMBER OF THE TRIGGER CHANNEL: ";
 //   cin >> triggerchannel;
    
    short samps[number_of_samples];
    int chan;
    ReadTree1.SetBranchAddress("samps", &samps);
    ReadTree1.SetBranchAddress("chan", &chan);
    
    //check how many channels there are
    int channel = -1;
    int prev_channel;
    for(int o = 0; o <= n; o++)
    {
        ReadTree1.GetEntry(o);
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
    
    number_of_channels = 308;
    
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
    double baseline;
    int counter;
    int triggerindex;
    vector<int> trigger;
    double abs_timediff;
    double rel_timediff;
    double abs_reftime;
    double rel_reftime;
    
    //This loop ensures that only when the trigger channel registers a trigger, will the timing be calculated
  /*  for(int i = 0; i < n; i++)
    {
        ReadTree1.GetEntry(i);
        integral = 0;
        counter = 0;
        baseline = 0;
        
        if(chan == triggerchannel)
        {
            for(int j = 0; j <= 20; j++)
            {
                baseline += samps[j];
                counter++;
            }
            
            baseline = baseline/static_cast<double>(counter);
            
            for(int j = 0; j <= number_of_samples; j++)
            {
                integral += (baseline - samps[j]);
            }
            
            if(integral > 100000)
            {
                trigger.push_back(i);
            }
        }
    } */
    
    vector<double> abs_reference;
    vector<double> rel_reference;
    int referenceindex;
    int map;
    
    for(int m = 0; m < number_of_channels; m++)
    {
    //    if(CHANNELMAP(m,true) != triggerchannel)
    //    {
            cout << "Processing data for channel " << CHANNELMAP(m,true) << endl;
            
            ostringstream timebranch;
            timebranch << "timec" << CHANNELMAP(m,true);
            string timebranchname = timebranch.str();
            
            ostringstream abstimebranch;
            abstimebranch << "abstimec" << CHANNELMAP(m,true);
            string abstimebranchname = abstimebranch.str();
            
            ostringstream reltimebranch;
            reltimebranch << "reltimec" << CHANNELMAP(m,true);
            string reltimebranchname = reltimebranch.str();
            
            ostringstream areabranch;
            areabranch << "areac" << CHANNELMAP(m,true);
            string areabranchname = areabranch.str();
            
            triggerindex = 0;
            referenceindex = 0;
            
            TBranch* TimeBranch = WriteTree->Branch(timebranchname.c_str(), &time);
            TBranch* RelTimeBranch = WriteTree->Branch(reltimebranchname.c_str(), &rel_timediff);
            TBranch* AbsTimeBranch = WriteTree->Branch(abstimebranchname.c_str(), &abs_timediff);
            TBranch* AreaBranch = WriteTree->Branch(areabranchname.c_str(), & integral);
            
            if(((m % 8 == 0) && (CHANNELMAP(m,true) < 280)) || ((m % 4 == 0) && (CHANNELMAP(m,true) >= 280)))
            {
                rel_reference.clear();
            }
            
            for(int i = 0; i < n; i++)
            {
                ReadTree1.GetEntry(i);
                integral = 0;
                counter = 0;
                baseline = 0;
                int min = 100000;
                int peak_loc = 0;
                
            //    if(chan == CHANNELMAP(m,true) && i == (trigger[triggerindex] + chan - triggerchannel))
            //    {
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
                    
                    
                    if(m == 0)
                    {
                        abs_reference.push_back(time);
                    }
                    
                    if(((m % 8 == 0) && (CHANNELMAP(m,true) < 280)) || ((m % 4 == 0) && (CHANNELMAP(m,true) >= 280)))
                    {
                        rel_reference.push_back(time);
                    }
                    
                    abs_timediff = time - abs_reference[referenceindex];
                    rel_timediff = time - rel_reference[referenceindex];
                    
                    TimeBranch->Fill();
                    RelTimeBranch->Fill();
                    AbsTimeBranch->Fill();
                    AreaBranch->Fill();
                    
                    triggerindex++;
                    referenceindex++;
             //   }
                
             /*   else if(chan == CHANNELMAP(m,true)) && i != (trigger[triggerindex] + chan - triggerchannel))
                {
                    time = -10000;
                    abs_timediff = -10000;
                    rel_timediff = -10000;
                    integral = -10000;
                    
                    TimeBranch->Fill();
                    RelTimeBranch->Fill();
                    AbsTimeBranch->Fill();
                    AreaBranch->Fill();
                }
            } */
        }
    }
    
    //Close the write/read file.
    WriteTree->Write();
    WriteFile->Close();
    
    TFile* ReadFile2 = TFile::Open("TIMING.root");
    TTree* ReadTree2 = (TTree*)ReadFile2->Get("DATA");
    
    ofstream fout1;
    fout1.open("Mean.txt");
    ofstream fout2;
    fout2.open("Mean_Difference_Total.txt");
    ofstream fout3;
    fout3.open("Mean_Difference_Cell_Group.txt");
    ofstream fout4;
    fout4.open("Mean_Pulse_Area.txt");
    
    int m = ReadTree2->GetEntries();
    
    int pulse_maxbin;
    double pulse_maxvalue, scale, mean;
    double time_pulse, reltime_pulse, abstime_pulse, area_pulse;
    
    TFile *WriteFile2 = new TFile("HISTOGRAMS.root","RECREATE");
    TTree *WriteTree2 = new TTree("HISTOGRAMS","");
    
    for(int k = 0; k < number_of_channels; k++)
    {
        gROOT->Reset();
        
       // if(k != triggerchannel)
       // {
            ostringstream timebranch;
            timebranch << "timec" << CHANNELMAP(k,true);
            string timebranchname = timebranch.str();
            
            ostringstream abstimebranch;
            abstimebranch << "abstimec" << CHANNELMAP(k,true);
            string abstimebranchname = abstimebranch.str();
            
            ostringstream reltimebranch;
            reltimebranch << "reltimec" << CHANNELMAP(k,true);
            string reltimebranchname = reltimebranch.str();
            
            ostringstream areabranch;
            areabranch << "areac" << CHANNELMAP(k,true);
            string areabranchname = areabranch.str();
            
            TH1F* time_hist = new TH1F("","",10000,0,number_of_samples*4);
            TH1F* abstime_hist = new TH1F("","",1000,-50,50);
            TH1F* reltime_hist = new TH1F("","",1000,-50,50);
            TH1F* area_hist = new TH1F("","",1000,0,100000);
            
            ReadTree2->SetBranchAddress(timebranchname.c_str(), &time_pulse);
            ReadTree2->SetBranchAddress(reltimebranchname.c_str(), &reltime_pulse);
            ReadTree2->SetBranchAddress(abstimebranchname.c_str(), &abstime_pulse);
            ReadTree2->SetBranchAddress(areabranchname.c_str(), &area_pulse);
            
            for(int i = 0; i < m; i++)
            {
                ReadTree2->GetEntry(i);
                
                time_hist->Fill(time_pulse);
                abstime_hist->Fill(abstime_pulse);
                reltime_hist->Fill(reltime_pulse);
                area_hist->Fill(area_pulse);
                
            }
            
            time_hist->SetTitle(timebranchname.c_str());
            abstime_hist->SetTitle(abstimebranchname.c_str());
            reltime_hist->SetTitle(reltimebranchname.c_str());
            area_hist->SetTitle(areabranchname.c_str());
            
            time_hist->Write();
            reltime_hist->Write();
            abstime_hist->Write();
            area_hist->Write();
            
            double mean = time_hist->GetMean(1);
            double sigma = time_hist->GetStdDev(1);
            double mean_absdiff = abstime_hist->GetMean(1);
            double mean_reldiff = reltime_hist->GetMean(1);
            double mean_area = area_hist->GetMean(1);
            
            fout1 << "PMTNumber: " << CHANNELMAP(k,true) << " PMTTimingMean: " << mean << endl;
            fout2 << "PMTNumber: " << CHANNELMAP(k,true) << " PMTTimingReferenceTotal: " << mean_absdiff << endl;
            fout3 << "PMTNumber: " << CHANNELMAP(k,true) << " PMTTimingReferenceCellGroup: " << mean_reldiff << endl;
            fout4 << "PMTNumber: " << CHANNELMAP(k,true) << " PulseArea: " << mean_area << endl;
            
       // }
    }
    
    WriteTree2->Write();
    
    fout1.close();
    fout2.close();
    fout3.close();
    fout4.close();
    ReadFile2->Close();
}

int main()
{
    TIMING_ANALYSIS_FULL_DETECTOR();
    return 0;
}
