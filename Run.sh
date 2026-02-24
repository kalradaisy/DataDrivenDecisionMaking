#!/bin/bash

outfile="cpu_power_cpu.txt"
prev=$(cat /sys/class/powercap/intel-rapl:0/energy_uj)

(
    while true; do
        sleep 1
        curr=$(cat /sys/class/powercap/intel-rapl:0/energy_uj)
        power=$(echo "scale=6; ($curr - $prev)/1000000" | bc)
        
        # total CPU usage %
        cpu_usage=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}')  # user + system
        
        # log timestamp, power (W), and CPU %
        echo "$(date +%s) $power $cpu_usage" >> "$outfile"
        
        prev=$curr
    done
) &
log_pid=$!

# -------------------------------
# COMPILE BINARIES
# -------------------------------

g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin2 ReadStitch_2crate.cpp #ReadBin_2crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin3 ReadStitch_3crate.cpp #ReadBin_3crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin4 ReadStitch_4crate.cpp #ReadBin_4crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin5 ReadStitch_5crate.cpp #ReadBin_5crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin6 ReadStitch_6crate.cpp #ReadBin_6crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin7 ReadStitch_7crate.cpp #ReadBin_7crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin8 ReadStitch_8crate.cpp #ReadBin_8crate_disk_newzmq.cpp
g++ -std=gnu++11 -lzmq -lboost_filesystem -lboost_system -o bin9 ReadStitch_9crate.cpp #ReadBin_9crate_disk_newzmq.cpp

g++ -std=gnu++11 -lzmq -o alg1 TrigAlg_Check1.cpp
g++ -std=gnu++11 -lzmq -o alg2 TrigAlg_Check2.cpp

g++ -std=gnu++11 -lzmq -o fil12 filter1_2.cpp
g++ -std=gnu++11 -lzmq -o fil13 filter1_3.cpp
g++ -std=gnu++11 -lzmq -o fil14 filter1_4.cpp
g++ -std=gnu++11 -lzmq -o fil15 filter1_5.cpp
g++ -std=gnu++11 -lzmq -o fil16 filter1_6.cpp
g++ -std=gnu++11 -lzmq -o fil17 filter1_7.cpp
g++ -std=gnu++11 -lzmq -o fil18 filter1_8.cpp
g++ -std=gnu++11 -lzmq -o fil19 filter1_9.cpp

g++ -std=gnu++11 -lzmq -o fil22 filter2_2.cpp
g++ -std=gnu++11 -lzmq -o fil23 filter2_3.cpp
g++ -std=gnu++11 -lzmq -o fil24 filter2_4.cpp
g++ -std=gnu++11 -lzmq -o fil25 filter2_5.cpp
g++ -std=gnu++11 -lzmq -o fil26 filter2_6.cpp
g++ -std=gnu++11 -lzmq -o fil27 filter2_7.cpp
g++ -std=gnu++11 -lzmq -o fil28 filter2_8.cpp
g++ -std=gnu++11 -lzmq -o fil29 filter2_9.cpp

#g++ -std=gnu++11 -lzmq -o fil2 filter2.cpp                                                                                                                                            
# -------------------------------
# RUN PROCESSES IN BACKGROUND
# -------------------------------

./fil12 > fil12.out &
./fil13 > fil13.out &
./fil14 > fil14.out &
./fil15 > fil15.out &
./fil16 > fil16.out &
./fil17 > fil17.out &
./fil18 > fil18.out &
./fil19 > fil19.out &


./fil22 > fil22.out &
./fil23 > fil23.out &
./fil24 > fil24.out &
./fil25 > fil25.out &
./fil26 > fil26.out &
./fil27 > fil27.out &
./fil28 > fil28.out &
./fil29 > fil29.out &


./alg1 > alg1.out &
./alg2 > alg2.out &

./bin9 9 tp9.txt st9.txt time9.txt wv9.txt 20 50 16 > tp9.out &
./bin2 2 tp2.txt st2.txt time2.txt wv2.txt 20 50 16 > tp2.out &
./bin3 3 tp3.txt st3.txt time3.txt wv3.txt 20 50 16 > tp3.out &
./bin4 4 tp4.txt st4.txt time4.txt wv4.txt 20 50 16 > tp4.out &
./bin5 5 tp5.txt st5.txt time5.txt wv5.txt 20 50 16 > tp5.out &
./bin6 6 tp6.txt st6.txt time6.txt wv6.txt 20 50 16 > tp6.out &
./bin7 7 tp7.txt st7.txt time7.txt wv7.txt 20 50 16 > tp7.out &
./bin8 8 tp8.txt st8.txt time8.txt wv8.txt 20 50 16 > tp8.out &




