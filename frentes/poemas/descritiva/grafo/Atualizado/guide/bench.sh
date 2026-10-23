# !/bin/bash

graphType="mesh"
durchlaeufe=30
height=100
width=40
scale=3
newVersion="/home/student/maagv/hiwi/guide/main"
oldVersion="/home/student/maagv/hiwi/guide.orginal/main"
param="-# $height -d 3 -g $graphType -i 4 -T $width -l 1 -b 1 -s 0 -r 20  -R 20 -1 0  -2 0 -C 0 -S 3.0 -h 0.17 -D 0"

erg=0
for (( i=0 ; i < $durchlaeufe; i++ )) ; do
    temp=$($oldVersion $param 2>&1 | grep "User time=" | cut -d "=" -f 2 | cut -d s -f 1)
    erg=$(echo "scale=$scale+1; $erg+$temp" | bc)
done
echo -n Durschnitt der alten Version:
echo "scale=$scale; $erg/$durchlaeufe" | bc

erg=0
for (( i=0 ; i < $durchlaeufe; i++ )) ; do
    temp=$($newVersion $param 2>&1 | grep "User time=" | cut -d "=" -f 2 | cut -d s -f 1)
    erg=$(echo "scale=$scale+1; $erg+$temp" | bc)
done
echo -n Durschnitt der neue Version:
echo "scale=$scale; $erg/$durchlaeufe" | bc

