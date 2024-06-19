#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string.h>
#include <sstream>
#include <math.h>
#include <map>
#include <tuple>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <sys/time.h>
#include <cstdint>

#include <chrono>

#include <zmq.hpp>
#include <unistd.h>

using namespace std;

struct InfoForAlg {

  uint32_t frame = {0};
  uint32_t fem   = {0};
  uint32_t channel  = {0};
  uint16_t time_start  = {0};
  int       time_over_threshold   = {0};
  uint64_t adc_peak   = {0};
  uint64_t adc_integral= {0};
  int driftRegion  = {0};
  int  crateid = {0};
  int runa={0};
  int subruna = {0};

  InfoForAlg(uint32_t fr4a, uint32_t fma, uint32_t mca, uint16_t tta, int tot1a, uint64_t ama, uint64_t ina, int tdda, int detidda, int ra, int sra)

  {

    frame = fr4a;
    fem = fma;
    channel = mca;
    time_start = tta;
    time_over_threshold  = tot1a;
    adc_peak = ama;
    adc_integral = ina;
    driftRegion  = tdda;
    crateid = detidda;
    runa = ra;
    subruna = sra;
}


};
struct TriggerPrimitive {

  uint32_t frame = {0};
  uint32_t fem   = {0};
  uint32_t channel  = {0};
  uint16_t time_start  = {0};
  int       time_over_threshold   = {0};
  uint64_t adc_peak   = {0};
  uint64_t adc_integral= {0};
  int driftRegion         = {0};
  int crateid            = {0};
  int candidateBool       = {0};
  int run = {0};
  int subrun = {0};

  TriggerPrimitive(uint32_t fr4a, uint32_t fma, uint32_t mca, uint16_t tta, int tot1a, uint64_t ama, uint64_t ina, int driftRegion_,int crateid_,int candidateBool_, int rra, int srra)

  {
    frame = fr4a;
    fem = fma;
    channel = mca;
    time_start = tta;
    time_over_threshold  = tot1a;
    adc_peak = ama;
    adc_integral = ina;
    driftRegion = driftRegion_;
    crateid = crateid_;
    candidateBool = candidateBool_;
    run = rra;
    subrun = srra;

  }

};

//bool compare_channel(const TriggerPrimitive& ch_a, const TriggerPrimitive& ch_b)                                                                            
bool compare_channel(const InfoForAlg&ch_a, const InfoForAlg& ch_b)
{
  // smallest comes first                                                                                                                                     
  return ch_a.channel < ch_b.channel ; // and (ch_a.time_start < ch_b.time_start));                                                                           
}

int getIndex(vector<int> v, int K){
  auto it = find(v.begin(), v.end(), K);
  if (it != v.end())
    {
      int index = it - v.begin();
      //std::cout << index << std::endl;                                                                                                                      
      return index;
    }
  else {
    //std::cout << "-1" << std::endl;                                                                                                                         
    return -99;
  }
}

