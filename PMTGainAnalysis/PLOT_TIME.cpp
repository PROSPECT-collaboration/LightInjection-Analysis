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

void PLOT_TIME()
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
    vector<float> time1;
    vector<float> time2;
    
    while(!input_file1.eof() && !input_file2.eof())
    {
        //reads in the beginning string
        input_file1 >> current_word;
        input_file2 >> current_word;
        
        //reads in the channel number
        input_file1 >> current_int;
        chan.push_back(current_int);
        input_file2 >> current_int;
        
        //reads in the middle string
        input_file1 >> current_word;
        input_file2 >> current_word;
        
        //reads in the PMT gain
        input_file1 >> current_float;
        time1.push_back(current_float);
        input_file2 >> current_float;
        time2.push_back(current_float);
    }
    
    //last entry is read in twice, which is rectified here
    if(chan[chan.size() - 2] == chan[chan.size() - 1])
    {
        chan.pop_back();
        time1.pop_back();
        time2.pop_back();
    }
    
    int number_of_channels = chan.size();
    float timediff[number_of_channels];
    float channel[number_of_channels];
    
    TFile* WriteFile = new TFile("TIME.root","RECREATE");
    TTree* WriteTree = new TTree("TIME","");
    TH1F* h1 = new TH1F("","",10,0,0);
    TH1F* h2 = new TH1F("","",10,0,0);
    TH1F* h3 = new TH1F("","",10,0,0);
    
    float time1_array[number_of_channels];
    float time2_array[number_of_channels];
    
    //gaindiff is an error percentage with respect to the first file
    for(int i = 0; i < number_of_channels; i++)
    {
        timediff[i] = (time2[i] - time1[i]);
        channel[i] = i;
        time1_array[i] = time1[i];
        time2_array[i] = time2[i];
        h1->Fill(timediff[i]);
        h2->Fill(time1[i]);
        h3->Fill(time2[i]);
    }
    
    h1->Write();
    
    //x-y plots of gain difference vs. channel
    TCanvas* c1 = new TCanvas();
    TGraph* g1_1 = new TGraph(number_of_channels,channel,timediff);
    g1_1->SetTitle("Channel vs. Arrival Time Difference");
    g1_1->GetXaxis()->SetTitle("Channel");
    g1_1->GetYaxis()->SetTitle("Arrival Time Difference (in ns)");
    g1_1->GetXaxis()->SetLimits(0,number_of_channels - 1);
    g1_1->SetMarkerStyle(8);
    g1_1->SetDrawOption("P");
    g1_1->Draw("AP");
    c1->SaveAs("xyPlot_timediff.pdf");
    delete(c1);
    delete(g1_1);
    
    c1 = new TCanvas();
    TGraph* g1_2 = new TGraph(number_of_channels,channel,time1_array);
    g1_2->SetTitle("Channel vs. Arrival Time (HFIR)");
    g1_2->GetXaxis()->SetTitle("Channel");
    g1_2->GetYaxis()->SetTitle("Arrival Time (in ns)");
    g1_2->GetXaxis()->SetLimits(0,number_of_channels - 1);
    g1_2->SetMarkerStyle(8);
    g1_2->SetDrawOption("P");
    g1_2->Draw("AP");
    c1->SaveAs("xyPlot_timehfir.pdf");
    delete(c1);
    delete(g1_2);
    
    c1 = new TCanvas();
    TGraph* g1_3 = new TGraph(number_of_channels,channel,time2_array);
    g1_3->SetTitle("Channel vs. Arrival Time (Yale)");
    g1_3->GetXaxis()->SetTitle("Channel");
    g1_3->GetYaxis()->SetTitle("Arrival Time (in ns)");
    g1_3->GetXaxis()->SetLimits(0,number_of_channels - 1);
    g1_3->SetMarkerStyle(8);
    g1_3->SetDrawOption("P");
    g1_3->Draw("AP");
    c1->SaveAs("xyPlot_timeyale.pdf");
    delete(c1);
    delete(g1_3);
    
    //1d histogram of gain differences
    h1->SetLineColor(1);
    h1->SetTitle("Arrival Time Difference (in ns) - Histogram");
    c1 = new TCanvas();
    h1->Draw("");
    c1->SaveAs("1dHistogram_timediff.pdf");
    delete(c1);
    delete(h1);
    
    h2->SetLineColor(1);
    h2->SetTitle("Arrival Time (HFIR) - Histogram");
    c1 = new TCanvas();
    h2->Draw("");
    c1->SaveAs("1dHistogram_timehfir.pdf");
    delete(c1);
    delete(h2);
    
    h3->SetLineColor(1);
    h3->SetTitle("Arrival Time (Yale) - Histogram");
    c1 = new TCanvas();
    h3->Draw("");
    c1->SaveAs("1dHistogram_timeyale.pdf");
    delete(c1);
    delete(h3);
    
    //2d histogram
    TH2F* h2_1 = new TH2F("","",28,0,28,11,0,11);
    TH2F* h2_2 = new TH2F("","",28,0,28,11,0,11);
    TH2F* h2_3 = new TH2F("","",28,0,28,11,0,11);

    int x,y;
    
    for(int i = 0; i < number_of_channels; i++)
    {
        x = i%28;
        y = i/28;
        h2_1->Fill(x,y,timediff[i]);
        h2_2->Fill(x,y,time1_array[i]);
        h2_3->Fill(x,y,time2_array[i]);
    }
    
    h2_1->Write();
    h2_1->SetStats(false);
    c1 = new TCanvas();
    gStyle->SetPalette(55);
    h2_1->SetTitle("Arrival Time Difference (in ns) - 2D Histogram");
    h2_1->Draw("colz");
    c1->SaveAs("2dHistogram_timediff.pdf");
    delete(c1);
    delete(h2_1);
    
    h2_2->Write();
    h2_2->SetStats(false);
    c1 = new TCanvas();
    gStyle->SetPalette(55);
    h2_2->SetTitle("Arrival Time (HFIR) - 2D Graph");
    h2_2->Draw("colz");
    c1->SaveAs("2dHistogram_timehfir.pdf");
    delete(c1);
    delete(h2_2);
    
    h2_3->Write();
    h2_3->SetStats(false);
    c1 = new TCanvas();
    gStyle->SetPalette(55);
    h2_3->SetTitle("Arrival Time (Yale) - 2D Graph");
    h2_3->Draw("colz");
    c1->SaveAs("2dHistogram_timeyale.pdf");
    delete(c1);
    delete(h2_3);
    
    //2d graph
    /*double x[308];
    double y[308];
    double timediff_full[308];
    double time1_full[308];
    double time2_full[308];
    
    TGraph2D* g2_1 = new TGraph2D();
    TGraph2D* g2_2 = new TGraph2D();
    TGraph2D* g2_3 = new TGraph2D();
    
    for(int i = 0; i < 308; i++)
    {
        x[i] = i % (28);
        y[i] = i / (28);
        
        if(i >= number_of_channels)
        {
            timediff_full[i] = 0;
            time1_full[i] = 0;
            time2_full[i] = 0;
        }
        
        else
        {
            timediff_full[i] = timediff[i];
            time1_full[i] = time1[i];
            time2_full[i] = time2[i];
        }
        
        g2_1->SetPoint(i,x[i],y[i],timediff_full[i]);
        g2_2->SetPoint(i,x[i],y[i],time1_full[i]);
        g2_3->SetPoint(i,x[i],y[i],time2_full[i]);
    }
    
    c1 = new TCanvas();
    g2_1->SetTitle("Arrival Time Difference - 2D Graph");
    g2_1->Draw("surf1");
    gPad->SetTheta(90);
    gPad->SetPhi(0);
    gPad->Update();
    c1->SaveAs("2dHistogram_timediff.pdf");
    delete(c1);
    delete(g2_1);
    
    c1 = new TCanvas();
    g2_2->SetTitle("Arrival Time (HF) - 2D Graph");
    g2_2->Draw("surf1");
    gPad->SetTheta(90);
    gPad->SetPhi(0);
    gPad->Update();
    c1->SaveAs("2dHistogram_timehfr.pdf");
    delete(c1);
    delete(g2_2);

    c1 = new TCanvas();
    g2_3->SetTitle("Arrival Time (Yale) - 2D Graph");
    g2_3->Draw("surf1");
    gPad->SetTheta(90);
    gPad->SetPhi(0);
    gPad->Update();
    c1->SaveAs("2dHistogram_timeyale.pdf");
    delete(c1);
    delete(g2_3);*/
    
}

int main()
{
    PLOT_TIME();
    return 0;
}
