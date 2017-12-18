//Root Class
#include"TROOT.h"
#include"TFile.h"
#include"TObject.h"
#include"TString.h"
#include"TTree.h"
#include"TGraph.h"
#include"TH1.h"
#include"TCanvas.h"
#include"THistPainter.h"
#include"TChain.h"
#include"TGraph2D.h"

//C++ class
#include<math.h>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<fstream>
using namespace std;

void PLOT_GAIN()
{
    gROOT->Reset();
    
    ifstream input_file1;
    string input_file1_name;
    cout << "WHAT IS THE NAME OF THE HFIR FILE?" << endl;
    cin >> input_file1_name;
    input_file1.open(input_file1_name);
    
    ifstream input_file2;
    string input_file2_name;
    cout << "WHAT IS THE NAME OF THE YALE FILE?" << endl;
    cin >> input_file2_name;
    input_file2.open(input_file2_name);
    
    int current_int;
    float current_float;
    string current_word;
    vector<int> chan;
    vector<float> gain1;
    vector<float> gain2;
    
    int number_of_channels = 0; //
    
    while(!input_file1.eof() && !input_file2.eof())
    {
        /*//reads in the two beginning strings
        input_file1 >> current_word;
        input_file1 >> current_word;
        input_file2 >> current_word;
        input_file2 >> current_word;
        
        //reads in the channel number
        input_file1 >> current_int;
        chan.push_back(current_int);
        input_file2 >> current_int;
        
        //reads in the middle string
        input_file1 >> current_word;
        input_file2 >> current_word;*/
        
        //reads in the PMT gain
        input_file1 >> current_float;
        gain1.push_back(current_float);
        input_file2 >> current_float;
        gain2.push_back(current_float);
        
        number_of_channels++; //
    }
    
    //last entry is read in twice, which is rectified here
    /*if(chan[chan.size() - 2] == chan[chan.size() - 1])
    {
        chan.pop_back();
        gain1.pop_back();
        gain2.pop_back();
    }*/
    
    //int number_of_channels = chan.size();
    float gaindiff[number_of_channels];
    float channel[number_of_channels];
    
    TFile* WriteFile = new TFile("GAIN.root","RECREATE");
    TTree* WriteTree = new TTree("GAIN","");
    TH1F* h1 = new TH1F("","",10,0,0);
    
    //gaindiff is an error percentage with respect to the first file
    for(int i = 0; i < number_of_channels; i++)
    {
        gaindiff[i] = ((gain2[i] - gain1[i])/gain2[i])*100;
        channel[i] = i;
        h1->Fill(gaindiff[i]);
    }
    
    h1->Write();
    
    //x-y plot of gain difference vs. channel
    TCanvas* c1 = new TCanvas();
    TGraph* graph = new TGraph(number_of_channels,channel,gaindiff);
    graph->SetTitle("Channel vs. Gain Difference Percentage");
    graph->GetXaxis()->SetTitle("Channel");
    graph->GetYaxis()->SetTitle("Gain Difference Percentage");
    graph->GetXaxis()->SetLimits(0,number_of_channels - 1);
    graph->SetMarkerStyle(8);
    graph->SetDrawOption("P");
    graph->Draw("AP");
    c1->SaveAs("xyPlot_gain.pdf");
    delete(c1);
    delete(graph);
    
    //1d histogram of gain differences
    h1->SetLineColor(1);
    h1->SetTitle("Gain Difference Percentage - Histogram");
    c1 = new TCanvas();
    h1->Draw("");
    c1->SaveAs("1dHistogram_gain.pdf");
    delete(c1);
    delete(h1);
    
    //2d histogram
    TH2F* h2 = new TH2F("","",28,0,28,11,0,11);
    int x,y;
    
    for(int i = 0; i < number_of_channels; i++)
    {
        x = i%28;
        y = i/28;
        h2->Fill(x,y,gaindiff[i]);
    }
    
    h2->Write();
    h2->SetStats(false);
    c1 = new TCanvas();
    gStyle->SetPalette(55);
    h2->SetTitle("Gain Difference Percentage - 2D Graph");
    h2->Draw("colz");
    c1->SaveAs("2dHistogram_gain.pdf");
    delete(c1);
    delete(h2);

    //2d graph
    /*double x[308];
    double y[308];
    double gaindiff_full[308];
    
    //TGraph2D* g2 = new TGraph2D();
    for(int i = 0; i < 308; i++)
    {
        x[i] = i % (28);
        y[i] = i / (28);
        
        if(i >= number_of_channels)
        {
            gaindiff_full[i] = 0;
        }
        
        else
        {
            gaindiff_full[i] = gaindiff[i];
        }
        
        g2->SetPoint(i,x[i],y[i],gaindiff_full[i]);
    }
    
    c1 = new TCanvas();
    gStyle->SetPalette(1);
    g2->SetMarkerStyle(20);
    g2->SetTitle("Gain Difference Percentage - 2D Graph");
    gPad->SetTheta(90);
    gPad->SetPhi(0);
    gPad->Update();
    g2->Draw("pcolz");
    c1->SaveAs("2dHistogram_gain.pdf");*/
}

int main()
{
    PLOT_GAIN();
    return 0;
}
