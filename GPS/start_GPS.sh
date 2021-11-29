#!/bin/bash

sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock -n

cgps -s

