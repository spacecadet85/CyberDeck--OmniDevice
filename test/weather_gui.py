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

        tempvar.set('Temperature: {0:.1f} C'.format(temperature))
        pressvar.set('Pressure: {0:.2f} hPa'.format(pressure))
        altvar.set('Altitude: {0:.2f} meters'.format(altitude))

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
    movingAvgBuff = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]
    count = 0
    percent = 0.00
    avg = 0.000
    last_avg = 0.000
    interval = 0.5
    delta = 0.1
    state = ""
    print("{:>5}\t{:>5}\t{:>5}".format("voltage", "percent", "state"))

    #with open('bat.csv', 'w') as f:
        #pass

    while True:
        batvoltage = chan.voltage / voltageDivider
        movingAvgBuff[count] = batvoltage
        output = [chan.value, chan.voltage, batvoltage, avg]
        #print (movingAvgBuff)
        count = count + 1
        if count >= 20:

            #calc average voltage
            avg = sum(movingAvgBuff)/len(movingAvgBuff)
            #with open('bat.csv', 'a', newline='\n') as f:
                #write = csv.writer(f)
                #write.writerow(output)
            count = 0
            #determine percentage using linear interpolation
            #find indexes
            for i in range(len(lipoVoltage_3s)-1):
                if lipoVoltage_3s[i] <= avg and lipoVoltage_3s[i+1] > avg:
                    percent = lipoPercent_3s[i]+(avg-lipoVoltage_3s[i])*(lipoPercent_3s[i+1]-lipoPercent_3s[i])/(lipoVoltage_3s[i+1]-lipoVoltage_3s[i])

            #determine the charge state (charge, discarge, charge complete)
            #if moving average increases greater than delta 
            if abs(last_avg - avg) > delta and avg > last_avg:
                state = "charging"
                last_avg = avg
                #break
            #if moving average decrease more than delta then discharge
            elif abs(last_avg - avg) > delta and avg < last_avg:
                state = "discharging"
                last_avg = avg
                #break
            elif avg > 12.6:
            #if greater thatn 12.6 volts 
                state = "charged"
                #break

        print("{:>5.3f}\t{:>5.3f}\t{}".format(avg, percent, state))
        batvoltagevar.set('Batt Voltage: {0:.2f} V'.format(avg))
        batpercentvar.set('Percent: {0:f} %'.format(percent))

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
#ttk.Label(frame2, textvariable= pressvar)
#ttk.Label(frame2, textvariable= altvar)
#ttk.Label(frame3, textvariable= batvoltagevar)
#ttk.Label(frame3, textvariable= batpercentvar)

#start threads
BME280_Handler.start()
batt_Handler.start()

root.mainloop()