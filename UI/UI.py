import sys
import time
from functools import partial

import numpy as np
import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation

import signal
import serial
import serial.tools.list_ports

import tkinter
import tkinter.ttk as ttk

imp_arr = []
od_arr = []

ser = serial.Serial(timeout = 1)
ser.baudrate = 9600

var_read = False

plot = Figure(figsize=(5,5), dpi = 100)
graph = plot.add_subplot(111)

def animate(i):
    graph.clear()

    #radio_var.get() => 1-Imp|2-OD|3-Both

    if(int(radio_var.get())%2==1):
        graph.plot(list(range(len(imp_arr))), imp_arr)
    if(int(radio_var.get())>1):
        graph.plot(list(range(len(od_arr))), od_arr)
    

def sett(port):
    ser.port = port()
    print(ser.port)

def graph_drawing(graph_type):
    while True:
        pass

def start():
    ser.open()
    global var_read
    var_read = True

def stop():
    global var_read
    ser.close()
    var_read = False

class CL(tkinter.Frame):
    '''
    classdocs
    '''  
    def __init__(self, parent):
        '''
        Constructor
        '''
        tkinter.Frame.__init__(self, parent)
        tkinter.Grid.rowconfigure(parent, 0, weight=1)
        tkinter.Grid.columnconfigure(parent, 0, weight=1)
        self.parent=parent
        self.initialize_user_interface()

    def initialize_user_interface(self):
        global radio_var
        """Draw a user interface allowing the user to type
        items and insert them into the treeview
        """
        
        self.parent.title("iGEM IISc - Synshine")
        for i in range(15):
            self.parent.grid_rowconfigure(i,weight=1, pad = 3)
            self.parent.grid_columnconfigure(i,weight=1, pad = 12)
        self.parent.config(background="lavender")
        

        # Define the different GUI widgets
        self.frameo = tkinter.Frame(self.parent, relief = "sunken", width = 1000, height = 1000, borderwidth = 1, bg = "lavender")
        self.frameo.grid(row = 0, rowspan = 3, pady = 10)
        self.comPort_label = tkinter.Label(self.parent, text = "COM_PORT:", bg = 'lavender')
        self.comPort_entry = tkinter.ttk.Combobox(self.parent, values = [i.device for i in serial.tools.list_ports.comports()])
        self.comPort_button = tkinter.Button(self.parent, text = "Set", bg = "lavender", command = partial(sett, partial(self.comPort_entry.get)))
        self.comPort_label.grid(row = 1, column = 1, sticky = tkinter.N)
        self.comPort_entry.grid(row = 1, column = 2, sticky = tkinter.N)
        self.comPort_button.grid(row = 1, column = 3, sticky = tkinter.N)

        self.null = tkinter.Label(self.frameo, text = "", bg = "lavender")
        self.null.grid(row = 1, column = 0)

        self.start = tkinter.Button(self.parent, text = "Start", bg = "lavender", command = partial(start))
        self.stop = tkinter.Button(self.parent, text = "Stop", bg = "lavender", command = partial(stop))
        self.start.grid(row = 5, column = 0, columnspan = 2, pady = 5)
        self.stop.grid(row = 5, column = 1, columnspan = 2, pady = 5)

        values = {"Impedence Graph" : "1",
                  "OD Graph" : "2",
                  "Impedence and OD Graph" : "3"}
        radio_var = tkinter.StringVar(win, "3")

        for index, (text, value) in enumerate(values.items()): 
            tkinter.Radiobutton(self.frameo, text = text, variable = radio_var,  value = value, indicator = 0, background = "light blue").grid(row = index, sticky = tkinter.N+tkinter.S+tkinter.E+tkinter.W)

        

        self.canvas_frame = tkinter.Frame(self.parent, relief = "sunken", borderwidth = 1, bg = "lavender")
        self.canvas_frame.grid(row = 6, rowspan = 4, columnspan = 4, pady = 10, padx = 5)
        self.canvas = FigureCanvasTkAgg(plot, self.canvas_frame)
        self.canvas.draw()
        self.canvas.get_tk_widget().grid(row = 0, column = 0)




win = tkinter.Tk()
win.iconphoto(False, tkinter.PhotoImage(file = "logo.png"))
CL(win)
ani = animation.FuncAnimation(plot, animate)

while True:
    try:
        if(var_read):
            inpt = ser.read_until('\n')
            inpt = str(inpt)[2:-5]
            print(inpt)
            if(inpt!=''):
                temp1, temp2 = inpt.split(',')
                temp1 = int(temp1)
                temp2 = int(temp2)
                imp_arr.append(temp1)
                od_arr.append(temp2)
        win.update_idletasks()
        win.update()
    except tkinter.TclError:
        break
