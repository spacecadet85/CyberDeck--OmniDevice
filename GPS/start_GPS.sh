#!/bin/bash

sudo stty -F /dev/ttyS0 38400

#sudo systemctl disable gpsd.socket

sudo systemctl disable hciuart

sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock

cgps -s

