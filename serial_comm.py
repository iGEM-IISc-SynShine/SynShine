import sys
import time
import signal
import serial

ser = serial.Serial()
ser.baudrate = 9600
ser.port = input('Enter the Port\n')
ser.open()

file = open('Arduino_Serial_Data.csv', 'w')

def prog_exit(signal, frame):
    print('CONNECTION CLOSED')
    file.close()
    sys.exit(0)


counter = 0

while True:
    inpt = ser.readline()
    if(counter < 10):
        counter += 1
        continue
    inpt = str(inpt)[2:-5]
    file.write(str(time.strftime("%H:%M:%S", time.localtime()))+','+inpt+'\n')
    signal.signal(signal.SIGINT, prog_exit)
