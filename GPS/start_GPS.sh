#!/bin/bash

sudo stty -F /dev/ttyAMA0 38400 

#sudo systemctl disable gpsd.socket
sudo systemctl disable hciuart

sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock

cgps -s

