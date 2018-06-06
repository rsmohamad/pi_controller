#!/bin/sh

sudo chown root:root pi_controller
sudo chmod 4755 pi_controller

sudo cp libts* /usr/lib/arm-linux-gnueabihf
sudo cp libGLESv2.so libEGL.so /opt/vc/lib
sudo ldconfig

