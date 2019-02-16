#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Fri Feb 15 12:00:16 2019

@author: mro
"""
import json
import serial
import time
    
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = 1
)
ser.flush()
ser.flushOutput()
ser.flushInput()

command_str = {'name':'Catrina','init':0,'end':6,'R':255,'G':0,'B':200,'Brightness':0,'mode':22,'mode_time':10}
command_json = json.dumps(command_str)
ser.write(command_json+'\n')



print ser.readline()
print ser.readline()
