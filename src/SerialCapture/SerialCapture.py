"""
                            Serial capture
"""

import serial
import time

iport = serial.Serial('/dev/ttyUSB0',115200)
data =[]

while len(data)!=6000:
    d = iport.readline()
    print(d)
    #d_string = d.decode()
    #datastripped = d_string.rstrip()+"\n"
    data.append(d)
    time.sleep(0.1)
    print(len(data))

with open("RSSI_results2.txt",'wb') as file:
    file.writelines(data)
