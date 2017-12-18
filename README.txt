TIMING ANALYSIS Procedures:
To make everything easy, change directory to TimingAnalysis
1. Run root -l

2. Run .x TIMING_ANALYSIS_FULL_DETECTOR.cpp

3. Enjoy



GAIN ANALYSIS Procedures: 
To make everything easy, change directory to GainAnalysis first.

Manual Steps:
1. Make Output Directory at desire location and desire name -> Output Dir

2. Change Parameters on PulseCruncher Config Files "LI_PulseCruncher.cfg"

3. Edit Makefile to: 
    Run PulseCrunhcer Job -> output to Pre-Output Dir 
    Run .h5 to .root convertor -> output to Pre-Output Dir

5. Combine Data Files: Copy and Paste ".root" Files in Output Dir to text file named "FileCombineList.txt" to combine step

6. Single Data File: Follow Terminal Steps to choose ".root" file for Gain Analysis

Automate Step:
7. Run ./LIAnalysis <combine files? yes or no> 
    If 5 is true (yes), LIAnalysis will combine files in FileCombineList.txt and then Proceed to GainAnalysis.
    If 5 is false (no), LIAnalysis will Proceed to GainAnalysis 




PLOTTING Procedures:
To make everything easy, change directory to Plotting
1. Run root -l
 
2. Run .x PLOT_TIME.cpp 

3. Inputs Essentials for PLOT_TIME

4. Run .x PLOT_GAIN.cpp 

5. Inputs Essentials for PLOT_GAIN_SINGLE

6. To Compare Yale vs HIFR run 
   a) .x PLOT_TIME.cpp twice
   b) .x PLOT_GAIN.cpp for Gain analysis
