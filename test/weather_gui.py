import tkinter as tk
from tkinter import ttk
import time
import board
import adafruit_bmp280

#this space for sensor daemon----------------------
def read_BMP280():
    # Create sensor object, communicating over the board's default I2C bus
    i2c = board.I2C()  # uses board.SCL and board.SDA
    bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c)

    # change this to match the location's pressure (hPa) at sea level
    bmp280.sea_level_pressure = 1008.47
    bmp280.mode = adafruit_bmp280.MODE_NORMAL
    bmp280.standby_period = adafruit_bmp280.STANDBY_TC_500
    bmp280.iir_filter = adafruit_bmp280.IIR_FILTER_X16
    bmp280.overscan_pressure = adafruit_bmp280.OVERSCAN_X16
    bmp280.overscan_temperature = adafruit_bmp280.OVERSCAN_X2
    # The sensor will need a moment to gather inital readings
    time.sleep(1)

    while True:
        temperature = 
        print("\nTemperature: %0.1f C" % bmp280.temperature)
        print("Pressure: %0.1f hPa" % bmp280.pressure)
        print("Altitude = %0.2f meters" % bmp280.altitude)
        time.sleep(2)

task = threading.Thread(target=read_BMP280, daemon=True)
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

tempvar = tk.DoubleVar()
pressvar = tk.DoubleVar()
altvar = tk.DoubleVar()

root.mainloop()