int main()
{

  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.bind("tcp://127.0.0.1:7611");
  //  std::cout << "1" << std::endl; 
 /*
  zmq::context_t context3(1);
  zmq::socket_t publisher3(context3, ZMQ_PUB);
  publisher3.bind("tcp://127.0.0.1:7622");

  zmq::context_t context4(1);
  zmq::socket_t publisher4(context4, ZMQ_PUB);
  publisher4.bind("tcp://127.0.0.1:7623");

  zmq::context_t context5(1);
  zmq::socket_t publisher5(context5, ZMQ_PUB);
  publisher5.bind("tcp://127.0.0.1:7624");

  zmq::context_t context6(1);
  zmq::socket_t publisher6(context6, ZMQ_PUB);
  publisher6.bind("tcp://127.0.0.1:7625");

  zmq::context_t context7(1);
  zmq::socket_t publisher7(context7, ZMQ_PUB);
  publisher7.bind("tcp://127.0.0.1:7626");

  zmq::context_t context8(1);
  zmq::socket_t publisher8(context8, ZMQ_PUB);
  publisher8.bind("tcp://127.0.0.1:7627");

  zmq::context_t context9(1);
  zmq::socket_t publisher9(context9, ZMQ_PUB);
  publisher9.bind("tcp://127.0.0.1:7628");
  */
  //we need subscriber to receive messages                                                                                                                    
  zmq::context_t contextta2(1);
  zmq::socket_t subscriberta2(contextta2, ZMQ_SUB);
  subscriberta2.connect("tcp://127.0.0.1:7650");
  subscriberta2.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //std::cout << "connect socket sub crate 2" << std::endl;                                                                                                 


  zmq::context_t contextta3(1);
  zmq::socket_t subscriberta3(contextta3, ZMQ_SUB);
  subscriberta3.connect("tcp://127.0.0.1:7730");
  subscriberta3.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //std::cout << "connect socket sub crate 3" << std::endl;                                                                                                   


  zmq::context_t contextta4(1);
  zmq::socket_t subscriberta4(contextta4, ZMQ_SUB);
  subscriberta4.connect("tcp://127.0.0.1:7740");
  subscriberta4.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //std::cout << "connect socket sub crate 4" << std::endl;                                                                                                  

  zmq::context_t contextta5(1);
  zmq::socket_t subscriberta5(contextta5, ZMQ_SUB);
  subscriberta5.connect("tcp://127.0.0.1:7657");
  subscriberta5.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  // std::cout << "connect socket sub crate 5" << std::endl;                                                                                                  

  zmq::context_t contextta6(1);
  zmq::socket_t subscriberta6(contextta6, ZMQ_SUB);
  subscriberta6.connect("tcp://127.0.0.1:7760");
  subscriberta6.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  // std::cout << "connect socket sub crate 6" << std::endl;                                                                                                  

  zmq::context_t contextta7(1);
  zmq::socket_t subscriberta7(contextta7, ZMQ_SUB);
  subscriberta7.connect("tcp://127.0.0.1:7770");
  subscriberta7.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  // std::cout << "connect socket sub crate 7 " << std::endl;                                                                                                 

  zmq::context_t contextta8(1);
  zmq::socket_t subscriberta8(contextta8, ZMQ_SUB);
  subscriberta8.connect("tcp://127.0.0.1:7780");
  subscriberta8.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //    std::cout << "connect socket sub crate 8" << std::endl;                                                                                                 

  zmq::context_t contextta9(1);
  zmq::socket_t subscriberta9(contextta9, ZMQ_SUB);
  subscriberta9.connect("tcp://127.0.0.1:7697");
  subscriberta9.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  // std::cout << "connect socket sub crate 9" << std::endl;      

  //int num=0;


  //std::ofstream outputFile;
  //  outputFile.open("Alg1.txt");

  //dead channels                                                                                                                                                             
  int coll, cola;
  std::vector<int> deadch;
  std::vector<int> nodeadch;
  ifstream deadchfile("MCC9_channel_list.txt");

  // <2 --> these are not dead channels,  4 no dead ch                                                                                                                        
  if (deadchfile.is_open() && !deadchfile.eof()){
    while( deadchfile  >> coll >> cola ){
      //modified by daisy based on evds I see and comparing those evds to MCC9 ... text file                                                                           
      if(cola < 2){
	//	cout << "zero" << endl; 
	deadch.push_back(0);  // Dead Channels
	}
	else{
	  // cout << "one" << endl;
	  deadch.push_back(1); // Not dead 
	}
    }}     

  deadchfile.close();

  int num=0;

  std::ofstream outputFile;
  outputFile.open("Alg1.txt");

  std::ofstream algtimefile;
  algtimefile.open("Algtime1.txt");

  bool timesee=true;
  //  cout << "deadch" << deadch.size() << endl;
  //cout << "dead" << deadch[2] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

  while(true) {
    zmq::message_t messageta2;
    //    auto start;// = std::chrono::system_clock::now();
    std::chrono::high_resolution_clock::time_point start;


    if(timesee==true){
      //    auto
 start = std::chrono::system_clock::now();
    subscriberta2.recv(&messageta2);
    timesee=false;
    
}
    else {

    subscriberta2.recv(&messageta2);
    //    auto
 start = std::chrono::system_clock::now();  
  }
    //    auto end1pr = std::chrono::system_clock::now();
    // auto elapsed1pr = std::chrono::duration_cast<std::chrono::nanoseconds>(end1pr - start);


    zmq::message_t messageta3;
    subscriberta3.recv(&messageta3);
    //std::cout << "message size TA3: " << messageta3.size() << std::endl;                                                                                   

    // auto end2pr = std::chrono::system_clock::now();
    // auto elapsed2pr = std::chrono::duration_cast<std::chrono::nanoseconds>(end2pr - start);


    zmq::message_t messageta4;
    subscriberta4.recv(&messageta4);
    // std::cout << "message size TA4: " << messageta4.size() << std::endl;                                                                                  

    zmq::message_t messageta5;
    subscriberta5.recv(&messageta5);
    // std::cout << "message size TA5: " << messageta5.size() << std::endl;                                                                                  

    //auto end4pr = std::chrono::system_clock::now();
    //auto elapsed4pr = std::chrono::duration_cast<std::chrono::nanoseconds>(end4pr - start);


    zmq::message_t messageta6;
    subscriberta6.recv(&messageta6);
    // std::cout << "message size TA6: " << messageta6.size() << std::endl;                                                                                  

    zmq::message_t messageta7;
    subscriberta7.recv(&messageta7);
    //std::cout << "message size TA7: " << messageta7.size() << std::endl;                                                                                   

    //auto end6pr = std::chrono::system_clock::now();
    //auto elapsed6pr = std::chrono::duration_cast<std::chrono::nanoseconds>(end6pr - start);

    zmq::message_t messageta8;
    subscriberta8.recv(&messageta8);
    //std::cout << "message size TA8: " << messageta8.size() << std::endl;                                                                                   

    zmq::message_t messageta9;
    subscriberta9.recv(&messageta9);
    auto endr = std::chrono::system_clock::now();
     auto elapsedr = std::chrono::duration_cast<std::chrono::microseconds>(endr - start);

    //cout << "dead" << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    std::cout << "receiving time from all 9 crates:" << std::dec << elapsedr.count() << " microseconds" << std::endl;

    std::vector<InfoForAlg> tp_list2( static_cast<InfoForAlg*>(messageta2.data()),
				      static_cast<InfoForAlg*>(messageta2.data()) + messageta2.size() / sizeof(InfoForAlg)
				      );
    //cout << "deada" << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    // auto end1pr = std::chrono::system_clock::now();
    // auto elapsed1pr = std::chrono::duration_cast<std::chrono::microseconds>(end1pr - endr);



    std::vector<InfoForAlg> tp_list3( static_cast<InfoForAlg*>(messageta3.data()),
                                      static_cast<InfoForAlg*>(messageta3.data()) + messageta3.size() / sizeof(InfoForAlg)
                                      );
    //cout << "dead b" << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    //    auto end2pr = std::chrono::system_clock::now();
    //    auto elapsed2pr = std::chrono::duration_cast<std::chrono::microseconds>(end2pr - endr);



    std::vector<InfoForAlg> tp_list4( static_cast<InfoForAlg*>(messageta4.data()),
                                      static_cast<InfoForAlg*>(messageta4.data()) + messageta4.size() / sizeof(InfoForAlg)
                                      );
    // cout << "dead c " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    std::vector<InfoForAlg> tp_list5( static_cast<InfoForAlg*>(messageta5.data()),
                                      static_cast<InfoForAlg*>(messageta5.data()) + messageta5.size() / sizeof(InfoForAlg)
                                      );

    //    auto end4pr = std::chrono::system_clock::now();
    //auto elapsed4pr = std::chrono::duration_cast<std::chrono::microseconds>(end4pr - endr);



    //cout << "tp " << tp_list5.size() << " , " << messageta5.size() << endl;
    //cout << "dead d " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    std::vector<InfoForAlg> tp_list6( static_cast<InfoForAlg*>(messageta6.data()),
                                      static_cast<InfoForAlg*>(messageta6.data()) + messageta6.size() / sizeof(InfoForAlg)
                                      );
    // cout << "dead e " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    std::vector<InfoForAlg> tp_list7( static_cast<InfoForAlg*>(messageta7.data()),
                                      static_cast<InfoForAlg*>(messageta7.data()) + messageta7.size() / sizeof(InfoForAlg)
                                      );
    //    cout << "dead f " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    //auto end6pr = std::chrono::system_clock::now();
    // auto elapsed6pr = std::chrono::duration_cast<std::chrono::microseconds>(end6pr - endr);



    std::vector<InfoForAlg> tp_list8( static_cast<InfoForAlg*>(messageta8.data()),
                                      static_cast<InfoForAlg*>(messageta8.data()) + messageta8.size() / sizeof(InfoForAlg)
                                      );

    //cout << "dead g " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    std::vector<InfoForAlg> tp_list9( static_cast<InfoForAlg*>(messageta9.data()),
                                      static_cast<InfoForAlg*>(messageta9.data()) + messageta9.size() / sizeof(InfoForAlg)
                                      );

    // cout << "dead AA : " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    auto end = std::chrono::system_clock::now();                                                                                                           
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);                                                                     
    std::vector<InfoForAlg> tp_list;
    tp_list.clear();

    for(const auto& tp_list_i : {tp_list2, tp_list3, tp_list4, tp_list5, tp_list6, tp_list7, tp_list8, tp_list9}){
      tp_list.insert(tp_list.end(), tp_list_i.begin(), tp_list_i.end());

    }
    //cout << "dead A : " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;

    std::cout << "Recived TPs from All crates: " << tp_list.size() << endl;
    auto end1 = std::chrono::system_clock::now();
    auto elapsed1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start);
    std::cout << "total time for all 9 crates:" << std::dec << elapsed1.count() << " microseconds" << std::endl;

    // cout << "dead 2 : " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    num+=1;

    // outputFile << num << "\t" << tp_list2.size() << " \t" <<  tp_list3.size()  << " \t" <<  tp_list4.size() << " \t" << tp_list5.size() << " \t" << tp_list6.size() << " \t" << tp_list7.size() << " \t" << tp_list8.size() << " \t" << tp_list9.size() << " \t" <<  tp_list.size()  << " \t" <<  elapsed1pr.count() << " \t" <<  elapsed2pr.count() << " \t" <<  elapsed4pr.count() << " \t" <<  elapsed6pr.count() <<   " \t" << elapsedr.count() << " \t"  <<  elapsed.count() << " \t" <<  elapsed1.count() << " \n";
    outputFile.flush();

    outputFile << num << "\t" << tp_list[0].driftRegion << "\t" << tp_list2.size() << " \t" <<  tp_list3.size()  << " \t" <<  tp_list4.size() << " \t" << tp_list5.size() << " \t" << tp_list6.size() << " \t" << tp_list7.size()<< " \t" << tp_list8.size() << " \t" << tp_list9.size() << " \t" <<  tp_list.size()  << " \t" << elapsedr.count() << " \t"  << elapsed1.count() << " \n";  



    //num+=1;
    tp_list2.clear();
    tp_list3.clear();
    tp_list4.clear();
    tp_list5.clear();
    tp_list6.clear();
    tp_list7.clear();
    tp_list8.clear();
    tp_list9.clear();

    //cout << "dead 3 : " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;


    /*
    //declare variables                                                                                                                                       
    //dead channels                                                                                                                                           
    int col1, col2;
    std::vector<int> deadch;
    std::vector<int> nodeadch;
    ifstream deadchfile("MCC9_channel_list.txt");

    // <2 --> these are not dead channels,  4 no dead ch                                                                                                      
    if (deadchfile.is_open() && !deadchfile.eof()){
      while( deadchfile  >> col1 >> col2 ){
        //modified by daisy based on evds I see and comparing those evds to MCC9 ... text file                                                                
        if (col1 >= 4800 and col2 ==1 ) //< 2) // not dead                                                                                                    
          {
            deadch.push_back(0);
          }
        //modified by daisy                                                                                                                                   
        else if (col1 >=4800 and col2==4) { // dead channels                                                                                                  
          deadch.push_back(1);
        }
      }
    }
    */

    //cout << "dead 4 : " << deadch[18] << " \t" << deadch[19] << " \t" << deadch[4952] << " \t" << endl;
    // cout << "deadCh Main: " << deadch[4952] << endl;

    int ch_init = 0;
    int checkSize;
    int candidateBool =0;
    int maxadcindex;
    int maxadcind;
    int maxadc =0;
    int chnlwid = 0;
    int timewid=0;
    int trigtot;
    int time_diff = 30;
    int braggE = 27500; //  27500 is used in uB based on incoming muon angle vs maxadc                                                                        
    int chnl_maxadc;
    int time_max, this_time_max, prev_time_max, horiz_tt;
    int temp_t;
    int time_min, this_time_min, prev_time_min, horiz_tb;
    int deadwidthscale = 8;
    float slopecm_scale = 0.04/0.3; //time ticks to cm = 1/25, channels to cm = 0.3                                                                           
    bool frontfound = false;
    bool hitfound = false;
    bool hitfoundb = false;
    int TPcount=0;
    int deadcnt=0;
    int braggcnt=0;
    int chcnt=0;
    int horiz_noise_cnt = 0;
    int horiz_tolerance = 8;
    int tracklen=26; //26;
    float radTodeg=180/3.14;
    int y2,y1,y3,y4;
    int x2,x1,x3,x4;
    float bky1,bky2,bky3,bky4, bkpy1,bkpy2,bkpy3,bkpy4;
    float frontangle_top, frontangle_mid, frontangle_bottom, backangle_top, backangle_mid, backangle_bottom;
    float slope, frontslope_top, frontslope_mid, frontslope_bottom, backslope_top, backslope_mid, backslope_bottom;
    //from chmapnew.txt file                                                                                                                                  
    int ColPlStartChnl = 4800; // from uB                                                                                                                     
    int ColPlEndChnl = 8255; //from uB                                                                                                                        
    //This alg is currently being tested on crate 3 file therefore we need to put chnl start and chnl end for crate 3 here:                                   
    //For testing: For final alg. we will use above values./                                                                                                  

    //int ColPlStartChnl =7296;                                                                                                                               
    //int ColPlEndChnl = 7775; //from uB                                                                                                                      

    int boxchcnt = 1;
    int prev_chnl, next_chnl, sf_chnl;
    int prev_tstart, next_tstart, sf_tstart;
    int prev_tot, next_tot, sf_tot;
    int contiguous_tolerance = 16;

    int boxwidtime=1150;
    std::vector<int> timeind_vec;
    int boxwidch=96; //96;                                                                                                                                    
    std::vector<int> chnlind_vec;
    //    std::vector<TriggerPrimitive> tp_list;   
    std::vector<TriggerPrimitive> tp_only;
    std::vector<TriggerPrimitive> tp_list_maxadc;
    std::vector<TriggerPrimitive> tp_list_maxadc_cp;
    std::vector<TriggerPrimitive> tp_list_this;
    std::vector<TriggerPrimitive> tp_list_prev;
    std::vector<TriggerPrimitive> tp_list_next;
    std::vector<TriggerPrimitive> tp_list_prevb;
    std::vector<TriggerPrimitive> tp_list_nextb;
    std::vector<TriggerPrimitive> tp_list_sf;
    std::vector<TriggerPrimitive> tp_list_sb;
    std::vector<InfoForAlg> tmpchnl_vec;
    std::vector<InfoForAlg> tmpchnl_vec_temp;
    std::vector<TriggerPrimitive> sublist;
    std::vector<TriggerPrimitive> final_tp_list;
    std::vector<int>  maxadcindex_vec;
    std::vector<int> initialvec_adc;
    std::vector<TriggerPrimitive> peaklist;

    int frame, fem, channel, time_start, time_over_threshold,adc_peak,adc_integral, adc_integralN,driftRegion,crateid;
    int run,subrun;

    //std::cout << "Initializing variables and clearing vector sizes" << std::endl;
    tp_list_maxadc.clear();
    boxchcnt=1;
    initialvec_adc.clear();
    tp_list_this.clear();
    frontfound = false;
    hitfoundb = false; 
    hitfound = false;
    deadcnt=0;
    braggcnt=0;
    slope = 0;
    horiz_noise_cnt = 0;
    frontslope_top = 0;
    backslope_top = 0;
    frontslope_mid = 0;
    backslope_mid = 0;
    frontslope_bottom = 0;
    backslope_bottom = 0;
    horiz_tb = 0;
    horiz_tt = 0;
    trigtot=0;

    //    for (int i=0; i<tp_list.size(); ++i){
    //std::cout << "Channels: " << tp_list[i].channel << std::endl;
    //} 

    //Checked and works
    std::sort (tp_list.begin(), tp_list.end(), compare_channel);

    for (int timeind=0; timeind <= 4600; timeind+=boxwidtime){
      //      std::cout << "Time box: " << timeind << std::endl;
      timeind_vec.push_back(timeind);
    }

    for(int chnlind=ColPlStartChnl; chnlind<(ColPlEndChnl+boxwidch); chnlind+=boxwidch){
      //std::cout << "Channel box: "<< chnlind << std::endl;
      chnlind_vec.push_back(chnlind);
    }


    for (int i=0; i<tp_list.size(); ++i){
      initialvec_adc.emplace_back(tp_list[i].adc_integral);
      run=tp_list[i].runa;
      subrun=tp_list[i].subruna;

      //std::cout << "Channel: " << tp_list[i].channel  << std::endl;


      if ((tp_list[i].channel > chnlind_vec[boxchcnt]) or (i==tp_list.size()-1)){
        if(tmpchnl_vec.size()==0){
          while(tp_list[i].channel > chnlind_vec[boxchcnt]){
            boxchcnt+=1;
            tmpchnl_vec_temp.push_back({tp_list[i].frame, tp_list[i].fem,tp_list[i].channel, tp_list[i].time_start, tp_list[i].time_over_threshold, tp_list[i].adc_peak, tp_list[i].adc_integral, tp_list[i].driftRegion, tp_list[i].crateid, tp_list[i].runa, tp_list[i].subruna});
           
	  }
        }

        else{
          tmpchnl_vec_temp.push_back({tp_list[i].frame, tp_list[i].fem,tp_list[i].channel, tp_list[i].time_start, tp_list[i].time_over_threshold, tp_list[i].adc_peak, tp_list[i].adc_integral, tp_list[i].driftRegion, tp_list[i].crateid, tp_list[i].runa, tp_list[i].subruna});
     
	  std::vector<int> maxadcVec;
	  std::vector<int> maxadcindVec;
          int indtouse;
          maxadc = 0;
          for(int time_ind=0; time_ind < 4; time_ind++){
            sublist.clear();

	    for (int tmpch=0; tmpch < tmpchnl_vec.size(); tmpch++){
	      if ((tmpchnl_vec[tmpch].time_start >= timeind_vec[time_ind]) and (tmpchnl_vec[tmpch].time_start < timeind_vec[time_ind+1])){

                sublist.push_back({tmpchnl_vec[tmpch].frame, tmpchnl_vec[tmpch].fem, tmpchnl_vec[tmpch].channel, tmpchnl_vec[tmpch].time_start,tmpchnl_vec[tmpch].time_over_threshold,tmpchnl_vec[tmpch].adc_peak, tmpchnl_vec[tmpch].adc_integral, tmpchnl_vec[tmpch].driftRegion, tmpchnl_vec[tmpch].crateid, candidateBool, run, subrun});
              }}

            maxadc = 0;

            if(sublist.size()>0){ //C. now  or i==tp_list.size()){                                                                                           \                                                                                                                                                                                          

              for (int sl=0; sl<sublist.size(); sl++){
                if (sublist[sl].adc_integral> maxadc) {
                  maxadc =  sublist[sl].adc_integral;
                  maxadcind = sl;
                }}
              //std::cout << "check on maxadc :" << maxadc  << " with index: " << maxadcind << std::endl;                                                                                                                                                                                                                                               

              if(maxadc > braggE and maxadc < 70000){
                tp_list_maxadc.push_back({sublist[maxadcind].frame, sublist[maxadcind].fem, sublist[maxadcind].channel, sublist[maxadcind].time_start, sublist[maxadcind].time_over_threshold, sublist[maxadcind].adc_peak, sublist[maxadcind].adc_integral, sublist[maxadcind].driftRegion, sublist[maxadcind].crateid, sublist[maxadcind].candidateBool,sublist[maxadcind].run, sublist[maxadcind].subrun});
                maxadc = 0;

              }}
          }
          tmpchnl_vec.clear();

        }
      }//end loop on channel indices [1]                                                                                                                                                                                                                                                                                                                
     
      if (tp_list[i].channel <= chnlind_vec[boxchcnt]){ // or i==tp_list.size()){                                                                       
	// cout << "filling tmp chnl" << endl;                                                                                                             
       if(tmpchnl_vec_temp.size()!=0) {
 	 tmpchnl_vec.insert(tmpchnl_vec.end(),tmpchnl_vec_temp.begin(),tmpchnl_vec_temp.end());
	 //std::cout << "again check size of tmpchnl_vec: " << tmpchnl_vec.size() << std::endl;
	 tmpchnl_vec_temp.clear();
      }  

       tmpchnl_vec.push_back({tp_list[i].frame, tp_list[i].fem,tp_list[i].channel, tp_list[i].time_start, tp_list[i].time_over_threshold, tp_list[i].adc_peak, tp_list[i].adc_integral, tp_list[i].driftRegion, tp_list[i].crateid, tp_list[i].runa, tp_list[i].subruna});
     
      }
  
    }//end of tp_list size


    //    std::cout << "max adc size - " << tp_list_maxadc.size() << std::endl;

    // ****************************SECOND PART ***************************************                                                                          
    for(int imaxadc=0; imaxadc<tp_list_maxadc.size(); imaxadc++){

      //cout << "checkl frame in maxadc list " << tp_list_maxadc[imaxadc].frame << " , dR: " << tp_list_maxadc[imaxadc].driftRegion << endl; 
      chnl_maxadc = tp_list_maxadc[imaxadc].channel;
      //cout << "chnl: " << chnl_maxadc << endl;                                                                                                                
      time_max = tp_list_maxadc[imaxadc].time_start + tp_list_maxadc[imaxadc].time_over_threshold;
      time_min = tp_list_maxadc[imaxadc].time_start;
      tp_list_this.push_back({tp_list_maxadc[imaxadc].frame, tp_list_maxadc[imaxadc].fem, tp_list_maxadc[imaxadc].channel, tp_list_maxadc[imaxadc].time_start,tp_list_maxadc[imaxadc].time_over_threshold,tp_list_maxadc[imaxadc].adc_peak,tp_list_maxadc[imaxadc].adc_integral,tp_list_maxadc[imaxadc].driftRegion, tp_list_maxadc[imaxadc].crateid, tp_list_maxadc[imaxadc].candidateBool,tp_list_maxadc[imaxadc].run,tp_list_maxadc[imaxadc].subrun});

      tp_list_prev = tp_list_this;
      tp_list_next = tp_list_this;
      tp_list_sf = tp_list_this;
      tp_list_sb = tp_list_this;

   frontfound = false;
   hitfound = false;
   deadcnt=0;
   braggcnt=0;
   slope = 0;
   horiz_noise_cnt = 0;
   frontslope_top = 0;
   backslope_top = 0;
   frontslope_mid = 0;
   backslope_mid = 0;
   frontslope_bottom = 0;
   backslope_bottom = 0;
   frontangle_top = 0;
   frontangle_mid = 0;
   frontangle_bottom = 0;
   backangle_top=0;
   backangle_mid=0;
   backangle_bottom=0;
   horiz_tb = 0;
   horiz_tt = 0;

   maxadcindex =  getIndex(initialvec_adc, tp_list_maxadc[imaxadc].adc_integral);
 

  int icheck;
   for (icheck=maxadcindex+1; icheck<tp_list.size(); icheck++){

     //cout << "checkl frame in full tp list " <<tp_list_maxadc[imaxadc].frame << " , dR: " << tp_list_maxadc[imaxadc].driftRegion << endl;

     //cout << " start channel with index: " << icheck << " , chnl: " << tp_list[icheck].channel << " ,time start - " << tp_list[icheck].time_start << ",  maxtim ts+tot" << tp_list[icheck].time_start+tp_list[icheck].time_over_threshold  << endl;   

     if (frontfound == true){
       std::cout << "maxadc dR: " << tp_list_this[imaxadc].driftRegion <<  " , frame: " << tp_list_this[imaxadc].frame  <<" , chnl: " <<  tp_list_this[imaxadc].channel<< " ,time:" << tp_list_this[imaxadc].time_start  << " , adc: " << tp_list_this[imaxadc].adc_integral << " ,tot: " << tp_list_this[imaxadc].time_over_threshold << std::endl;
       //std::cout << "Front found cordinates: " << tp_list_next[imaxadc].channel << " , time: " << tp_list_next[imaxadc].time_start  << " , adc: " << tp_list_next[imaxadc].adc_integral << " , tot: " << tp_list_next[imaxadc].time_over_threshold << std::endl;  
       break;
     }                                                                 

     // ***************************************** CHNL+2 LOOP   

     if(tp_list[icheck].channel >= (chnl_maxadc+2)){
       chnl_maxadc = tp_list_next[imaxadc].channel;

       if (hitfound == false){
	 //cout << "no hit found!"<< endl;                                                                                                                                                                                                                                                                                                               
	 break;
       }

       // ******** Checking if the next channel is dead channel                                                                                                                                                                                                                                                                                          
       if (deadch[chnl_maxadc+1] ==0 and tp_list[icheck].channel!=(chnl_maxadc+1)){
	 if (tp_list_prev[imaxadc].channel == chnl_maxadc){
	   slope = 0;
	   //  cout << "zero slope" << endl;                                                                                                                                                                                                                                                                                                             
	 }

	 else if (tp_list_prev[imaxadc].channel == tp_list_sf[imaxadc].channel){
	   int den3 = (tp_list_prev[imaxadc].channel-tp_list_maxadc[imaxadc].channel);
	   if(den3!=0){
	     slope = float((tp_list_prev[imaxadc].time_start + (tp_list_prev[imaxadc].time_over_threshold)/2 - tp_list_maxadc[imaxadc].time_start - (tp_list_maxadc[imaxadc].time_over_threshold)/2))/ den3 ;
                                                                          
	   }}

	 else {
	   int den4 = (tp_list_prev[imaxadc].channel-tp_list_sf[imaxadc].channel);

	   if(den4!=0){
	     slope = float((tp_list_prev[imaxadc].time_start + (tp_list_prev[imaxadc].time_over_threshold)/2 - tp_list_sf[imaxadc].time_start - (tp_list_sf[imaxadc].time_over_threshold)/2))/ den4;
	     // cout << "slope " << slope << endl;                                                                                                                                                                                                                                                                                                       
	   }}
	 deadcnt = 0;

	 while (deadch[chnl_maxadc+1] ==0 and chnl_maxadc+1 < ColPlEndChnl) {
	   chnl_maxadc += 1;
	   deadcnt += 1;
	   // std::cout << "Skipping through dead channel: " << chnl_maxadc << "with dead cnt: " << deadcnt << std::endl;                                                                                                                                                                                                                                
	 }
	 tp_list_prev[imaxadc].time_start = tp_list_prev[imaxadc].time_start + std::floor(float(slope)*float(deadcnt));
       }
       // ************************* Dead Channel accounting ends                                                                                                                                                                                                                                                                                         
       if (hitfound == true){
	 braggcnt+=1;
	 //      cout << "update bragg count " << braggcnt << endl;                                                                                                                                                                                                                                                                                      
	 if (braggcnt==3){
	   tp_list_sf = tp_list_next;
	 }
	 if (braggcnt >= tracklen/2){
	   frontfound = true;
	   int denf = (tp_list_next[imaxadc].channel - tp_list_sf[imaxadc].channel);
	   if (denf!=0){

	     frontslope_top = float(tp_list_next[imaxadc].time_start + tp_list_next[imaxadc].time_over_threshold - tp_list_sf[imaxadc].time_start - tp_list_sf[imaxadc].time_over_threshold)/ denf;

	     frontslope_mid = float(tp_list_next[imaxadc].time_start + (tp_list_next[imaxadc].time_over_threshold)/2 -tp_list_sf[imaxadc].time_start - (tp_list_sf[imaxadc].time_over_threshold)/2)/ denf ;

	     frontslope_bottom = float(tp_list_next[imaxadc].time_start - tp_list_sf[imaxadc].time_start)/ denf;

	     //    cout << " front Slopes: " << frontslope_top << " , " << frontslope_mid  << " , " << frontslope_bottom << endl;

	   }

	 }
	 //      cout << "prev (before updating) : " << tp_list_prev[imaxadc].channel << " , next channel " << tp_list_next[imaxadc].channel << endl;                                                                                                                                                                                                    
	 tp_list_prev = tp_list_next;

       }

       hitfound = false;
       //cout << "hit found turned to false and times to zero  " << endl;                                                                                                                                                                                                                                                                                
       this_time_max = 0;
       this_time_min = 0;
       prev_time_max = 0;
       prev_time_min = 0;

     }



    // ***************************************** CHNL+1 LOOP

     if(tp_list[icheck].channel == (chnl_maxadc+1)){ // or tp_list[icheck].channel == (chnl_maxadc+2)){
      this_time_max = tp_list[icheck].time_start + tp_list[icheck].time_over_threshold;
      this_time_min =  tp_list[icheck].time_start;
      prev_time_max = tp_list_prev[imaxadc].time_start + tp_list_prev[imaxadc].time_over_threshold;
      prev_time_min =tp_list_prev[imaxadc].time_start;


      if ((this_time_min>=prev_time_min and this_time_min<=prev_time_max) or (this_time_max<=prev_time_max and this_time_max>=prev_time_min) or (prev_time_max<=this_time_max and prev_time_min>=this_time_min) or (prev_time_min<=this_time_max and prev_time_min>this_time_min) or (prev_time_max<this_time_max and prev_time_max>=this_time_min)){

      if (horiz_noise_cnt == 0){
	horiz_tb = prev_time_min;
	horiz_tt = prev_time_max;
      }


      if (tp_list[icheck].channel == tp_list_next[imaxadc].channel) break;
      hitfound = true; 
      //std::cout << "Hit Found in channel: " << tp_list[icheck].channel << std::endl;  
      // cout << "Next cordinates- Chnl " << tp_list_next[imaxadc].channel << " , tmin "  << tp_list_next[imaxadc].time_start << endl; 
      tp_list_next[imaxadc].channel = tp_list[icheck].channel;   // Assigning right value (tp_list) to left value (next)                            
      tp_list_next[imaxadc].time_start = tp_list[icheck].time_start;
      tp_list_next[imaxadc].adc_integral = tp_list[icheck].adc_integral;
      tp_list_next[imaxadc].adc_peak = tp_list[icheck].adc_peak;
      tp_list_next[imaxadc].time_over_threshold = tp_list[icheck].time_over_threshold;

      if (abs(this_time_min - horiz_tb) <=1 or abs(this_time_max - horiz_tt) <=1){
	horiz_noise_cnt+=1;
	if (horiz_noise_cnt>horiz_tolerance) break;
      }
      else{
	horiz_noise_cnt = 0;
      }
      if (this_time_max > time_max){
	//	cout << "updating time max" << endl;                                                                                                       
	time_max = this_time_max;
	//cout << "updated tmax val: " << time_max << endl; 
      }
      if (this_time_min < time_min){
	//cout << "updating time min: " << time_min << endl;  
	time_min = this_time_min;
	//cout << "updated time min: " << time_min << endl;                                                                                             
      }

    }//end of time conditions
    }// end of chanel > chlmax+1

   }// end of tp_list max adc 
   // look backward now  

   chnl_maxadc = tp_list_maxadc[imaxadc].channel;
   time_max = tp_list_maxadc[imaxadc].time_start + tp_list_maxadc[imaxadc].time_over_threshold;
   time_min = tp_list_maxadc[imaxadc].time_start;

   tp_list_prev = tp_list_this;
   tp_list_next = tp_list_this;

   this_time_max =0;
   this_time_min =0;
   prev_time_max = 0;
   prev_time_min =0;
   deadcnt=0;
   slope=0;
   hitfoundb = false;
   horiz_noise_cnt = 0;
   horiz_tb = 0;
   horiz_tt = 0;

   // cout << "chmax is set back to original value: " << chnl_maxadc << endl;
   // cout << "prev and next set to this channel check " << endl; 

   if (frontfound == true){
     //std::cout << "FRONT FOUND!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
     //cout << "check on bragg cnt: " <<  braggcnt << endl;
     int icheckb;
     for (icheckb=maxadcindex-1; icheckb>=0; icheckb--){
       //cout << "bk icheck channel : " << tp_list[icheckb].channel << "maxadc: "  << chnl_maxadc << endl;       




       //******************* ch-2 loop                                                                                                                                                                                                                                                                                                                  
       if(tp_list[icheckb].channel <= (chnl_maxadc-2)){
         chnl_maxadc = tp_list_next[imaxadc].channel;
         // cout << "Check hitfound and hitfounb both: " << hitfound << " , " << hitfoundb << endl;                                                                                                                                                                                                                                                     
         if (hitfoundb == false) break;

         //******* dead channel accounting                                                                                                                                                                                                                                                                                                              
         if (deadch[chnl_maxadc-1] ==0 and tp_list[icheckb].channel!=(chnl_maxadc-1)){
           if (tp_list_prev[imaxadc].channel == chnl_maxadc){
             slope = 0;
           }
           else if (tp_list_prev[imaxadc].channel == tp_list_sb[imaxadc].channel){
             int den5 = (tp_list_prev[imaxadc].channel-tp_list_maxadc[imaxadc].channel);

             if(den5!=0){
               slope = float(tp_list_prev[imaxadc].time_start + (tp_list_prev[imaxadc].time_over_threshold)/2 - tp_list_maxadc[imaxadc].time_start - (tp_list_maxadc[imaxadc].time_over_threshold)/2) / den5 ;
             }}
           else {
             int den6 = (tp_list_prev[imaxadc].channel-tp_list_sb[imaxadc].channel);
             if(den6!=0){
               slope = (tp_list_prev[imaxadc].time_start + (tp_list_prev[imaxadc].time_over_threshold)/2 - tp_list_sb[imaxadc].time_start - (tp_list_sb[imaxadc].time_over_threshold)/2)/ den6;
             }}
           deadcnt = 0;
           while (deadch[chnl_maxadc-1] ==0 and chnl_maxadc-1 > ColPlStartChnl) {
             chnl_maxadc -= 1;
             deadcnt += 1;

           }
           tp_list_prev[imaxadc].time_start = tp_list_prev[imaxadc].time_start - std::floor(float(slope)*float(deadcnt));
         }

         // ********* dead channel accounting ends                                                                                                                                                                                                                                                                                                      

         if (hitfoundb == true) {
           braggcnt+=1;
           //cout << "bkward hit found and bragg cnt updated to " << braggcnt << endl;                                                                                                                                                                                                                                                                  

           if (braggcnt == tracklen/2+3){

             //cout << "brg trck found and sb channel updated to next " << tp_list_sb[imaxadc].channel <<  " , " << tp_list_next[imaxadc].channel << endl;                                                                                                                                                                                              
             tp_list_sb = tp_list_next;

             //  cout << "updated " << tp_list_sb[imaxadc].channel <<  " , " << tp_list_next[imaxadc].channel << endl;                                                                                                                                                                                                                                  
           }

           if (braggcnt >= tracklen){
             // cout << "bk bragg track " << endl;                                                                                                                                                                                                                                                                                                      
             bky1=tp_list_next[imaxadc].time_start;
             bky2=tp_list_next[imaxadc].time_over_threshold;
             bky3=tp_list_sb[imaxadc].time_start;
             bky4=tp_list_sb[imaxadc].time_over_threshold;

             //non zero den conditions:                                                                                                                                                                                                                                                                                                                 
             float num = float(bky1+bky2-(bky3+bky4));
             int den = (tp_list_next[imaxadc].channel - tp_list_sb[imaxadc].channel);
             //cout << "num " << num << " , den " << den << endl;                                                                                                                                                                                                                                                                                       
             if(den!=0){
               backslope_top = (bky1+bky2-(bky3+bky4)) / den;
               backslope_mid = (bky1+(bky2/2)-(bky3+(bky4/2))) / den;
               backslope_bottom = (bky1-bky3) / den;

	       //  cout << "back Slopes: " << backslope_top << " , " << backslope_mid  << " , " << backslope_bottom << endl;               

	       frontangle_top = (atan(slopecm_scale*float(frontslope_top)))*radTodeg;
               backangle_top = (atan(slopecm_scale*float(backslope_top)))*radTodeg;
               frontangle_mid = (atan(slopecm_scale*float(frontslope_mid)))*radTodeg;
               backangle_mid = (atan(slopecm_scale*float(backslope_mid)))*radTodeg;
               frontangle_bottom = (atan(slopecm_scale*float(frontslope_bottom)))*radTodeg;
               backangle_bottom = (atan(slopecm_scale*float(backslope_bottom)))*radTodeg;

	       //cout << "angles: " << frontangle_top << " , " <<  backangle_top << "angles mid: " << frontangle_mid << " , " <<  backangle_mid << "angles bottom: " << frontangle_bottom << " , " <<  backangle_bottom << endl; 

	       /* double angletop = atan(slopecm_scale*fabs((backslope_top  - frontslope_top) / (1 + frontslope_top * backslope_top)));
               double angletopDeg = angletop*radTodeg;

               double anglemid = atan(slopecm_scale*fabs((backslope_mid  - frontslope_mid) / (1 + frontslope_mid * backslope_mid)));
	       double anglemidDeg = anglemid*radTodeg;

               double anglebottom = atan(slopecm_scale*fabs((backslope_bottom  - frontslope_bottom) / (1 + frontslope_bottom * backslope_bottom)));
	       double anglebottomDeg = anglebottom*radTodeg;
	       */
	       //	       cout << "front angle: " << abs(frontangle_mid-backangle_mid) << " , front angle top: " << abs(frontangle_top-backangle_top) << " , front angle bottom: " << abs(frontangle_bottom-backangle_bottom) << endl;
	       //cout << "angle btw two tracks: " << anglemidDeg << " , " << angletopDeg << " , " <<  anglebottomDeg << endl;



               //peaklist.clear();
               //peaklist.push_back({tp_list_maxadc[imaxadc].frame, tp_list_maxadc[imaxadc].fem, tp_list_maxadc[imaxadc].channel,tp_list_maxadc[imaxadc].time_start,tp_list_maxadc[imaxadc].time_over_threshold,tp_list_maxadc[imaxadc].adc_peak,tp_list_maxadc[imaxadc].adc_integral,tp_list_maxadc[imaxadc].driftRegion, tp_list_maxadc[imaxadc].crateid, tp_list_maxadc[imaxadc].candidateBool, tp_list_maxadc[imaxadc].run, tp_list_maxadc[imaxadc].subrun});
	       //std::cout << " Front found corinates, Frame: " << tp_list_maxadc[imaxadc].frame << " , channel: " << tp_list_maxadc[imaxadc].channel << " , time: " << tp_list_maxadc[imaxadc].time_start <<" , integral: "      << tp_list_maxadc[imaxadc].adc_integral<< " , front channel: " <<  tp_list[icheck].channel << " , time: " <<  tp_list[icheck].time_start << " , dR: " << tp_list_maxadc[imaxadc].driftRegion  << std::endl;                                                  

	       //std::cout << "Back cordinates dR: " << tp_list_next[imaxadc].driftRegion <<  " , frame: " << tp_list_next[imaxadc].frame  << " , chnl: "  <<  tp_list_next[imaxadc].channel << " , time: " << tp_list_next[imaxadc].time_start << " , adc: " << tp_list_next[imaxadc].adc_integral << " , tot: " << tp_list_next[imaxadc].time_over_threshold << std::endl;
	       //std::cout << "sb channel: " << tp_list_sb[imaxadc].channel << " , time: " << tp_list_sb[imaxadc].time_start << std::endl;


	     auto end2st = std::chrono::system_clock::now();
	    auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2st - endr);

	    std::cout << "time to trigger Stopping muon:" << std::dec << elapsed2.count() << " microseconds" << std::endl;
	     if(peaklist.size() != 0 ){
	       std::cout << "Peaklist size: " << peaklist.size() << std::endl;
	       zmq::message_t message1(sizeof(TriggerPrimitive) * peaklist.size());
	       memcpy(message1.data(), peaklist.data(), sizeof(TriggerPrimitive) * peaklist.size());
	       publisher.send(message1);
	       peaklist.clear();
	       }

	     //auto end2 = std::chrono::system_clock::now();
	     //auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - endr);
	     algtimefile.flush();
	     algtimefile << tp_list_maxadc[imaxadc].driftRegion << "\t" << elapsed2.count() << " \n" ;


	     if (abs(frontangle_mid-backangle_mid)>30  and abs(frontangle_top-backangle_top)>30 and abs(frontangle_bottom-backangle_bottom)>30){
	       trigtot += 1;

	       candidateBool=1; // issue a candidate                                                                                                                                                                                                                                                                                                  
	       std::cout <<" Trigger: " << trigtot << std::endl;
	       peaklist.clear();

	       peaklist.push_back({tp_list_maxadc[imaxadc].frame, tp_list_maxadc[imaxadc].fem, tp_list_maxadc[imaxadc].channel,tp_list_maxadc[imaxadc].time_start,tp_list_maxadc[imaxadc].time_over_threshold,tp_list_maxadc[imaxadc].adc_peak,tp_list_maxadc[imaxadc].adc_integral,tp_list_maxadc[imaxadc].driftRegion, tp_list_maxadc[imaxadc].crateid, tp_list_maxadc[imaxadc].candidateBool, tp_list_maxadc[imaxadc].run, tp_list_maxadc[imaxadc].subrun});


	       std::cout << " Front found corinates, Frame: " << tp_list_maxadc[imaxadc].frame << " , channel: " << tp_list_maxadc[imaxadc].channel << " , time: " << tp_list_maxadc[imaxadc].time_start <<" , integral: "<< tp_list_maxadc[imaxadc].adc_integral<< " , Back channel: " <<   tp_list[icheckb].channel << " , time: " <<  tp_list[icheckb].time_start << " , dR: " << tp_list_maxadc[imaxadc].driftRegion << std::endl;

	       auto end2 = std::chrono::system_clock::now();
	       auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - endr);
	       algtimefile.flush();
	       algtimefile << tp_list_maxadc[imaxadc].driftRegion << "\t" << elapsed2.count() << " \n" ;
 
	       std::cout << "time to trigger michel:" << std::dec << elapsed2.count() << " microseconds" << std::endl;


	       if(peaklist.size() != 0 ){
		 std::cout << "Peaklist size: " << peaklist.size() << std::endl;
		 zmq::message_t message1(sizeof(TriggerPrimitive) * peaklist.size());
		 memcpy(message1.data(), peaklist.data(), sizeof(TriggerPrimitive) * peaklist.size());
		 publisher.send(message1);

	       }

	       auto end2r = std::chrono::system_clock::now();
	       auto elapsed2r = std::chrono::duration_cast<std::chrono::microseconds>(end2r - endr);
	       std::cout << "Sent candidate decision: " << std::dec << elapsed2r.count() << " microseconds" << std::endl;
	       peaklist.clear();
	       break;

	     }
	     else {
	       break;
	     }
	   }}
	 tp_list_prev = tp_list_next;
       }

       //      cout <<"Bkwrd turning hit foundb to false and intitializing time vars " << endl;                                                                                                                                                                                                                                                       
       hitfoundb =false;
       this_time_max = 0;
       this_time_min = 0;
       prev_time_max = 0;
       prev_time_min = 0;
     }


       //******************* ch-1 loop
       if(tp_list[icheckb].channel == (chnl_maxadc-1)){ // or tp_list[icheckb].channel == (chnl_maxadc-2)){
	 this_time_max = tp_list[icheckb].time_start + tp_list[icheckb].time_over_threshold;
         this_time_min =  tp_list[icheckb].time_start;
         prev_time_max = tp_list_prev[imaxadc].time_start + tp_list_prev[imaxadc].time_over_threshold;
         prev_time_min =tp_list_prev[imaxadc].time_start;

	 if ((this_time_min>=prev_time_min and this_time_min<=prev_time_max) or (this_time_max>=prev_time_min and this_time_max<=prev_time_max) or(prev_time_max<=this_time_max and prev_time_min>=this_time_min) or (prev_time_min<=this_time_max and prev_time_min>this_time_min) or (prev_time_max<this_time_max and prev_time_max>=this_time_min)) {

	   if (horiz_noise_cnt == 0){
	     horiz_tb = prev_time_min;
	     horiz_tt = prev_time_max;

	   }
	   if (tp_list[icheckb].channel == tp_list_next[imaxadc].channel) break;
	   hitfoundb = true;
	   //cout << " bkwrd hit found " << endl; 
	   tp_list_next[imaxadc].channel = tp_list[icheckb].channel;
	   tp_list_next[imaxadc].time_start = tp_list[icheckb].time_start;
	   tp_list_next[imaxadc].adc_integral = tp_list[icheckb].adc_integral;
	   tp_list_next[imaxadc].adc_peak = tp_list[icheckb].adc_peak;
	   tp_list_next[imaxadc].time_over_threshold = tp_list[icheckb].time_over_threshold;
	   //cout << "bk assigninf next channel = this channel icheckb " << tp_list_next[imaxadc].channel << " , " << tp_list[icheckb].channel << endl;   

	   if (abs(this_time_min - horiz_tb) <=1 or abs(this_time_max - horiz_tt) <=1){
	     horiz_noise_cnt+=1;
	     if (horiz_noise_cnt>horiz_tolerance) break;
	   }
	   else{
	     horiz_noise_cnt = 0;
	   }
	   if (this_time_max > time_max) time_max = this_time_max;
	   if (this_time_min < time_min) time_min = this_time_min;
	   //cout << "check hitfound b at end of first loop: " << hitfoundb << endl;
         }}


     }// end of icheckb

   } // end of front found 

   

    } // end of tp_list size


    /*    if(peaklist.size() != 0 ){

      std::cout << "Peaklist size: " << peaklist.size() << std::endl;
      zmq::message_t message1(sizeof(TriggerPrimitive) * peaklist.size());
      memcpy(message1.data(), peaklist.data(), sizeof(TriggerPrimitive) * peaklist.size());
      //std::cout << "Sending Ist message of size: " << message1.size() << std::endl; \                                
                                                                                                                    \

      publisher.send(message1);
    }

    auto end2r = std::chrono::system_clock::now();
    auto elapsed2r = std::chrono::duration_cast<std::chrono::microseconds>(end2r - endr);
    std::cout << "Sent candidate decision: " << std::dec << elapsed2r.count() << " microseconds" << std::endl;
    */

  } // end of while
} // end of main function

