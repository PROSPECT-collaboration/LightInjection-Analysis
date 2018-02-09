# Change contents in "all:"
# For Contents under #Execute PulseCruncher use:
# <Location of PulseCruncher> <PulseCruncher Config File> <Location of InputFile> <Location of Waveform File> <Location to Output/Output File Name>
# For Concents under #Execute .h5 to .root convertor use:
# <Location of Convertor> <Location of .h5>
all:
	#Execute PulseCruncher
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/LI1.cfg /projects/prospect/converted_data/Unpacked/DryCommissioning/series011/s011_f00000_ts1516031237_Unpacked.root ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00000_ts1516031237_V1.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/LI2.cfg /projects/prospect/converted_data/Unpacked/DryCommissioning/series011/s011_f00001_ts1516031844_Unpacked.root ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00001_ts1516031844_V2.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/LI3.cfg /projects/prospect/converted_data/Unpacked/DryCommissioning/series011/s011_f00002_ts1516032223_Unpacked.root ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00002_ts1516032223_V3.h5;
	#Execute .h5 to .root convertor
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00000_ts1516031237_V1.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00001_ts1516031844_V2.h5;
	~/MPMAnalysis/V5/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS/series011/Combine/s011_f00002_ts1516032223_V3.h5;
    
