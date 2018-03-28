# Change contents in "all:"
# For Contents under #Execute PulseCruncher use:
# <Location of PulseCruncher> <PulseCruncher Config File> <Location of InputFile> <Location of Waveform File> <Location to Output/Output File Name>
# For Concents under #Execute .h5 to .root convertor use:
# <Location of Convertor> <Location of .h5>
all:
	#Execute PulseCruncher
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/Standard.cfg /projects/prospect/converted_data/Unpacked/WetCommissioning/series011/s011_f00001_ts1520181395_Unpacked.root ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/s011_f00001_ts1520180835_10.6.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/10.8V.cfg /projects/prospect/converted_data/Unpacked/WetCommissioning/series011/s011_f00002_ts1520181805_Unpacked.root ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/s011_f00002_ts1520181805_10.8.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/11.0V.cfg /projects/prospect/converted_data/Unpacked/WetCommissioning/series011/s011_f00003_ts1520182276_Unpacked.root ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/s011_f00003_ts1520182276_11.0.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/PulseCruncher/./PulseCruncher ~/LightInjection-Analysis/GainAnalysis/11.5V.cfg /projects/prospect/converted_data/Unpacked/WetCommissioning/series011/s011_f00005_ts1520182949_Unpacked.root ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/s011_f00005_ts1520182949_11.5.h5;
	#Execute .h5 to .root convertor
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/CombinedVoltage/s011_f00001_ts1520180835_10.6.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/CombinedVoltage/s011_f00002_ts1520181805_10.8.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/CombinedVoltage/s011_f00003_ts1520182276_11.0.h5;
	~/MPMAnalysis/V6/PROSPECT2x_Analysis/Examples/./DetPulse_HDF5_to_ROOT ~/LightInjection-Analysis/OCS_Scan/WetCommissioning/CombinedVoltage/s011_f00005_ts1520182949_11.5.h5;

