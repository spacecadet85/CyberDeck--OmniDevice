# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

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
voltageDivider = 0.179
movingAvgBuff = [0,0,0,0,0,0,0,0,0,0]
count = 0
avg = 0.000

print("{:>5}\t{:>5}\t{:>5}\t{:>5}".format("raw", "v", "bat voltage","moving avg"))

with open('bat.csv', 'w') as f:
    pass

while True:
    batvoltage = chan.voltage / voltageDivider
    movingAvgBuff[count] = batvoltage
    output = [chan.value, chan.voltage, batvoltage, avg]
#    print (movingAvgBuff)
    count = count + 1
    if count >= 10:
        avg = sum(movingAvgBuff)/len(movingAvgBuff)
        with open('bat.csv', 'a', newline='\n') as f:
            write = csv.writer(f)
            write.writerow(output)
        count = 0
    print("{:>5}\t{:>5.3f}\t{:5.3f}\t{:5.3f}".format(chan.value, chan.voltage, batvoltage, avg))

    time.sleep(1)
