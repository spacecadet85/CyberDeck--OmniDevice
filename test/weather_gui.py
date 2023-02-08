import tkinter as tk
from tkinter import ttk
from tkinter.ttk import Label
import time
import board
from adafruit_bme280 import basic as adafruit_bme280
import threading
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
import busio
import csv

is_on = True
logger_is_on = False

#this space for sensor daemon----------------------
def read_BME280():
    # Create sensor object, communicating over the board's default I2C bus
    bme280_address = '76'
    i2c = board.I2C()  # uses board.SCL and board.SDA
    bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, int(bme280_address, 16))

    # change this to match the location's pressure (hPa) at sea level
    bme280.sea_level_pressure = 1030.82
    #bme280.mode = adafruit_bme280.MODE_NORMAL
    #bme280.standby_period = adafruit_bme280.STANDBY_TC_500
    #bme280.iir_filter = adafruit_bme280.IIR_FILTER_X16
    #bme280.overscan_pressure = adafruit_bme280.OVERSCAN_X16
    #bme280.overscan_temperature = adafruit_bme280.OVERSCAN_X2
    # The sensor will need a moment to gather inital readings
    time.sleep(1)

    while True:
        temperature = bme280.temperature
        pressure = bme280.pressure
        altitude = bme280.altitude
        temp_des = "C"
        press_des = "hPa"
        alt_des = "meters"

        #convert to imperial on false
        if is_on == False:
            temperature = temperature * 9/5+32
            pressure = pressure * 1/33.863886666667
            altitude = altitude * 3.28084
            temp_des = "F"
            press_des = "inHg"
            alt_des = "ft"

        tempvar.set('Temperature: {0:.1f} {1}'.format(temperature, temp_des))
        pressvar.set('Pressure: {0:.2f} {1}'.format(pressure, press_des))
        altvar.set('Altitude: {0:.2f} {1}'.format(altitude, alt_des))   

        print("\nTemperature: %0.1f C" % temperature)
        print("Pressure: %0.1f hPa" % pressure)
        print("Altitude = %0.2f meters" % altitude)
        time.sleep(2)

def batt_monitor():

    #ads1115_address = 
    # Create the I2C bus
    i2c = busio.I2C(board.SCL, board.SDA)

    # Create the ADC object using the I2C bus
    ads = ADS.ADS1115(i2c)

    chan = AnalogIn(ads, ADS.P0, ADS.P3)
    voltageDivider = 0.180
    lipoVoltage_3s = [9.82, 10.83, 11.06, 11.12, 11.18, 11.24, 11.3, 11.36, 11.39, 11.45, 11.51, 11.56, 11.62, 11.74, 11.86, 11.95, 12.07, 12.25, 12.33, 12.45, 12.6]
    lipoPercent_3s = [0 ,5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]
    movingAvgBuff = [0,0,0,0,0,0,0,0,0,0]
    count = 0
    percent = 0.00
    avg = 0.000
    last_avg = 0.000
    interval = 0.5
    delta = 0.1
    state = ""
    file_toggle = True
    fileheader = ['Time','Voltage']
    

    #with open('bat.csv', 'w') as f:
        #pass

    def get_avg():
        count = 0
        movingAvgBuff = [0,0,0,0,0,0,0,0,0,0]
        while count > 10:
            movingAvgBuff[count] = chan.voltage / voltageDivider
            count = count + 1
        return sum(movingAvgBuff)/len(movingAvgBuff)

    while True:
        batvoltage = get_avg()
        seconds = time.time()
        output = [seconds, batvoltage]

        for i in range(len(lipoVoltage_3s)-1):
            if lipoVoltage_3s[i] <= batvoltage and lipoVoltage_3s[i+1] > batvoltage:
                percent = lipoPercent_3s[i]+(avg-lipoVoltage_3s[i])*(lipoPercent_3s[i+1]-lipoPercent_3s[i])/(lipoVoltage_3s[i+1]-lipoVoltage_3s[i])
        
        #file logger if button is turned on 
        if logger_is_on:
            if file_toggle:
                with open('battery.csv', 'w') as file:
                    writer = csv.write(file)
                    writer.writerow(fileheader)
                file_toggle = False

            log = [time.time(), batvoltage]
            with open('battery.csv', 'w', newline='') as file:
                writer = csv.write(file)
                writer.writerow(output)


        print("{:>5}\t{:>5}".format("battery Volts", "percent"))        
        print("{:>5.3f}\t{:>5.3f}".format(batvoltage, percent))
        batvoltagevar.set('Batt Voltage: {0:.2f} V'.format(batvoltage))
        batpercentvar.set('Percent: {0:.1f} %'.format(percent))

        time.sleep(interval)

BME280_Handler = threading.Thread(target=read_BME280, daemon=True)
batt_Handler = threading.Thread(target=batt_monitor, daemon=True)
#--------------------------------------------------

root = tk.Tk()
root.geometry('400x300')
root.title('Recover Deck V1.0 Info Display')

#message = tk.Label(root, text="Recover Deck Info")
#message.pack()

#create a notebook
notebook = ttk.Notebook(root)
notebook.pack(pady=10, expand=True)

# create frames
frame1 = ttk.Frame(notebook, width=400, height=280)
frame2 = ttk.Frame(notebook, width=400, height=280)
frame3 = ttk.Frame(notebook, width=400, height=280)

frame1.pack(fill='both', expand=True)
frame2.pack(fill='both', expand=True)
frame3.pack(fill='both', expand=True)

# add frames to notebook
notebook.add(frame1, text='General Information')
notebook.add(frame2, text='Enviroment')
notebook.add(frame3, text='Battery')

#shared variables
tempvar = tk.StringVar()
pressvar = tk.StringVar()
altvar = tk.StringVar()
batvoltagevar = tk.StringVar()
batpercentvar = tk.StringVar()

# Define Our Images
#on = PhotoImage(file="on.png")
#off = PhotoImage(file="off.png")

#button var

#button_txt = 'Metric'

#functions
def switch():
    global is_on

    # Determine is on or off
    if is_on:
        sys_of_msmt_button.config(text= 'Metric')
        is_on = False
    else:
       
        sys_of_msmt_button.config(text = 'Imperial')
        is_on = True

def v_logger_switch():
    global logger_is_on

    # Determine is on or off
    if logger_is_on:
        v_logger_button.config(text= 'On')
        logger_is_on = False
    else:
       
        v_logger_button.config(text = 'Off')
        logger_is_on = True

#buttons
sys_of_msmt_button = ttk.Button(root, text='Metric', command=switch)
sys_of_msmt_button.pack(in_=frame2, ipadx=10)

v_logger_button = ttk.Button(root, text='Off', command=v_logger_switch)
v_logger_button.pack(in_=frame3, ipadx=10)

#labels
templabel = tk.Label(notebook, textvariable= tempvar)
templabel.pack(in_=frame2, expand=True, ipadx=10)

presslabel = tk.Label(notebook, textvariable=pressvar)
presslabel.pack(in_=frame2, expand=True, ipadx=10)

altlabel = tk.Label(notebook, textvariable=altvar)
altlabel.pack(in_=frame2, expand=True, ipadx=10)

voltlabel = tk.Label(notebook, textvariable=batvoltagevar)
voltlabel.pack(in_=frame3, expand=True, ipadx=10)

perclabel = tk.Label(notebook, textvariable=batpercentvar)
perclabel.pack(in_=frame3, expand=True, ipadx=10)

#start threads
BME280_Handler.start()
batt_Handler.start()

root.mainloop()