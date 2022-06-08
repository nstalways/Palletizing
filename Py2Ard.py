# Python과 Arduino를 연결하는 코드

import serial
import time

arduino = serial.Serial('com3', 9600)
time.sleep(1)

print('1을 입력하면 LED ON, 0을 입력하면 LED OFF')

while 1:

    var = input()

    if (var == '1'):
        var = var.encode('utf-8')
        arduino.write(var)
        print('LED turned ON')
        time.sleep(1)

    if (var == '0'):
        var = var.encode('utf-8')
        arduino.write(var)
        print('LED turned OFF')
        time.sleep(1)