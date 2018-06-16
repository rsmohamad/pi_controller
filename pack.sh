#!/bin/sh

mkdir software
rm -f software.zip
make clean && sudo make -j4 -o3
cp pi_controller install.sh lib* *.csv software/
zip software.zip -r software/
rm -rf software

