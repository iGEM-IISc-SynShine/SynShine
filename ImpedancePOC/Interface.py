import serial
import sys
import time
# Initialize Comms
ser=serial.Serial(input(),baudrate=9600,timeout=1)

#some functions we might need for sending stuff from the computer to the board
def convLongStr(num):
    return ''.join([chr(int(char,base=16)) for char in hex(num).strip('0x')])

def convStrLong(Str):
    return int('0x'+''.join([hex(ord(char)).strip('0x') for char in Str]),base=16)

def __xor__(arr):
    if len(arr) is 0:
        return None
    sum=0
    for i in range(len(arr)):
        sum^=arr[i]
    return sum

def serialListenError():
    if ser.in_waiting>0:
        ser.reset_input_buffer()
    ser.write("ERROR".encode('ascii'))
##################################################################################
#Initialization functions - Only with the very basic functionalities!!!
def setfr(num):
    sendMsg('SETFR'+convLongStr(num))

def cgstp(num):
    sendMsg('CGSTP'+convLongStr(num))

def mlstp(num):
    sendMsg('MLSTP'+convLongStr(num))

def chkcf(): #Should'nt this be CHCKF?????
    sendMsg('CHKCF')

def genlo():
    sendMsg('GENLO')

def error(msg):
    sendMsg('ERROR'+msg)

def setgn(num):
    sendMsg('ERROR'+convLongStr(num))

##################################################################################
#Functions for parsing packets, along with the verification protocol


def sendMsg(msg):
    a=chr(__xor__([ord(char) for char in msg]))
    b=chr(sum([ord(char) for char in msg])%256)

    while(True): 
        ser.reset_input_buffer()
        ser.write(("ST"+msg+a+b+'\r\n').encode('ascii'))
        time.sleep(0.01)
        if ser.in_waiting:
            FL=ser.read()
            if FL=='C':
                if ser.read()=='N':
                    if ser.read()=='F':
                        if ser.read()==a:
                            break
            elif FL=='E':
                ser.reset_input_buffer()
                sendMsg(msg)
            else:
                return None

        ser.reset_input_buffer()

def msgReceived(msg):
    if (msg[0:5]=='SDDAT'):
        pass #Worry about this later.
    elif (msg[0:5]=='ERROR'):
        pass #Decide upon its contents later.  

def listener():
    ser.flush()
    if ser.in_waiting>0:
        run=True
        if ser.in_waiting>0:
            run = True
        if ser.read()=='S':
            if ser.read()=='T':
                Str=''
                while (run and ser.in_waiting):
                    c=ser.read()
                    if c!='\r':
                        Str+=c
                    else:
                        run=False
                        ser.read_until('\n')
                        if c is '\n':
                            run = False
                if run is True:
                    serialListenError()
                    return None
                n=len(Str)
                msg=Str[0:n-2]
                a=chr(__xor__([ord(char) for char in msg]))
                b=chr(sum([ord(char) for char in msg])%256)
                if (Str[n-1]==b) and (Str[n-2]==a):
                    ser.write(('CNF'+a).encode('ascii'))
                # The msg received is a bit different. Will implement later
                    #msgReceived()
                    return None
                else:
                    serialListenError()
            else:
                serialListenError()
                return None    
##################################################################################
while True:
    listener()
    time.sleep(0.01)
