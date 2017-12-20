//Root Class
#include"TROOT.h"
#include"TFile.h"
#include"TObject.h"
#include"TString.h"
#include"TTree.h"
#include"TH1.h"
#include"TH2.h"
#include"TChain.h"
#include"TRandom3.h"

//C++ class
#include<math.h>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<fstream>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<ctime>
using namespace std;

//--------------------------------------------------------------------------------------------
/*CHANGEABLE GLOBAL VARIABLES*/

int number_of_samples = 120; //total number of samples per waveform (changing number of samples does not change range of pulse -> change pulse_range to do that) //120
int peak_lock = 60; //starting point of pulse //60
int number_of_channels = 308; //total number of PMTs
int entries_per_channel = 10000; //number of entries for each PMT
double data_voltage = 1600; //data set voltage (best used from 1500V - 1700V)
double mean_poisson = 100; //mean number of pulses //0.5 for 1700V Darkbox Data

//--------------------------------------------------------------------------------------------
/*FIXED GLOBAL VARIABLES*/

int number_of_entries = number_of_channels*entries_per_channel;
double baseline = 15000.0; //15000
int pulse_range = 10; //10
double null = 0.0;
double sigma_noise = 2.17; //2.17

//--------------------------------------------------------------------------------------------

//FUNCTION: returns a random number according to a gaussian distribution whith a given mean and sigma
double GAUSSIAN(double mean, double sigma)
{
    TRandom3 myrandom1(0);
    double random_number = myrandom1.Gaus(mean,sigma);
    
    return random_number;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: returns a random number according to a poisson distribution whith a given mean
double POISSON(double mean)
{
    TRandom3 myrandom2(0);
    double random_number = myrandom2.Poisson(mean);
    
    return random_number;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: defines the cubic spline function used to model the pulse shape
double CUBIC_SPLINE(double x) {
    const int fNp = 17, fKstep = 0;
    const double fDelta = -1, fXmin = 0, fXmax = 16;
    const double fX[17] = { 0, 1, 2, 3, 4,
        5, 6, 7, 8, 9,
        10, 11, 12, 13, 14,
        15, 16 };
    const double fY[17] = { 0, 0, 0, 0, -2.7,
        -15.2, -20.5, -24, -21.1, -11.7,
        -3.5, -2, -0.7, 0, 0,
        0, 0 };
    const double fB[17] = { -0.0620958, 0.0310479, -0.0620958, 0.217335, -8.90724,
        -10.1884, -3.73933, -1.25431, 6.95658, 10.328,
        4.53143, 0.646274, 1.28347, 0.219834, -0.0628098,
        0.0314049, -0.0628098 };
    const double fC[17] = { 0.0931436, 0, -0.0931436, 0.372575, -9.49715,
        8.21604, -1.76702, 4.25204, 3.95885, -0.587427,
        -5.20914, 1.32398, -0.68678, -0.376859, 0.0942146,
        0, 1 };
    const double fD[17] = { -0.0310479, -0.0310479, 0.155239, -3.28991, 5.9044,
        -3.32769, 2.00636, -0.0977328, -1.51542, -1.54057,
        2.17771, -0.670253, 0.103307, 0.157024, -0.0314049,
        -0.0314049, 0.464102 };
    int klow=0;
    if(x<=fXmin) klow=0;
    else if(x>=fXmax) klow=fNp-1;
    else {
        if(fKstep) {
            // Equidistant knots, use histogramming
            klow = int((x-fXmin)/fDelta);
            if (klow < fNp-1) klow = fNp-1;
        } else {
            int khig=fNp-1, khalf;
            // Non equidistant knots, binary search
            while(khig-klow>1)
                if(x>fX[khalf=(klow+khig)/2]) klow=khalf;
                else khig=khalf;
        }
    }
    // Evaluate now
    double dx=x-fX[klow];
    return (fY[klow]+dx*(fB[klow]+dx*(fC[klow]+dx*fD[klow])));
}

//--------------------------------------------------------------------------------------------

//FUNCTION: generates the noise points
double NOISE_GENERATOR()
{
    double noise_distribution = GAUSSIAN(null, sigma_noise);
    
    double noise = baseline + noise_distribution;
    
    return noise;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: generates the pulses (calls the cubic spline function)
double PULSE_GENERATOR(int j, int lower_pulse_bound, double function_shift)
{
    //interval of function where pulse is located
    double function_start = 2; //beginning of interval where function resembles pulse
    double function_end = 14; //end of interval where function resembles pulse
    
    double function_range = function_end - function_start;
    double function_step = function_range/static_cast<double>(pulse_range - 1);
    
    double x = function_start - function_step + (j - lower_pulse_bound)*function_step - function_shift*function_step;
    
    //best fit function given by interpolation of average waveform data
    double pulse_function = CUBIC_SPLINE(x);
    
    //pulse function can't get higher than the baseline (excluding noise)
    if(pulse_function > 0)
    {
        pulse_function = 0;
    }
    
    return pulse_function;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: re-scales the pulse after creation according to the integral area mean for each channel
double SCALE_PULSE(double pulse, double pulse_integral, double pulse_distribution)
{
    //This is the scale factor (given a random value from the Gaussian representing the area distribution of real data set divided by the integral of the analytical function representing the pulse in our simulation (defined in the PULSE_GENERATOR function)
    double integral_scale_factor = fabs(pulse_distribution/pulse_integral);
    
    pulse = pulse*integral_scale_factor;
    
    return pulse;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: calls GAUSSIAN or POISSON to determine how many pulses appear
int NOISE_OR_PULSE()
{
    int random_number = rand();
    int number_of_pulses = 0;
    
    if(mean_poisson <= 50)
    {
        number_of_pulses = round(POISSON(mean_poisson));
    }
    
    else
    {
        number_of_pulses = round(GAUSSIAN(mean_poisson, sqrt(mean_poisson)));
    }
    
    return number_of_pulses;
}

//--------------------------------------------------------------------------------------------

//FUNCTION: determines timing constants
void TIMING(const double time_offset, int &sample_shift, double &function_shift, int &lower_pulse_bound, int &upper_pulse_bound)
{
    double random_time = GAUSSIAN(time_offset, 0.1);

    sample_shift = round(random_time);
    function_shift = random_time - sample_shift;
    lower_pulse_bound = peak_lock + sample_shift;
    upper_pulse_bound = peak_lock + pulse_range + sample_shift;
}

//--------------------------------------------------------------------------------------------

void SIMULATION_V5()
{
    gROOT->Reset();
    
    int number_of_pulses;
    
    //counter variables
    int c = 0;
    int m = 0;
    
    //channel specific variable arrays
    double pulse_integral[number_of_entries];
    double time_offset[number_of_channels];
    double integral_mean[number_of_channels];
    double integral_sigma[number_of_channels];
    int sample_shift;
    double function_shift;
    int lower_pulse_bound;
    int upper_pulse_bound;
    double t_off, mean_integral, sigma_integral;
    
    //WriteTree variables
    UInt_t nsamp;
    Short_t voltage[number_of_samples];
    Int_t channel;
    ULong64_t event;
    ULong64_t trigger;
    Long64_t offset = 40000;//80000;
    
    double voltage_double[number_of_samples];
    double voltage_unscaled[number_of_samples];
    
    //setting up the ReadTree
    TFile *ReadFile = TFile::Open("PARAMETERS.root");
    TTree *ReadTree = (TTree*)ReadFile->Get("PARAMETERS");
    ReadTree->SetBranchAddress("time_offset", &t_off);
    ReadTree->SetBranchAddress("integral_mean", &mean_integral);
    
    //loop that saves the parameters in an array for each channel
    for(int b = 0; b < number_of_channels; b++)
    {
        ReadTree->GetEntry(b);
        integral_mean[b] = mean_integral*(exp(0.0042*data_voltage)/exp(0.0042*1700)); //assuming exponential relationship between integral mean and PMT voltage
        
        integral_sigma[b] = 0.3984*integral_mean[b] + 2.7299; //assuming linear relationship between integral mean and sigma
        time_offset[b] = t_off;
    }
    
    //setting up the WriteTree
    TFile *WriteFile = new TFile("SIMULATION.root","RECREATE");
    TTree *WriteTree = new TTree("Waveforms","");
    WriteTree->Branch("samps", &voltage, Form("voltage[%d]/S", number_of_samples));
    WriteTree->Branch("nsamp", &nsamp);
    WriteTree->Branch("chan", &channel);
    WriteTree->Branch("evt", &event);
    WriteTree->Branch("trig", &trigger);
    WriteTree->Branch("noff", &offset);
    
    //main loop that creates the simulated data
    for(int i = 0; i < number_of_entries; i++)
    {
        pulse_integral[i] = 0;
        number_of_pulses = NOISE_OR_PULSE();
        
        event = m;
        trigger = m + 1;
        channel = c;
        nsamp = number_of_samples;
        
        TIMING(time_offset[c], sample_shift, function_shift, lower_pulse_bound, upper_pulse_bound);
            
        double pulse_distribution[number_of_pulses];
        
        for(int a = 0; a < number_of_pulses; a++)
        {
            pulse_distribution[a] = GAUSSIAN(integral_mean[c], integral_sigma[c]);
            
            //this while loop makes sure that the SPE area peak value for every given photoelectron is larger than 0
            while(pulse_distribution[a] < 0)
            {
                pulse_distribution[a] = GAUSSIAN(integral_mean[c], integral_sigma[c]);
                
            }
        }
        
        for(int j = 0; j < number_of_samples; j++)
        {
            if((j <= lower_pulse_bound) || (j > upper_pulse_bound) || (number_of_pulses == 0))
            {
                voltage[j] = static_cast<short>(round(NOISE_GENERATOR()));
                
                if(voltage[j] > (pow(2,14) - 1))
                {
                    voltage[j] = pow(2,14) - 1;
                }
                
                else if(voltage[j] < 0)
                {
                    voltage[j] = 0;
                }
            }
            
            else if((j > lower_pulse_bound) && (j <= upper_pulse_bound) && (number_of_pulses != 0))
            {
                voltage_unscaled[j] = PULSE_GENERATOR(j, lower_pulse_bound, function_shift);
                pulse_integral[i] += voltage_unscaled[j]; //integral is calculated to rescale the pulse
            }
        }
        
        if(number_of_pulses != 0)
        {
            for(int d = lower_pulse_bound + 1; d <= upper_pulse_bound; d++)
            {
                voltage_double[d] = 0;
                
                for(int k = 0; k < number_of_pulses; k++)
                {
                    voltage_double[d] += SCALE_PULSE(voltage_unscaled[d], pulse_integral[i], pulse_distribution[k]);
                }
                
                voltage_double[d] += NOISE_GENERATOR();
                voltage[d] = static_cast<short>(round(voltage_double[d]));
                
                if(voltage[d] > (pow(2,14) - 1))
                {
                    voltage[d] = pow(2,14) - 1;
                }
                
                else if(voltage[d] < 0)
                {
                    voltage[d] = 0;
                }
            }
        }
        
        WriteTree->Fill();
        c++;
        
        if(c == number_of_channels)
        {
            c = 0;
            offset += 40000; //40000
            m++;
        }
        
        cout << (i + 1) << "/" << number_of_entries << endl;
    }
    
    WriteTree->Write();
    WriteFile->Close();
}
