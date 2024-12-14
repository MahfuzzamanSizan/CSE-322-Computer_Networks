#!/bin/bash
./ns3 run "scratch/1905054_1.cc --nWifi=20 --numFlow=10 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=40 --numFlow=20 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=60 --numFlow=30 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=80 --numFlow=40 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"



./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_1.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"


./ns3 run "scratch/1905054_2.cc --nWifi=20 --nnumFlow=10 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=40 --numFlow=20 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=60 --numFlow=30 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=80 --numFlow=40 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"



./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"
./ns3 run "scratch/1905054_2.cc --nWifi=100 --numFlow=50 --nPackets=20 --speed=10 --coverageArea=2"

echo "Hello"

gnuplot -e "set output 'scratch/1905054_output/1905054_output_1.png';\
            
            plot 'scratch/1905054_output/delay.dat' using 1:2 with lines title 'wiegweuifwweuif'"


        