#!/bin/bash

<<<<<<< HEAD
sudo stty -F /dev/ttyAMA0 38400

#sudo systemctl disable gpsd.socket
=======
sudo stty -F /dev/ttyS0 38400

#sudo systemctl disable gpsd.socket

>>>>>>> 07fa4993cbc7aca882be6f0a44fb2eb4ee0403f8
sudo systemctl disable hciuart

sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock

cgps -s

