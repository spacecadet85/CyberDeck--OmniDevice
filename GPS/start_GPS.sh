#!/bin/bash

sudo stty -F /dev/ttyAMA1 38400
sudo systemctl disable gpsd.socket
sudo systemctl disable hciuart

sudo gpsd /dev/ttyAMA1 -F /var/run/gpsd.sock

cgps -s

