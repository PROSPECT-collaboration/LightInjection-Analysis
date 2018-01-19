Download The LightInjection-Analysis Package from GitHub using git clone <http link>
Everything in the package are label accordingly

TIMING ANALYSIS Procedures:
To make everything easy, change directory to TimingAnalysis
1. Run root -l

2. Run .x TIMING_ANALYSIS_FULL_DETECTOR.cpp

3. Follow TIMING_ANALYSIS Prompt to proceed

3. Enjoy



GAIN ANALYSIS Procedures: 
To make everything easy, change directory to GainAnalysis first.

Manual Steps:
1. Make Output Directory at desire location and desire name -> Output Dir.

2. Change Parameters on PulseCruncher Config Files "LI_PulseCruncher.cfg".

3. Edit Makefile using any text editor: This makefile will run PulseCruncher using PulseCruncher's Config File (set integration windows and what channel to apply integration on, an example of PulseCruncher config file for light injection is locate within the GainAnalysis Folder named PulseCruncher.cfg). Within the make file, the term Location of InputFile means "where the unpacked waveform data are stored currently." Once the make file is completely prepared, run make on the terminal space to:
    Run PulseCrunhcer Job -> output to Output Dir 
    Run .h5 to .root convertor -> output to Output Dir

5. Combine Data Files: Copy and Paste ".root" Files in Output Dir to text file named "FileCombineList.txt" to combine step. A good trick to use is by typing "ls *.root" to list all ".root" files.

6. Single Data File: Follow Terminal Steps to choose whichever ".root" file desire for Gain Analysis.

Automate Step:
7. Run ./LIAnalysis <Output_Dir> <Number of Ch> <combine files? y or n> 
    If 5 is true (yes), LIAnalysis will combine files in FileCombineList.txt and then Proceed to GainAnalysis.
    If 5 is false (no), LIAnalysis will Proceed to GainAnalysis.




PLOTTING Procedures:
To make everything easy, change directory to Plotting
1. Run root -l
 
2. Run .x PLOT_TIME.cpp 

3. Inputs Essentials for PLOT_TIME

4. Run .x PLOT_GAIN.cpp 

5. Inputs Essentials for PLOT_GAIN_SINGLE

6. To Compare Yale vs HIFR run 
   a) .x PLOT_TIME.cpp twice for timing analysis
   b) .x PLOT_GAIN.cpp for Gain analysis


CalcOccupancy Procedures:
To make everything easy, change directory to CalcOccupancy first.
1. Run ./CalcOccupancy and follow the prompt on terminal.



Further questions?
Email: ktrinh.particle@gmail.com
Email: Johannes.Wagner@cc
