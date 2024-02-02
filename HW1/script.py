import subprocess
import re
from datetime import datetime
#clkPeriod, coreUtil, congEffor, timingDriven, drcViol, slack, totalWireLength, chipArea

timing_range = range(1, 11)
core_util = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
congEff = ["low", "medium", "high"]
timeDriv = [0, 1]
for t in timing_range:
	sdc_orig = open("./design_orig.sdc", "r")
	sdc = open("./design.sdc", "w")
	for line in sdc_orig.readlines():
		if(re.match(".*waveform.*", line)):
			newLine = 'create_clock [get_ports clk] -name CLK -period ' + str(float(t)) + ' -waveform {0 ' + str(float(t)/2) +'}\n'
			print(newLine)
			sdc.write(newLine)
		else:
			sdc.write(line)
	sdc.close()
	sdc_orig.close()
	for util in core_util:
		for cong in congEff:
			for timingOn in timeDriv:
				now = datetime.now()
				currentTime = now.strftime("%H:%M:%S")
				log = open("./result.log", "a")
				log.write(currentTime + "\n")
				log.write("paramters: \n" + "core utilization: " + str(util) + "\n")
				log.write("clock period:" + str(t) + "\n")
				log.write("congestionEffort: " + str(cong) + "\n")
				log.write("timiingdriven: " + str(timingOn) + "\n")
				apr_orig = open("./apr_orig.tcl", "r")
				apr = open("./apr.tcl", "w")
				for line in apr_orig.readlines():
					if(re.match("^floorPlan.*", line)):
						line = 'floorPlan -coreMarginsBy die -site FreePDK45_38x28_10R_NP_162NW_34O -r 1.0 ' + str(util) + ' 4.0 4.0 4.0 4.0\n'
						print(line)
					if(re.match(".*congEffort.*", line)):
						line = 'setPlaceMode -congEffort ' + cong +' -timingDriven ' + str(timingOn) + ' -clkGateAware 1 -powerDriven 0 -ignoreScan '
						line += '1 -reorderScan 1 -ignoreSpare 0 -placeIOPins 1 -moduleAwareSpare 0 -preserveRouting 1 -rmAffectedRouting 0 '
						line += '-checkRoute 0 -swapEEQ 0\n'
						print(line)
					apr.write(line)
				apr.close()
				apr_orig.close()
				#run aprline
				subprocess.call(['innovus -files apr.tcl -batch -no_gui'], shell = True, cwd = './')
				summaryrpt = open("./summary.rpt", "r")
				timingrpt = open("./timing.rpt", "r")
				drcrpt = open("./drc.rpt", "r")
				result = open("./result.csv", "a")
				resultString = str(t) + ',' + str(util) + ',' + str(cong) + ',' + str(timingOn)  
				log.write("result:\n")
				for line in drcrpt:
					if(re.match(".*Verification Complete", line)):
						resultString += ',' + line.split()[-2]
						log.write(line.strip())
						log.write("\n")
						#print(line.split())
				for line in timingrpt:
					if(re.match(".*Slack Time", line)):
						resultString += ',' + line.split()[-1]
						log.write(line.split()[1] + " " + line.split()[2] + " " + line.split()[3] + "\n")
						print(line.split())
				for line in summaryrpt:
					if(re.match("Total wire length", line)):
						resultString += ',' + line.split()[-2]
						log.write(line)
						#print(line.split())
					if(re.match("Total area of Chip", line)):
						resultString += ',' + line.split()[-2]
						log.write(line)
						#print(line.split())
				log.write("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n")
				resultString += '\n'
				print(resultString)
				result.write(resultString)
				log.close()
				result.close()
