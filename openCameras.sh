#!/bin/bash

# Starting Left Camera
echo "Opening Left Camera"
gnome-terminal -- bash -c "nvgstcapture-1.0 --sensorid=0 --orientation 2; exec bash"

# Let Camera 1 start up
sleep 2

# Starting Right Camera 
echo "Opening Right Camera"
gnome-terminal -- bash -c "nvgstcapture-1.0 --sensorid=1 --orientation 2; exec bash"




