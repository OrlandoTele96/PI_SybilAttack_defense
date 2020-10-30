"""
                            Serial capture
"""

import serial
import time

iport = serial.Serial('',11500)
data =[]

while len(data)!=10000:
    d = iport.readline()
    d_string = d.decode()
    datastripped = d_string.rstrip()+"\n"
    data.append(datastripped)
    time.sleep(0.1)

with open("test.txt",'a') as file:
    file.writelines(data)
