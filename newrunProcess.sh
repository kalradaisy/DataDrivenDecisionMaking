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
#g++ -std=gnu++11 -lzmq -o alg1 TrigAlg_firstinstance.cpp
#g++ -std=gnu++11 -lzmq -o alg2 TrigAlg_secondinstance.cpp

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



#./fil2 > fil2.out &

./alg1 > alg1.out &
./alg2 > alg2.out &

./bin2 2 tp2.txt st2.txt time2.txt > tp2.out &
./bin3 3 tp3.txt st3.txt time3.txt > tp3.out &
./bin4 4 tp4.txt st4.txt time4.txt > tp4.out &
./bin5 5 tp5.txt st5.txt time5.txt > tp5.out &
./bin6 6 tp6.txt st6.txt time6.txt > tp6.out &
./bin7 7 tp7.txt st7.txt time7.txt > tp7.out &
./bin8 8 tp8.txt st8.txt time8.txt > tp8.out &
./bin9 9 tp9.txt st9.txt time9.txt > tp9.out &


#./bin2 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb02-2021_10_11_23_23_53-0028554-00000.ubdaq 2 > tp2.out &
#./bin3 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb03-2021_10_11_23_23_53-0028554-00000.ubdaq 3 > tp3.out &
#./bin4 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb04-2021_10_11_23_23_53-0028554-00000.ubdaq 4 > tp4.out &
#./bin5 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb05-2021_10_11_23_23_54-0028554-00000.ubdaq 5 > tp5.out &
#./bin6 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb06-2021_10_11_23_23_53-0028554-00000.ubdaq 6 > tp6.out &
#./bin7 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb07-2021_10_11_23_23_53-0028554-00000.ubdaq 7 > tp7.out &
#./bin8 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb08-2021_10_11_23_23_53-0028554-00000.ubdaq 8 > tp8.out &
#./bin9 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb09-2021_10_11_23_23_50-0028554-00000.ubdaq 9 > tp9.out &

#./bin2 2  time2.txt tp2.txt > tp2.out & 
#./bin3 3  time3.txt tp3.txt > tp3.out &
#./bin4 4  time4.txt tp4.txt > tp4.out &  
#./bin5 5  time5.txt tp5.txt > tp5.out &   
#./bin6 6  time6.txt tp6.txt > tp6.out & 
#./bin7 7  time7.txt tp7.txt > tp7.out &  
#./bin8 8  time8.txt tp8.txt > tp8.out &    
#./bin9 9  time9.txt tp9.txt > tp9.out  


#./bin2 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb02-2021_10_11_23_23_53-0028554-00000.ubdaq 2  time2.txt tp2.txt > tp2.out &
#./bin3 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb03-2021_10_11_23_23_53-0028554-00000.ubdaq 3  time3.txt tp3.txt > tp3.out &
#./bin4 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb04-2021_10_11_23_23_53-0028554-00000.ubdaq 4  time4.txt tp4.txt > tp4.out &
#./bin5 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb05-2021_10_11_23_23_54-0028554-00000.ubdaq 5  time5.txt tp5.txt > tp5.out &
#./bin6 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb06-2021_10_11_23_23_53-0028554-00000.ubdaq 6  time6.txt tp6.txt > tp6.out &
#./bin7 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb07-2021_10_11_23_23_53-0028554-00000.ubdaq 7  time7.txt tp7.txt > tp7.out &
#./bin8 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb08-2021_10_11_23_23_53-0028554-00000.ubdaq 8  time8.txt tp8.txt > tp8.out &
#./bin9 /home/uboonedaq/kalra_seb01/Files/2StreamTest-SN-seb09-2021_10_11_23_23_50-0028554-00000.ubdaq 9  time9.txt tp9.txt > tp9.out 


#./bin2 /home/uboonedaq/kalra_seb01/Files/test2.ubdaq 2 tp2.txt time2.txt > tp2.out &
#./bin3 /home/uboonedaq/kalra_seb01/Files/test3.ubdaq 3 tp3.txt time3.txt > tp3.out &
#./bin4 /home/uboonedaq/kalra_seb01/Files/test4.ubdaq 4 tp4.txt time4.txt > tp4.out &
#./bin5 /home/uboonedaq/kalra_seb01/Files/test5.ubdaq 5 tp5.txt time5.txt > tp5.out &
#./bin6 /home/uboonedaq/kalra_seb01/Files/test6.ubdaq 6 tp6.txt time6.txt > tp6.out &
#./bin7 /home/uboonedaq/kalra_seb01/Files/test7.ubdaq 7 tp7.txt time7.txt > tp7.out &
#./bin8 /home/uboonedaq/kalra_seb01/Files/test8.ubdaq 8 tp8.txt time8.txt > tp8.out &
#./bin9 /home/uboonedaq/kalra_seb01/Files/test9.ubdaq 9 tp9.txt time9.txt > tp9.out



