# Change contents in "all:"
# For Contents under #Execute PulseCruncher use:
# <Location of PulseCruncher> <PulseCruncher Config File> <Location of InputFile> <Location of Waveform File> <Location to Output/Output File Name>
# For Concents under #Execute .h5 to .root convertor use:
# <Location of Convertor> <Location of .h5>
all:
	#Execute PulseCruncher
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/LI_PulseCruncher.cfg /projects/prospect/converted_data/Unpacked/DryCommissioning/series011/s011_f00004_ts1516033038_Unpacked.root ~/LightInjection-Analysis/OCS/series011/f00004_-13.0_GainOutput/s011_f00004_ts1516033038_OCS.h5;

	#Execute .h5 to .root convertor
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS/series011/f00004_-13.0_GainOutput/s011_f00004_ts1516033038_OCS.h5;

    
