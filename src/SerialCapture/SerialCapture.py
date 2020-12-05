"""
                            Serial capture
"""

import serial
import time

iport = serial.Serial('/dev/ttyUSB0',115200)
data =[]
i=0
while len(data)<=1000:
    d = iport.readline()
    print(d)
    d_string = d.decode()
    #datastripped = d_string.rstrip()+"\n"
    data.append(d)
    time.sleep(0.1)
    #print(len(data))

with open("pow_node1_test.txt",'wb') as file:
    file.writelines(data)
