#!/bin/bash

suod stty -F /dev/ttyS0 38400

sudo systemctl disable gpsd.socket

sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock

cgps -s

