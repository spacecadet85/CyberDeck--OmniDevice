
import time
import board
import busio
import csv
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Create the ADC object using the I2C bus
ads = ADS.ADS1115(i2c)

# Create single-ended input on channel 0
# chan = AnalogIn(ads, ADS.P0)

# Create differential input between channel 0 and 1
chan = AnalogIn(ads, ADS.P0, ADS.P3)
voltageDivider = 0.180
lipoVoltage_3s = [9.82, 10.83, 11.06, 11.12, 11.18, 11.24, 11.3, 11.36, 11.39, 11.45, 11.51, 11.56, 11.62, 11.74, 11.86, 11.95, 12.07, 12.25, 12.33, 12.45, 12.6]
lipoPercent_3s = [0 ,5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]
movingAvgBuff = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]
count = 0
avg = 0.000
last_avg = 0.000
interval = 0.01
delta = 0.1
state = ""
print("{:>5}\t{:>5}\t{:>5}".format("voltage", "percent", "state"))

with open('bat.csv', 'w') as f:
    pass

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

    time.sleep(interval)
