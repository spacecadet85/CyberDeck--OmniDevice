#!/bin/bash

sudo stty -F /dev/ttyS0 9600 

sudo systemctl disable gpsd.socket

sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock

cgps -s

