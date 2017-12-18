# Change contents in "all:"
# For Contents under #Execute PulseCruncher use:
# <Location of PulseCruncher> <PulseCruncher Config File> <Location of InputFile> <Location of Waveform File> <Location to Output/Output File Name>
# For Concents under #Execute .h5 to .root convertor use:
# <Location of Convertor> <Location of .h5>
all:
	#Execute PulseCruncher
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/MPMAnalysis/V5/PMTGainAnalysis/Core/LS.cfg /projects/prospect/converted_data/Unpacked/P50X/series015/s015_f00011_ts1500965665_Unpacked.root ~/TestOutput/s015_f11_test.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/MPMAnalysis/V5/PMTGainAnalysis/Core/LS.cfg /projects/prospect/converted_data/Unpacked/P50X/series015/s015_f00012_ts1500969269_Unpacked.root ~/TestOutput/s015_f12_test.h5;
	#Execute .h5 to .root convertor
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/TestOutput/s015_f11_test.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/TestOutput/s015_f12_test.h5;
    
