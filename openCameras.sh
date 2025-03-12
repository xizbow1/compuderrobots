#!/bin/bash

# Starting Camera 1
echo "Opening Camera 1 (rotated)"
nvgstcapture-1.0 --camsrc=0 --orientation 2

# Let Camera 1 start up
sleep 2

# Starting Camera 2
echo "Opening Camera 2 (rotated)"
nvgstcapture-1.0 --camsrc=1 --orientation 2



