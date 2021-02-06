"""
                            Serial capture
"""

import serial
import time

iport = serial.Serial('/dev/ttyUSB0',115200)
data =[]
i=0
j=0
while len(data)<=10000:
    d = iport.readline()
    print(d)
    d_string = d.decode()
    #datastripped = d_string.rstrip()+"\n"
    data.append(d)
    time.sleep(0.1)
    print(len(data))
    i+=1
    if i == 20:
	#print(i)
	i=0
	with open('consensus_node1_f1_d3_dif3.txt','a') as file:
		for j in range(len(data)-20,len(data)):
			#print(j)
			file.writelines(data[j])
