//Code written by Daisy Kalra (June 20, 2022) dkalra@nevis.columbia.edu                
// Binary decoder to decode SN data, calculate TPs and write TPs to a text file.       

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <map>
#include <tuple>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <sys/time.h>
#include <cstdint>
//#include <filesystem>
#include <boost/filesystem.hpp>


#include <chrono>

#include <zmq.hpp>
#include <unistd.h>
#include <thread>

#include <dirent.h>
#include <cstring>


using namespace std;

bool naturalSortComparator(const std::string& a, const std::string& b) {
  std::string::const_iterator it1 = a.begin();
  std::string::const_iterator it2 = b.begin();

  while (it1 != a.end() && it2 != b.end()) {
    if (isdigit(*it1) && isdigit(*it2)) {
      size_t num1 = 0;
      size_t num2 = 0;
      while (it1 != a.end() && isdigit(*it1)) {
	num1 = num1 * 10 + (*it1 - '0');
	++it1;
      }

      while (it2 != b.end() && isdigit(*it2)) {
	num2 = num2 * 10 + (*it2 - '0');
	++it2;
      }

      if (num1 != num2) {
	return num1 < num2;
      }
    } else if (*it1 != *it2) {
      return *it1 < *it2;
    } else {
      ++it1;
      ++it2;
    }
  }

  return it1 == a.end() && it2 != b.end();
}



struct ForChannelMap {
  int crate = {0};
  int femchmap   = {0};
  int ch    = {0};
  int LArWire = {0};

  ForChannelMap(int crate11, int femchmap11, int ch11, int LArWire11)
  {
    crate = crate11;
    femchmap = femchmap11;
    ch = ch11;
    LArWire = LArWire11;
  }



};

struct ForBaseline {
  int seb = {0};
  int sebfem   = {0};
  int sebfemch    = {0};
  int sebfemchped = {0};

  ForBaseline(int seba, int sebfema, int sebfemcha, int sebfemchpeda)
  {
    seb = seba;
    sebfem = sebfema;
    sebfemch = sebfemcha;
    sebfemchped = sebfemchpeda;
  }



};


struct PassInfoToStitching {
  uint32_t minFrame = {0};
  uint32_t frame4 = {0};
  uint32_t fem   = {0};
  int mappedchannel  = {0};
  uint16_t timetick  = {0};
  int      tot   = {0};
  uint64_t amp   = {0};
  uint64_t intgrl= {0};
  //int run={0};
  //int subrun={0};

  PassInfoToStitching(uint32_t minfr, uint32_t fr4, uint32_t fm, int mc, uint16_t tt, int tot1, uint64_t am, uint64_t in)//, int rr, int rs)

  {
    minFrame = minfr;
    frame4 = fr4;
    fem = fm;
    mappedchannel = mc;
    timetick = tt;
    tot = tot1;
    amp = am;
    intgrl = in;
    //    run=rr;
    //subrun=rs;

  }


};

struct PassInfoToAlg {

  uint32_t frame4a = {0};
  uint32_t fema   = {0};
  uint32_t mappedchannela  = {0};
  uint16_t timeticka  = {0};
  int      tota   = {0};
  uint64_t ampa   = {0};
  uint64_t intgrla= {0};
  int tda = {0};
  int detida = {0};
  int runa={0};                                                                                                                                                         
  int subruna={0};                                                                                                                                                        

  PassInfoToAlg(uint32_t fr4a, uint32_t fma, uint32_t mca, uint16_t tta, int tot1a, uint64_t ama, uint64_t ina, int tdda, int detidda, int rra, int rsa)             

  {

    frame4a = fr4a;
    fema = fma;
    mappedchannela = mca;
    timeticka = tta;
    tota = tot1a;
    ampa = ama;
    intgrla = ina;
    tda = tdda;
    detida = detidda;
    runa = rra;
    subruna = rsa;
  }


};

int nearestPowerOf2(double n1) {
  // Find the exponent that is closest to log2(n1)                                                                                                     
  int exponent = round(log2(n1));

  // Compute the number closest to 2^exponent                                                                                                          
  double closest = pow(2, exponent);

  // Return the closest number                                                                                                                         
  return static_cast<int>(closest);
}


int returnWire(int fcrate, int ffem, int fch, std::vector<ForChannelMap> fholdChMapIds){
  int mappedwire;
  for (int i=0; i<fholdChMapIds.size(); i++){
    std::map<std::tuple<int,int,int>, int> multimap;
    if(fholdChMapIds[i].crate==fcrate and fholdChMapIds[i].femchmap == ffem and fholdChMapIds[i].ch == fch) {
      std::tuple<int,int,int> key(fholdChMapIds[i].crate, fholdChMapIds[i].femchmap, fholdChMapIds[i].ch);
      int val = fholdChMapIds[i].LArWire;
      multimap[key] = val;
      mappedwire = multimap[key];
      return mappedwire;
    }
  }}

int returnbaseline(int fseb, int fsebfem, int fsebfemch, std::vector<ForBaseline> fholdsebpeds){
  int baseline;
  for (int i=0; i<fholdsebpeds.size(); i++){

    std::map<std::tuple<int,int,int>, int> multimap;
    // std::cout << "Match SEB: " << fholdsebpeds[i].seb << ", " << fseb << "MatchFEM: " << fholdsebpeds[i].sebfem <<  "," << fsebfem << " Match FEM ch: " <<  fholdsebpeds[i].sebfemch  << "," << fsebfemch << std::endl;                                                        \
                                                                                                                                          
    if(fholdsebpeds[i].seb==fseb and fholdsebpeds[i].sebfem == fsebfem  and fholdsebpeds[i].sebfemch == fsebfemch) {
      //std::cout << "Matched!!!!!!"  << fseb << "," << fsebfem << "," << fsebfemch << std::endl;                                        
 
      std::tuple<int,int,int> key(fholdsebpeds[i].seb, fholdsebpeds[i].sebfem, fholdsebpeds[i].sebfemch);
      int val = fholdsebpeds[i].sebfemchped;
      multimap[key] = val;
      baseline = multimap[key];
      //std::cout << "baseline: " << baseline << std::endl;                                                                             
  
      return baseline;
    }
  }}


int main(int argc, char** argv){
//int main (int argc, char* argv[]){
//int main(){
//  zmq::context_t context(1);
  // zmq::socket_t publisher(context, ZMQ_PUB);
  //publisher.bind("tcp://127.0.0.1:7732");
  //std::cout << "bind socket pub" << std::endl;

  zmq::context_t context1(1);
  zmq::socket_t publisher(context1, ZMQ_PUB);
  publisher.bind("tcp://127.0.0.1:7697"); // 50 from 42                                                                                            
                            
   

  zmq::context_t context2s(1);
  zmq::socket_t publisher2s(context2s, ZMQ_PUB);
  publisher2s.bind("tcp://127.0.0.1:7698"); //52
  

  //For channel mapping                                                                                                                                                                            

  // int CRATE=strtol(argv[2], nullptr, 0); //Change for SEB                                                             
                                                    
  int CRATE=strtol(argv[1], nullptr, 0); //Change for SEB  
   // int CRATE = 2;             
  std::vector<ForChannelMap> holdChMapIds;
  std::ifstream mapFile;
  mapFile.open("chmapnew.txt");

  std::ofstream outputFile;
  outputFile.open(argv[2]);
  //outputFile.open("tp2.txt");

  std::ofstream outputFile1;
  outputFile1.open(argv[3]);

  std::ofstream tptimeFile;
  tptimeFile.open(argv[4]);

  std::ofstream outFile;
  outFile.open(argv[5]);

  std::vector<ForBaseline> holdsebpeds;
  std::ifstream pedFile;
  pedFile.open("baseline.txt");
  //  int pedestal;                                                                                                                       
  //change in different seb files
  int myseb=9;

  int sebfemchped,sebfemch, sebfem, seb, baseline,countflipbit;
  countflipbit=0;
  if(pedFile.is_open() && !pedFile.eof()) {
    while(pedFile >> seb >> sebfem >> sebfemch >> sebfemchped ){  // FemId >> ChnlId){                                                   \
                                                                                                                                          
      //      holdChMapIds.push_back({crate,femchmap,ch,LArWire});                                                                       \
                                                                                                                                          
      holdsebpeds.emplace_back(seb, sebfem,sebfemch,sebfemchped);
    }
  }
  pedFile.close();

  

  string plane;
  int LArWire;
  int mwire;
  int crate, femchmap, ch;
  int savetd;
  int savetd2;
  int savedr;
  int savedr2;
  uint32_t savefr,savefr2;
  int mappedwire;
  if(mapFile.is_open() && !mapFile.eof()) {
    while(mapFile >> plane >> LArWire >> crate >> femchmap >> ch ){  // FemId >> ChnlId){                                                                                                          
      //      holdChMapIds.push_back({crate,femchmap,ch,LArWire});                                                                                                                                 
      holdChMapIds.emplace_back(crate,femchmap,ch,LArWire);
    }
  }
  mapFile.close();


  std::vector<PassInfoToStitching> tp_list ;
  //Read binary code starts                                                                                                                                                                        
  int k=-1;
  int femHdrCount=0;
  uint32_t frame = 0;
  uint32_t minFrame;
  int FrameCounter = 0;
  uint32_t frame1, frame2, frame3, frame4, framebitsToskip;
  uint32_t fem;
  int channel,channel1, mappedchannel;
  uint16_t timetick=0;

  uint64_t adcval, amp, intgrl; // intgrlval;                                                                                                             
  uint64_t limit=100;
  uint64_t limit2=20;
  int diff,adcdiff,nearestPower;
  int samplecount;
  int tot;
  uint64_t adcvalremainder, prevadcval;// baseline;
  //  adcvalremainder=0;
  //adcval=480;
  //baseline=480;

                                          
  int run,subrun;

  std::ifstream binFile;
  bool doincrement=false;
  bool proceed=false;

  //binFile.open("/home/uboonedaq/kalra_seb01/Files/test2.ubdaq", std::ios::binary);

  // binFile.open(argv[1], std::ios::binary); //  | std::ios::ate);                         
                          


  //change seb number in diifferent instances
 std::string pattern = "seb09";
 // const char* directory = "/data/kalra_TPCTriggerFiles/Nominal54/"; // "/home/uboonedaq/kalra_seb01/Files/54/";
 const char* directory = "/data/kalra_TPCTriggerFiles/Nom_plus_0/";
 DIR* dir = opendir(directory);
 if (dir == nullptr) {
   std::cerr << "Error opening directory." << std::endl;
   return 1;
   // break;                                                                                                                       
 }
 std::string fileExtension = ".ubdaq";
 std::vector<std::string> filenames;


 struct dirent* entry;
 while ((entry = readdir(dir)) != nullptr) {
   std::string fileName = entry->d_name;

   // Check if the filename has the desired file extension                                                                         
   if (fileName.size() >= fileExtension.size() &&
       fileName.substr(fileName.size() - fileExtension.size()) == fileExtension) {
     // Check if the filename contains the pattern "seb02"                                                                         
     if (fileName.find(pattern) != std::string::npos) {
       // Extract the SEB number from the filename                                                                                 
       //       std::string sebNumber = fileName.substr(3, 2);
       filenames.push_back(fileName);
     }}}
 std::sort(filenames.begin(), filenames.end(), naturalSortComparator);

 bool goStitch=false;


 auto startf = std::chrono::system_clock::now();
 for (const auto& filename : filenames) {
   
   std::cout << "Found file: " << filename <<  std::endl;   
       //   std::ifstream binFile;
       binFile.open("/data/kalra_TPCTriggerFiles/Nom_plus_0/"+filename, std::ios::binary);
       auto endf = std::chrono::system_clock::now();
       auto elapsedf = std::chrono::duration_cast<std::chrono::microseconds>(endf - startf);
       std::cout << "time to open a file: " << std::dec << elapsedf.count() << " microseconds" << std::endl;

       std::string runNumber = filename.substr(42,5);                                                                                                           
       run = std::stoi(runNumber);                                                                                                                                 
       std::string subrunNumber = filename.substr(48,5);                                                                                                           
       subrun = std::stoi(subrunNumber);                                                                                                                           
       //       std::cout << "Run: " << run <<  " ,Subrun " << subrun << std::endl; 
           


 if( !binFile.is_open() ){
   std::cerr << "ERROR: Could not open file " << std::endl;
   return 0;
 }

  std::streamsize sizeb = binFile.tellg();
 
  int word_count = 0;

  //  std::vector<uint32_t> buffer; // Buffer to hold 32-bit words

  cout << " binary file " << binFile << endl;

  while( binFile.peek() != EOF ){
    auto start = std::chrono::system_clock::now();
    uint32_t word32b;
    binFile.read( reinterpret_cast<char*>(&word32b), sizeof(word32b) );

    std::cout.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield                                                                                                            
    std::cout.setf ( std::ios::showbase ); // activate showbase    i.e with prefix 0x                                                                                                              

    uint16_t first16b = word32b & 0xffff; //right 16 bit word                                                                                                                                      
    uint16_t last16b = (word32b>>16) & 0xffff; // left 16 bit word                                                                                                                                 

    //    std::cout << "32-bit word- " << word32b << " \t" << "made from - " << last16b << first16b << std::endl;


    // auto start = std::chrono::system_clock::now();
    //    cout << "Initial : "<<std::dec << start.time_since_epoch().count()  << endl;                                                                                                     
    if(word32b == 0xffffffff) {
      cout << "Start of frame ********************" << endl;
      femHdrCount=1;
      doincrement=true;
      start = std::chrono::system_clock::now();
      //cout << "tplist at frame beginning: " << std::dec << tp_list.size() << endl;
    }
    else if (word32b  == 0xe0000000){
     cout << "End of frame*******************" << endl;
      auto end = std::chrono::system_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      //      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      std::cout << "Elapsed time (eof): " << std::dec << elapsed.count() << " microseconds" << std::endl;
      //      cout << "check frame4: " << frame4 << endl;
      if(frame4==minFrame+2){
        //Pass the tp_list to next stitching code and then clear the list, for now checking the size of tp_list here and then clearing the tp_list                                                 
        // --> Need to put interfacing code here                                                                                                                                                   
        cout << "Size of tp_list vector at the end of 3 frames : " << std::dec << tp_list.size() << endl;

	goStitch=true;
	//	cout << "GoStitchTrue 1" << endl;
             }

             else if (frame4!=minFrame+2 and frame4==minFrame+5){

	       cout << " second Size of tp_list vector at the end of 3 frames : " << tp_list.size() << endl;
        goStitch=true;
        //cout <<"GoStitchTrue 2" << endl;

	 }

	if(goStitch==true){
	  //cout << "hello" << endl;

	  //Stitching code starts here -
	  std::vector<int> readTP;
	  int min1p=0;
	  int cratenum=2;
	  int tmin=0;
	  int tmax=3200;
	  int tdrift=4600;
	  int tdiff=tmax-tmin;
	  int tDiff=tdrift-tdiff;

	  std::vector<PassInfoToAlg> stitchedTP;
	  std::vector<int> thisFrameList;
	  int td=0;
	  int tmin2=0;
	  int tdiff2=tmax-tmin;
	  int tDiff2=tdrift-tdiff2;
	  int tdiffa=tmax-tmin;
	  int tDiffa=tdrift-tdiffa;
	  bool FirstTime=true;
	  bool changeMin=false;
	  std::vector<int> channellist2;
	  std::vector<PassInfoToAlg> stitchedTP2;
	  int tminp=0;
	  int frmcntby3 = 3;
	  int frmcntby0 = 0;
	  int tmin_eo1 = 0;
	  int tdiff_eo1 = tmax-tmin_eo1;
	  int tDiff_eo1=tdrift-tdiff_eo1;

	  int tmin_eo2 = 0;
	  int tdiff_eo2 = tmax-tmin_eo2;
	  int tDiff_eo2=tdrift-tdiff_eo2;

	  int tmin_eo3 = 0;
	  int tdiff_eo3 = tmax-tmin_eo3;
	  int tDiff_eo3=tdrift-tdiff_eo3;
	  int tdiff_sf1 = tmax-tmin_eo1;
	  int tDiff_sf1=tdrift-tdiff_eo1;
	  int minp2 = 0;
	  int tdiffP =tdiff;
	  std::vector<int> hitlist;
	  int colplstart = 2976;
	  int step = 96;
	  std::vector<PassInfoToAlg> stitchedTP_part1;
	  std::vector<PassInfoToAlg> stitchedTP_part2;

	  int tdiffP2=tdiff;
	  int tdiffP3=tdiff;
	  string runa;
	  string subruna;

	  //	  std::vector<PassInfoToAlg> stitchedTP_tot;


	  for (const auto& elem : tp_list) {
	    //cout << "check elem: " << elem.mappedchannel << " , " << elem.timetick << " , " <<  elem.tot<< endl;
	    //	    if(elem.mappedchannel!=0 and elem.timetick!=0 and elem.tot!=0 and elem.intgrl!=0){
	    min1p=elem.minFrame;
	    //cout << "frame: " << elem.frame4 << "min frame: " << elem.minFrame << endl;
            if(elem.frame4==min1p){
              if(FirstTime==true){
                // cout << "first td and min1p, tdiff, tDiff : " <<  td << " , " << min1p << " , " << readTP[4] << " , " << tdiff << " , " << tDiff << endl;                   
                td=min1p;
                tmin2=0;
                tminp=tmin;
                tmin=tmin2;
                tmax=3200;
                tdrift=4600;
                tdiff=tmax-tmin;
                tdiff_sf1 =tdiff;
                tDiff=tdrift-tdiff;
                tDiff_sf1 =tDiff;
		if(elem.timetick>=tmin && elem.timetick<=tmax){
		  if (td==min1p){
		    //	    cout <<"tot check" << elem.tot << endl;
                    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);

                    cout << "size of stitchTPPart1 t=minp: " << stitchedTP_part1.size() << endl;                                                                             
                  }

                  else if (td==min1p+1){
                    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  tmin_eo1=tDiff;
                  tmax=3200;
                  tdiff_eo1=tmax-tmin_eo1;
                  tDiff_eo1=tdrift-tdiff_eo1;
		}}
              else{ // if not first time
		td=min1p;
                tmin=tmin_eo3;
                tmax=3200;
                tdiff=tdiff_eo3; //#tmax-tmin #tdiff2                                                                                                                          
                tdiff_sf1 = tdiff;
                tDiff=tDiff_eo3;// #tdrift-tdiff #tDiff2                                                                                                                       
                tDiff_sf1 = tDiff;
		if(stitchedTP2.size() != 0){
		  if (td==min1p){
		    stitchedTP_part1.insert(stitchedTP_part1.end(),stitchedTP2.begin(),stitchedTP2.end());
		    //stitchedTP_part1.extend(stitchedTP2)                                                                                                                   
		    //   cout << "Extended size of stitchTPPart1 : " << stitchedTP_part1.size() << endl;                                                                     
		  }
		  else if(td == min1p+1){
		    stitchedTP_part2.insert(stitchedTP_part1.end(),stitchedTP2.begin(),stitchedTP2.end());
		    //stitchedTP_part2.extend(stitchedTP2)                                                                                                                   
		    //cout << "Extended size of stitchTPPart2 : " << stitchedTP_part2.size() << endl;                                                                        
		    stitchedTP2.clear();
		  }
		  //tried this also stitchedTP2.clear();                                                                                                                     
                }
                if(tDiff==3200){
                  if(elem.timetick >= tminp && elem.timetick <= tDiff){
		    if (td==min1p){
                      stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }

                    else if (td==min1p+1){
                      stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }
		    tmin_eo1=tminp;
		    tmax=3200;
		    tdiff_eo1=tmax-tmin_eo1;
		    tDiff_eo1=tdrift-tdiff_eo1;
		    //cout << "end third  td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                        
		  }}

		else if(tDiff<3200){
		  if(elem.timetick >= tminp && elem.timetick <= tDiff){
		    if (td==min1p){
		      stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }

		    else if (td==min1p+1){
		      stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }
		    tmin_eo1=tDiff;
		    tmax=3200;
		    tdiff_eo1=tmax-tmin_eo1;
		    tdiff_sf1 = tdiff_eo1;
		    tDiff_eo1=tdrift-tdiff_eo1;
		    tDiff_sf1 = tDiff_eo1;
		  }
		  else{
		    td=min1p+1;
		    if (td==min1p){
		      stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }
		    else if (td==min1p+1){
		      stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }

		    td=min1p;
		    tmin_eo1=tDiff;
		    tmax=3200;
		    tdiff_eo1=tmax-tmin_eo1;
		    tdiff_sf1 = tdiff_eo1;
		    tDiff_eo1=tdrift-tdiff_eo1;
		    tDiff_sf1 = tDiff_eo1;
		  }

		}

		else if(tDiff>3200){
		  tmin2=tminp;
		  tmin=tmin2;
		  tmax=3200;
		  tdiffP = tdiff;
		  tdiff=tdiff+(tmax-tmin);
		  tdiff_sf1 = tdiff;
		  tDiff=tdrift-tdiff;
		  tDiff_sf1 = tDiff;
		  if(elem.timetick >= tmin && elem.timetick <= tmax){
		    if (td==min1p){
		      stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }

		    else if (td==min1p+1){
		      stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		    }
		    td=min1p;
		    tmin_eo1=tDiff;
		    tmax=3200;
		    tdiff_eo1=tmax-tmin_eo1;
		    tDiff_eo1=tdrift-tdiff_eo1;
		  }

		}//end of else if(tDiff>3200)                                                                                                                                

              }//end of else if not first time                                                                                                                                 
	    } // end of readTP==min1p          
	    if(elem.frame4 == min1p+1){
	      FirstTime=false;
	      //cout << "starting next frame: "  << endl;                                                                                                                         
	      tmin2=tminp;
	      tmin=tmin2;
	      tmax=3200;
	      tdiff= tdiff_sf1;
	      tdiffP2 = tdiff;
	      tDiff= tDiff_sf1;
	      //cout << "7th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                          
	      if(tDiff==3200){
		if(elem.timetick >= 0 && elem.timetick <= tDiff){
		  // cout << "7th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                     

		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);

		  }
		  tmin_eo2=tminp;
		  tmax=3200;
		  tdiff_eo2=tmax-tmin_eo2;
		  tDiff_eo2=tdrift-tdiff_eo2;
		  //cout << "end 7th  td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                 
		}
	      }//end of if tDiff=3200       
	      else if(tDiff < 3200){

		if(elem.timetick >= tminp && elem.timetick <= tDiff){
		  //cout << "8th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                      

		  if (td==min1p){
		    //              stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum);    
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  //cout << "3rd  size of stitchTPPart1 t=minp: " << stitchedTP_part1.size() <<  " , Part2: " << stitchedTP_part2.size() <<  endl;                                

		  tmin_eo2=tmin_eo1;
		  tmax=3200;
		  tdiff_eo2=tmax-tmin_eo2;
		  tDiff_eo2=tdrift-tdiff_eo2;
		}
		else{

		  td=min1p+1;

		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  td=min1p;
		  tmin_eo2=tmin_eo1;
		  tmax=3200;
		  tdiff_eo2=tmax-tmin_eo2;
		  tDiff_eo2=tdrift-tdiff_eo2;
		}

	      }//end of else if tDiff<3200                                                                                                                                        

	      else if(tDiff>3200){

		td=min1p+1;
		tmin2=tminp;
		tmin=tmin2;
		tmax=3200;
		tdiff=tdiff+(tmax-tmin);
		tDiff=tdrift-tdiff;
		//cout << "10th  td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                      
		if(elem.timetick >= tmin && elem.timetick <= tmax){
		  //  cout << "10th  td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                  
		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP2, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP2, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  td=min1p;
		  tmin_eo2=tminp;
		  tmax=3200;
		  tdiff_eo2=tdiff;
		  tDiff_eo2=tDiff;
		}
	      }//end of else if tDiff>3200                                                                                                                                        
	    }//end of readTP==min1p +1 

	    if(elem.frame4 == min1p+2){
	      tmin=tmin_eo2;
	      tmax=3200;
	      tdiff=tdiff_eo2;
	      tdiffP3=tdiff;
	      tDiff=tDiff_eo2;
	      //cout << "11th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                       
	      if(tDiff==3200){
		if(int(elem.timetick)>=tminp and  int(elem.timetick)<=tDiff){

		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  // cout << "6th size of stitchTPPart1 t=minp: " << stitchedTP_part1.size() <<  " , Part2: " << stitchedTP_part2.size() <<  endl;                              
		  tmin_eo3=tminp;
		  tmax=3200;
		  tdiff_eo3=tmax-tmin_eo3;
		  tDiff_eo3=tdrift-tdiff_eo3;
		}
	      }//end of tdiff==3200                  
	      else if(tDiff<3200){
		if(elem.timetick >= tminp && elem.timetick <= tDiff){
		  td=min1p+1;
		  //cout << "inc.td 12th td and min1p : "<<  td << " , "<< min1p  << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                           
		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  //cout <<"7th size of stitchTPPart1 t=minp: " << stitchedTP_part1.size() <<  " , Part2: " << stitchedTP_part2.size() <<  endl;                                

		  td=min1p;
		  tmin_eo3=tDiff;
		  tmax=3200;
		  tdiff_eo3=tmax-tmin_eo3;
		  tDiff_eo3=tdrift-tdiff_eo3;
		}

		else{
		  //cout << "initital 13th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                          
		  td=min1p+3;

		  //cout << "13th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                   
		  stitchedTP2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick-tDiff, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  td=min1p;
		  tmin_eo3=tDiff;
		  tmax=3200;
		  tdiff_eo3=tmax-tmin_eo3;
		  tDiff_eo3=tdrift-tdiff_eo3;

		}//end of else                                                                                                                                          

	      }//end of elseif tDiff<3200                                                                                                                              



	      else if(tDiff > 3200){

		tmin2=tminp;
		tmin=tmin2;
		tmax=3200;
		tdiff=tdiff+(tmax-tmin);
		tDiff=tdrift-tdiff;
		if(elem.timetick >= tmin && elem.timetick <= tmax){
		  //cout << "14th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                   

		  if (td==min1p){
		    stitchedTP_part1.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP3, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }

		  else if (td==min1p+1){
		    stitchedTP_part2.emplace_back(elem.frame4,elem.fem, elem.mappedchannel, elem.timetick+tdiffP3, elem.tot, elem.amp,elem.intgrl,td, cratenum,run,subrun);
		  }
		  //cout << "8th size of stitchTPPart1 t=minp: " << stitchedTP_part1.size() <<  " , Part2: " << stitchedTP_part2.size() <<  endl;                               

		  td=min1p;
		  tmin_eo3=tDiff;
		  tmax=3200;
		  tdiff_eo3=tmax-tmin_eo3;
		  tDiff_eo3=tdrift-tdiff_eo3;
		}
	      } //end of else if tDiff>3200                                                                                                                                     

	    }//end of readTP==min1p +2                                                                                                                                          
	    //cout << "end 14th td and min1p : "<<  td << " , "<< min1p << " , " << readTP[4] << " , " <<  tdiff << " , " << tDiff << endl;                                     
	  } // end loop on tp_list                                                                                                                                           

	  cout << "Size of tp_list: " << tp_list.size() << endl;
	  cout << "Size of stitchedTP_part1: " << stitchedTP_part1.size() << endl;                                                                                             
	  cout << "Size of stitchedTP_part2: " << stitchedTP_part2.size() << endl;                                                                                                   
	  cout << "size: " << stitchedTP_part1.size()+stitchedTP_part2.size()+stitchedTP2.size() << endl;    

	  //	  std::cout << "First savedr: " << savedr << " , " << savedr2 << std::endl;

	  if(stitchedTP_part1.size()>1 and stitchedTP_part2.size()>1){
	    savedr=stitchedTP_part1[0].tda;
	    savedr2=stitchedTP_part2[0].tda;
	   
	    savefr=stitchedTP_part1[0].frame4a;
	    savefr2=stitchedTP_part2[0].frame4a;
	   
	    std::cout << "savedr 1 : " << savedr << " , " <<savedr2<< std::endl;   
	    zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
            memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
            publisher.send(message1);

	    zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
            memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
            publisher2s.send(message2);

	  }


	  else if (stitchedTP_part1.size()==1 and stitchedTP_part2.size()>1 )
	    {
	      stitchedTP_part1[0].tda=savedr+3;
	      stitchedTP_part1[0].frame4a=savefr+1;
	      std::cout << "st: " <<  stitchedTP_part1[0].tda << " , " <<  stitchedTP_part1[0].frame4a << std::endl;
	      savedr2=stitchedTP_part2[0].tda;
	      std::cout << "savedr 2 : " << savedr << " , " <<savedr2<< std::endl;
	      zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
              memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
              publisher.send(message1);

	      zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
              memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
              publisher2s.send(message2);

}
	      else if (stitchedTP_part1.size()==0 and stitchedTP_part2.size()>1 )
	      {
		stitchedTP_part1.emplace_back(savefr+1,0, 0, 0, 0, 0,0,savedr+3, cratenum,run,subrun);
	      
		savedr2=stitchedTP_part2[0].tda;
		std::cout << "savedr 2 : " << savedr << " , " <<savedr2<< std::endl;
		zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		publisher.send(message1);
		
		zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
		memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
		publisher2s.send(message2);
	    }



	  else if (stitchedTP_part1.size()>1 and stitchedTP_part2.size()==1 )
            {
	     
		stitchedTP_part2[0].tda=savedr2+3;
		stitchedTP_part2[0].frame4a=savefr2+1;
		savedr=stitchedTP_part1[0].tda;
		std::cout << "savedr 3 : " << savedr << " , " <<savedr2<< std::endl;
		zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		publisher.send(message1);

		zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
		memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
		publisher2s.send(message2);
 
	    }
	  else if(stitchedTP_part1.size()>1 and stitchedTP_part2.size()==0){
                stitchedTP_part2.emplace_back(savefr2+1,0, 0, 0, 0, 0,0,savedr2+3, cratenum,run,subrun);
              
		savedr=stitchedTP_part1[0].tda;
		std::cout << "savedr 3 : " << savedr << " , " <<savedr2<< std::endl;
		zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
		publisher.send(message1);

	      zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
              memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
              publisher2s.send(message2);
            }

	  else{

	    //	    stitchedTP_part2[0].tda=savedr2+3;
	    //stitchedTP_part1[0].tda=savedr+3;
	    //stitchedTP_part2[0].frame4a=savefr2+1;
            //stitchedTP_part1[0].frame4a=savefr+1;
	    stitchedTP_part2.emplace_back(savefr2+1,0, 0, 0, 0, 0,0,savedr2+3, cratenum,run,subrun);
	    stitchedTP_part1.emplace_back(savefr+1,0, 0, 0, 0, 0,0,savedr+3, cratenum,run,subrun);
	    std::cout << "savedr 4 : " << savedr << " , " <<savedr2<< std::endl;
	    zmq::message_t message1(sizeof(PassInfoToAlg) * stitchedTP_part1.size());
	    memcpy(message1.data(), stitchedTP_part1.data(), sizeof(PassInfoToAlg) * stitchedTP_part1.size());
	    publisher.send(message1);

	    zmq::message_t message2(sizeof(PassInfoToAlg) * stitchedTP_part2.size());
	    memcpy(message2.data(), stitchedTP_part2.data(), sizeof(PassInfoToAlg) * stitchedTP_part2.size());
	    publisher2s.send(message2);
	  }



	  //	  std::cout << "savedr 1 : " << savedr << " , " <<savedr2<< std::endl;
	  //std::cout << "savedr 2: " << savedr << " , " <<savedr2<< std::endl;


	  auto end2dr = std::chrono::system_clock::now();
	  auto elapsed2dr = std::chrono::duration_cast<std::chrono::microseconds>(end2dr - start);
	  std::cout << "Sent 2 drift regions. : " << std::dec << elapsed2dr.count() << " microseconds" << std::endl;

	  cout << "Size of tp_list: " << tp_list.size() << endl;


	  for(const auto& elem: tp_list){     
	    outputFile << elem.minFrame << " \t" << elem.frame4  << " \t" <<   elem.fem  << " \t" <<   elem.mappedchannel    << " \t" << elem.timetick    << " \t" <<  elem.tot << " \t" << elem.amp << " \t" << elem.intgrl <<  " \n" ; //<< elem.run << " \t" << elem.subrun << " \n";  
	  }


          if(stitchedTP_part2.size()>1 and stitchedTP_part1.size()>1){
            savetd=0;
            savetd2=0;
            //std::cout << "stda " << stitchedTP_part1[1].tda << std::endl;                                                                                                                                                                                                                     

            tptimeFile << stitchedTP_part1[1].tda << " \t" <<   stitchedTP_part1.size() << " \t" << stitchedTP_part2[1].tda << " \t" << stitchedTP_part2.size() << "\t" << elapsed2dr.count() << "\n";

            savetd=stitchedTP_part1[1].tda;
            savetd2=stitchedTP_part2[1].tda;

	    std::cout << "save td 1: " << savetd << " , " << savetd2 << std::endl;

          }
          else if (stitchedTP_part1.size()<=1 and stitchedTP_part2.size()>1 )
            {
              savetd=savetd+3;
              savetd2=stitchedTP_part2[1].tda;
              tptimeFile << savetd << " \t" <<   stitchedTP_part1.size() << " \t" << stitchedTP_part2[1].tda << " \t" << stitchedTP_part2.size() << "\t" << elapsed2dr.count() << "\n";
	      std::cout << "save td 2: " << savetd << " , " << savetd2 << std::endl;
            }
          else if (stitchedTP_part2.size()<=1 and stitchedTP_part1.size()>1 )
            {
              savetd=stitchedTP_part1[1].tda;
              savetd2=savetd2+3;

              tptimeFile << stitchedTP_part1[1].tda << " \t" <<   stitchedTP_part1.size() << " \t" << savetd2 << " \t" << stitchedTP_part2.size() << "\t" << elapsed2dr.count() << "\n";
              //              savetd=stitchedTP_part1[1].tda;                                                                                                                                                                                                                                   
              //savetd2=savetd2+3;                                                                                                                                                                                                                                                              
	      std::cout << "save td 3 : " << savetd << " , " << savetd2 << std::endl;

            }
          else{ // if (stitchedTP_part1.size()<=1 and stitchedTP_part2.size()<=1) {                                                                                                                                                                                                            \
                                                                                                                                                                                                                                                                                                

	    std::cout << "save td 4 : " << savetd << " , " << savetd2 << std::endl;
            tptimeFile << savetd+3 << " \t" <<   stitchedTP_part1.size()  << " \t" << savetd2+3 << " \t" << stitchedTP_part2.size() << "\t" << elapsed2dr.count() << "\n";
          }



	  if(stitchedTP_part1.size()!=0){

	  for (const auto& elem : stitchedTP_part1) {                                                                                                                                
	    outputFile1 << elem.tda << " \t" << elem.frame4a << " \t" << elem.fema << " \t" << elem.mappedchannela << " \t" << elem.timeticka << " \t" << elem.tota << " \t" << elem.ampa << " \t" << elem.intgrla << " \n";                                                                                                                                       
                                                                                                                                                                               
	  }                                                                                                                                                                 }         
              
	  if(stitchedTP_part2.size()!=0){   //here tota
	  for (const auto& elem : stitchedTP_part2) {
            outputFile1 << elem.tda << " \t" << elem.frame4a << " \t" << elem.fema << " \t" << elem.mappedchannela << " \t" << elem.timeticka << " \t" << elem.tota << " \t" << elem.ampa << " \t" << elem.intgrla << " \n";

          }
	  }
	  tptimeFile.flush();
	  outputFile.flush();
	  outputFile1.flush();

	  // Clear received data                                                                                                                                                                                
	  tp_list.clear();
          FrameCounter=0;
       	  stitchedTP.clear();                                                                                                                                                     
	  stitchedTP_part1.clear();                                                                                                                                               
	  stitchedTP_part2.clear();    
	  std::cout << "gostitch false " << std::endl;
	  goStitch=false;
	  } // end of tp_list
 
	//FrameCounter=0;
        //tp_list.clear();

	//}
    }

    else {

      if(doincrement==true){ //we need this boolean so as to stop FEM cntr after evaluating frame number.                                                                                                                                                             
        femHdrCount+=1;

        if (femHdrCount==5){ // FEM frame number                                                                                                                                                                                                                      
          frame = (first16b & 0xfff);
          frame1 = (last16b>>6 & 0x3f);
          framebitsToskip = (last16b & 0x3f);   // last 6 bits of FEM frame number                                                                                                                                                                                    
        }

        if (femHdrCount==8){ //ROI frame number                                                                                                                                                                                                                       
          if(first16b>>12 == 0x1){
            frame2=(first16b>>6) & 0x3f;
            //Take care of roll over of bits                                                                                                                                                                                                                          
            if ((framebitsToskip-frame2)>=0){
              frame3 = (frame<<12)+(frame1<<6)+(frame2);
              frame4 = (frame3 & 0xffffff);
	      //std::cout <<"Frame: " <<  std::dec << frame4 << std::endl;     
	      if(frame4>1){  //!=0){                                                                                                                                                                                                                                  
		FrameCounter+=1;
              }
              if(FrameCounter==1){
                minFrame = frame4;
                //cout << std::dec << minFrame << endl;                                                                                                                                                                                                               
	      }

	    }
            else if ((framebitsToskip-frame2)<0){
	      frame3 = (frame<<12)+(frame1<<6)+(frame2);
            }

            k+=1;
            if (k!=(frame3 & 0xffffff)){
              //std::cout << "************* Missing frame **********" << std::endl;                                                                                                                                                                                   
              // std::cout << k << " , " << (frame3 & 0xffffff) << std::endl;                                                                                                                                                                                         
              k=(frame3 & 0xffffff);
            }

            doincrement=false;
            femHdrCount=0;

          }

          else if(last16b>>12 == 0x1){
            frame2=(last16b>>6) & 0x3f; // skip first 6 bits and then take 6 bits to count                                                                                                                                                                            
            if ((framebitsToskip-frame2)>=0){
              frame3 = (frame<<12)+(frame1<<6)+(frame2);
              //std::cout << "LH frame :***** " << std::dec << frame4 << std::endl; // (frame3 & 0xffffff) << std::endl;                                                                                                                                              
            }

            if ((framebitsToskip-frame2)<0){
              //std::cout << "Roll Over" <<std::endl;                                                                                                                                                                                                                 
              //std::cout << "diff. : " << (framebitsToskip-frame2)<< " and dec is: " << std::dec <<  (framebitsToskip-frame2) << std::endl;                                                                                                                          
              frame3 = (frame<<12)+(frame1<<6)+(frame2);
              frame4 = (frame3 & 0xffffff);
	      //std::cout<<"Frame L: " <<  std::dec << frame4 << std::endl;
	      if(frame4>1){ //!=0){                                                                                                                                                                                                                                   
                FrameCounter+=1;
              }
              if(FrameCounter==1){
                minFrame = frame4;
		//                cout <<std::dec << minFrame <<endl;


              }


            }
            k+=1;
            if (k!=(frame3 & 0xffffff)){
              //std::cout << "************* Missing frame **********" << std::endl;                                                                                                                                                                                   
              k=(frame3 & 0xffffff);
            }
            doincrement=false;
            femHdrCount=0;
          }

        } //close femhdrcnt == 8                                                                                                                                                                                                                                      

      } //close do increment loop                                                                                                                                                                                  
      //      else{ // if doincrement is not true                                                 

      if ((last16b >>8 == 0xf1) and (first16b == 0xffff)){
        fem =(last16b&0x1f);
	// std::cout << "FEM number : " << std::dec << fem << std::endl;                                                                                                                                                                                               
      }

      if(first16b>>12 == 0x1){
        channel = (first16b & 0x3f);
	//std::cout <<  "Channel Number: " << std::dec << channel << std::endl;
	//std::cout << CRATE << std::endl;
	mappedchannel = returnWire(CRATE,fem,channel,holdChMapIds);
	//std::cout <<  "larsoft Channel Number: " << " \t" << mappedchannel << std::endl;
	//std::cout << "Start of channel : " << std::dec << channel << std::endl;

	baseline = returnbaseline(myseb,fem,channel,holdsebpeds);
	//std::cout <<  "Pedestal for channel is:  " << std::dec << baseline << std::endl;


        tot=0;
        intgrl=0;
        amp=0;
	samplecount=0;
	//	prevadcval=adcval;
	//baseline=adcval;

      }

      else if (first16b>>14 == 01){
	//cout << " ******* beg. of waveform " << endl;
	proceed=true;
	//	cout << "TP: " << std::dec  << tot<< " , " << amp<< " , " << intgrl << endl;
	
	/*if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){ // and amp<4097){
	  std::cout <<"************* First 16 bits loop *********" << std::endl;
	  std::cout << "Check Frame: " <<std::dec << frame4 << std::endl; //hex_to_decimal_32u(frame4) << std::endl;                                                                                 
	  std::cout << "FEM number : " << std::dec << fem << std::endl;
	  std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	  std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	  std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	  std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	  std::cout << "tot: " <<  tot << std::endl;
	  std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	  std::cout << "time: " << std::dec << timetick << std::endl;
	  
          if(channel>31 and channel<64){
	    cout << "filling tplist" <<endl;
	    tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);
	  } }
	*/
	samplecount=0;      
	tot=0;
	intgrl=0;
	amp=0;
       
	timetick =  first16b & 0x3fff ;
	// std::cout << "time: " << std::dec << timetick << std::endl;                          
	//std::cout << "TPs:  " << amp << " tot: " << tot << " integral: " << intgrl << std::endl;    
	prevadcval=baseline; //adcval;
	//baseline=adcval;
	//std::cout << "prev at start of waveform: " << prevadcval << std::endl;
	//outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
      }

      if(last16b>>12 == 0x1){
        channel = (last16b & 0x3f);
        //std::cout <<  "Channel Number: " << std::dec << channel << std::endl;                                                                                                                                                      
        //std::cout << CRATE << std::endl;                                                                                                                                                                                           
        mappedchannel = returnWire(CRATE,fem,channel,holdChMapIds);
	//std::cout <<  "larsoft Channel Number: " << " \t" << mappedchannel << std::endl;
        //std::cout << "Start of channel : " << std::dec << channel << std::endl;                                                                                                                                                    
	baseline = returnbaseline(myseb,fem,channel,holdsebpeds);
	//std::cout <<  "Pedestal for channel is:  " << std::dec << baseline << std::endl;



        tot=0;
        intgrl=0;
        amp=0;
        samplecount=0;
	//        prevadcval=adcval;
        //baseline=adcval;
	//outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
      }

     
      else if (last16b>>14 == 01){
	//	cout <<" ******* last beg. of waveform " <<endl;
	//cout << "TP: "<< std::dec  << tot<< " , " << amp<< " , " << intgrl << endl;
        proceed=true;
	//if(proceed==true){                                               

	/*	if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){ // and amp<4097){                                                                                       	  std::cout <<"************* Last 16 bits loop *********" << std::endl;
	  std::cout << "Check Frame: " << std::dec << frame4 << std::endl; //hex_to_decim                                                                          
	  std::cout << "FEM number : " << std::dec << fem << std::endl;
	  std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	  std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	  std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	  std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	  std::cout << "tot: " <<  tot << std::endl;
	  std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	  std::cout << "time: " << std::dec << timetick << std::endl;

          if(channel>31 and channel<64){
            cout << "filling tplist" << endl;
            tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                    
          }
	} 
	*/                                                                                                                                                
    	tot=0;
        intgrl=0;
        amp=0;
	samplecount=0;

	timetick =  last16b & 0x3fff ;
	prevadcval=baseline; //adcval;
	//	baseline=adcval;
	//outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
      }

      if(proceed==true){ 
      
       if (first16b>>12 == 0x2){
	tot+=1;
	adcval=(first16b & 0xfff);
	//std::cout << "ADC value a1: " << std::dec << adcval << std::endl;  
	if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	  diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	  nearestPower = nearestPowerOf2(diff);
	  //std::cout <<std::dec << "adc val a1: " << adcval << " \t" << prevadcval << std::endl;
	  //std::cout << "diff a1: " << std::dec << diff << std::endl;
	  //std::cout << "nearest power a: " << nearestPower << std::endl;
	  adcval = abs(adcval-nearestPower);
	  prevadcval=adcval;
	  //std::cout << "new adc cval a1: " << std::dec << "adc: " <<adcval << " , baseline: " << baseline << std::endl;
	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << " 1 remainder: " << std::dec<< adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	  if(adcvalremainder>amp){
	    amp=adcvalremainder;
	    //std::cout << "Amplitude 1: " << amp << std::endl;
	  }
	  intgrl +=adcvalremainder;
	}

	else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	  adcval=prevadcval; //adcval-10;                                                                                                                                   

	  prevadcval=adcval;
	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "remainder: " << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
            //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                           
          }
          intgrl +=adcvalremainder;

	}


	else{
	  //adcval=(first16b & 0xfff);
	  prevadcval=adcval;
	  //std::cout << std::dec << "new adc a1 : " <<adcval << " , baseline: " << baseline << std::endl;
	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << " 1 remainder: " << std::dec << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
	    //std::cout << "Amplitude 1: " << amp<< std::endl;
          }
          intgrl +=adcvalremainder;

	}
	samplecount++;
      }

       else if (first16b>>15 == 1) {
	int j=0;
	for (int i = 0; i < 15; ++i) {
	  if(((first16b >> i) & 1) == 1){
	    j=i;
	    if(((first16b >> j+1) & 1) == 0 and ((first16b >> j+2) & 1) == 0 and ((first16b >> j+3) & 1) == 0 and ((first16b >> j+4) & 1) == 0 and ((first16b>> j+5) & 1) == 0 and ((first16b >> j+6) & 1) == 0) {
	      tot+=1;
	      adcval=adcval+3;
	      //  std::cout <<std::dec << "adc val a2: " << adcval << " \t" << prevadcval << std::endl;
	      if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
		diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
		nearestPower = nearestPowerOf2(diff);
		//std::cout <<std::dec << "adc val a2: " << adcval << " \t" << prevadcval << std::endl;
		//std::cout << "diff a2: " << std::dec << diff << std::endl;

		//	std::cout << "nearest power a2: " << nearestPower << std::endl;

		adcval = abs(adcval-nearestPower);
		prevadcval=adcval;
		//std::cout << "new adc cval a2: " << std::dec << "adc: " <<adcval << " , baseline: " << baseline << std::endl;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	     	//std::cout << "2 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
		  amp=adcvalremainder;
		  //std::cout << "Amplitude 2: " << amp<< std::endl;
		}
		intgrl +=adcvalremainder;
	      }

	      else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
		adcval=prevadcval; //adcval-10;                                                                                                                                   
		prevadcval=adcval;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "remainder: " << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
		  amp=adcvalremainder;
		  //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                           
		}
		intgrl +=adcvalremainder;

	      }


	      else{
		//adcval=(first16b & 0xfff);
		prevadcval=adcval;
		//std::cout << std::dec << "new adc a2 : " <<adcval << " , baseline: " << baseline << std::endl;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "2 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
		  amp=adcvalremainder;
		  //std::cout << "Amplitude 2: " << amp<< std::endl;
		}
		intgrl +=adcvalremainder;

	      }
	      samplecount++;
	    }

	    else if(((first16b >> j+1) & 1) == 0 and ((first16b >> j+2) & 1) == 0 and ((first16b >> j+3) & 1) == 0 and ((first16b >> j+4) & 1) == 0 and ((first16b>> j+5) & 1) == 0){
              tot+=1;
              adcval=adcval-3;
	      //std::cout <<std::dec << "adc val a3: " << adcval << " \t" << prevadcval << std::endl;
              if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
                diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
                nearestPower = nearestPowerOf2(diff);
		//std::cout <<std::dec << "adc val a3: " << adcval << " \t" << prevadcval << std::endl;
		//std::cout << "diff a3: " << std::dec << diff << std::endl;
		//std::cout << "nearest power a3: " << nearestPower << std::endl;
		adcval = abs(adcval-nearestPower);
                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		//std::cout << "3 remainder: " << std::dec << adcvalremainder << std::endl;
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  //std::cout << "Amplitude 3: " << amp<< std::endl;
                }
                intgrl +=adcvalremainder;
              }
	      else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
		adcval=prevadcval; //adcval-10;                                                                                                                                   

		prevadcval=adcval;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "remainder: " << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
		  amp=adcvalremainder;
		  //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                           
		}
		intgrl +=adcvalremainder;	      

	      }

              else{
                //adcval=(first16b & 0xfff);
                prevadcval=adcval;
		//std::cout << std::dec << "new adc a3 : " <<adcval << " , baseline: " << baseline << std::endl;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "3 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount <<  " \t" << adcvalremainder << " \n";
		//		std::cout << "Amplitude 3: " << amp<< std::endl;
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  //std::cout << "Amplitude 3: " << amp<< std::endl;
		}
                intgrl +=adcvalremainder;

              }
              samplecount++;
            }

	    else if(((first16b >> j+1) & 1) == 0 and ((first16b >> j+2) & 1) == 0 and ((first16b >> j+3) & 1) == 0 and ((first16b >> j+4) & 1) == 0 ){
	    tot+=1;
	    adcval=adcval+2;
	    //std::cout <<std::dec << "adc val a4: " << adcval << " \t" << prevadcval << std::endl;
	    if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	      diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	      nearestPower = nearestPowerOf2(diff);
	      //std::cout <<std::dec << "adc val a4: " << adcval << " \t" << prevadcval << std::endl;
	      //std::cout << "diff a4: " << std::dec << diff << std::endl;
	      //std::cout << "nearest power a4: " << nearestPower << std::endl;
	      adcval = abs(adcval-nearestPower);
	      prevadcval=adcval;
	      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
	      //std::cout << "4 remainder: " << std::dec << adcvalremainder << std::endl;
	      if(adcvalremainder>amp){
		amp=adcvalremainder;
		//std::cout << "Amplitude 4: " << amp<< std::endl;
	      }
	      intgrl +=adcvalremainder;
	    }

	    else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	      adcval=prevadcval; //adcval-10;                                                                                                                                                                                           

	      prevadcval=adcval;
	      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	      //std::cout << "remainder: " << adcvalremainder << std::endl;
	      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	      if(adcvalremainder>amp){
		amp=adcvalremainder;
		//std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                    \
                                                                                                                                                                                                                                          
	      }
	      intgrl +=adcvalremainder;

	    }

	    else{
	      //adcval=(first16b & 0xfff);
	      prevadcval=adcval;
	      //std::cout << std::dec << "new adc a4 : " <<adcval << " , baseline: " << baseline << std::endl;
	      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	      //std::cout << "4 remainder: " << std::dec << adcvalremainder << std::endl;
	      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	      if(adcvalremainder>amp){
		amp=adcvalremainder;
		//std::cout << "Amplitude 4: " << amp<< std::endl;
	      }
	      intgrl +=adcvalremainder;

	    }
	    samplecount++;
	    }

	    else if(((first16b >> j+1) & 1) == 0 and ((first16b >> j+2) & 1) == 0 and ((first16b >> j+3) & 1) == 0){
              tot+=1;
              adcval=adcval-2;
	      //std::cout <<std::dec << "adc val a5: " << adcval << " \t" << prevadcval << std::endl;
              if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
                diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
                nearestPower = nearestPowerOf2(diff);
		//std::cout <<std::dec << "adc val a5: " << adcval << " \t" << prevadcval << std::endl;
		//std::cout << "diff a5: " << std::dec << diff << std::endl;
		//std::cout << "nearest power a5: " << nearestPower << std::endl;
                adcval = abs(adcval-nearestPower);
                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "5 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  // std::cout << "Amplitude 5: " << amp<< std::endl;
}
                intgrl +=adcvalremainder;
              }

              else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
                adcval=prevadcval; //adcval-10;                                                                                                                                                                                           

                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "remainder: " << adcvalremainder << std::endl;
                outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
                  amp=adcvalremainder;
                  //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                                                                                                                                                                                                                             
                }
                intgrl +=adcvalremainder;

              }


              else{
                //adcval=(first16b & 0xfff);
                prevadcval=adcval;
		//std::cout << std::dec << "new adc a5 : " <<adcval << " , baseline: " << baseline << std::endl;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	       	//std::cout << "5 remainder: " << std::dec <<  adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  //std::cout << "Amplitude 5: " << amp<< std::endl;
                }
                intgrl +=adcvalremainder;

              }
              samplecount++;
            }

	    else if(((first16b >> j+1) & 1) == 0 and ((first16b >> j+2) & 1) == 0){
              tot+=1;
              adcval=adcval+1;
	      //std::cout <<std::dec << "adc val a6: " << adcval << " \t" << prevadcval << std::endl;
              if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
                diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
                nearestPower = nearestPowerOf2(diff);
		//std::cout <<std::dec << "adc val a6: " << adcval << " \t" << prevadcval << std::endl;
		//std::cout << "diff a6: " << std::dec << diff << std::endl;
		//std::cout << "nearest power a6: " << nearestPower << std::endl;
		adcval = abs(adcval-nearestPower);
                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		///	std::cout << "6 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  //std::cout << "Amplitude 6: " << amp<< std::endl;
}
                intgrl +=adcvalremainder;
              }

              else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
                adcval=prevadcval; //adcval-10;                                                                                                                                                                                           

                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "remainder: " << adcvalremainder << std::endl;
                outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
                  amp=adcvalremainder;
                  //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                             \
                                                                                                                                                                                                                              
                }
                intgrl +=adcvalremainder;

              }


              else{
		// adcval=(first16b & 0xfff);
                prevadcval=adcval;
		//	std::cout << std::dec << "new adc a6 : " <<adcval << " , baseline: " << baseline << std::endl;
		adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "6remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
		  //std::cout << "Amplitude 6: " << amp<< std::endl;
                }
                intgrl +=adcvalremainder;

              }
              samplecount++;
            }

	    else if(((first16b >> j+1) & 1) == 0){
              tot+=1;
              adcval=adcval-1;
	      //std::cout <<std::dec << "adc val a7: " << adcval << " \t" << prevadcval << std::endl;
              if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
                diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
                nearestPower = nearestPowerOf2(diff);
		//std::cout <<std::dec << "adc val a7: " << adcval << " \t" << prevadcval << std::endl;
		//std::cout << "diff a7: " << std::dec << diff << std::endl;
		//std::cout << "nearest power a7: " << nearestPower << std::endl;
		adcval = abs(adcval-nearestPower);
                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//	std::cout << "7 adc: " << std::dec <<adcval << " , baseline7: " << baseline << std::endl;
		//std::cout << "7 remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){ 
                  amp=adcvalremainder;
                }
                intgrl +=adcvalremainder;
              }

              else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
                adcval=prevadcval; //adcval-10;                                                                                                                                                                                           

                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
		//std::cout << "remainder: " << adcvalremainder << std::endl;
                outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
		if(adcvalremainder>amp){
                  amp=adcvalremainder;                                                                                                                                                                                             
                }
                intgrl +=adcvalremainder;

              }


              else{
		//                adcval=(first16b & 0xfff);
                prevadcval=adcval;
                adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	       	//std::cout << "7 adc: " << std::dec << adcval << " , baseline7: " << baseline << std::endl;
		//std::cout << "7remainder: " << std::dec << adcvalremainder << std::endl;
		outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
                if(adcvalremainder>amp){
                  amp=adcvalremainder;
                }
                intgrl +=adcvalremainder;

              }
              samplecount++;
            }
	  }}}

       else if (first16b>>12 == 0x3){
	proceed=false; //sometimes, a word defiing beg. of waveform is missing which overwrite the TP valuest. this bool will contrlol that
        tot+=1;
        adcval=(first16b & 0xfff);
	//	std::cout << "ADC amp: " << std::dec << adcval << std::endl;

	//std::cout << "End of waveforme" << std::endl;

        if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
          diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
          nearestPower = nearestPowerOf2(diff);
	  //std::cout <<std::dec << "adc val a8: " << adcval << " \t" << prevadcval << std::endl;
	  //std::cout << "diff a8: " << std::dec << diff << std::endl;
	  //std::cout << "nearest power a8: " << nearestPower << std::endl;
	  adcval = abs(adcval-nearestPower);
          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	  // std::cout << "baseline: " << std::dec << baseline << std::endl;
	  //std::cout << "8remainder: " << std::dec << adcvalremainder << std::endl;

          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl +=adcvalremainder;
	  //cout << "TP: " << std::dec  << tot<< " , " << amp<< " , " << intgrl << endl;
         if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	   std::cout <<"************* First 16 bits loop *********" << std::endl;
	   std::cout << "Check Frame: " <<std::dec << frame4 << std::endl; //hex_to_decimal_32u(frame4) << std::endl;                                        
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Amp: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

	    if(channel>31 and channel<64){
	      cout << "filling tplist" <<endl;
	      tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                            
	    }
	    }
	 tot=0;
	 amp=0;
	 intgrl=0;
        

	}

	else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	  adcval=prevadcval; //adcval-10;                                                                                                                                                                                           

	  prevadcval=adcval;
	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "remainder: " << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	  if(adcvalremainder>amp){
	    amp=adcvalremainder;
	    //std::cout << "Amplitude 1: " << amp << std::endl;                                                                                                                                                                    \
                                                                                                                                                                                                                                          
	  }
	  intgrl +=adcvalremainder;
	  //cout << "TP: " << std::dec  << tot<< " , " << amp<< " , " << intgrl << endl;
	  if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	    std::cout <<"************* First 16 bits loop *********" << std::endl;
	    std::cout << "Check Frame: " <<std::dec << frame4 << std::endl; //hex_to_decimal_32u(frame4) << std::endl;                                                                                                                    
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Amp: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

            if(channel>31 and channel<64){
              cout << "filling tplist" <<endl;
              tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                                                                                            
            }
	  }
	  tot=0;
	  amp=0;
	  intgrl=0;


	}

	else{
          //adcval=(first16b & 0xfff);
          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "8remainder: " << std::dec << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl +=adcvalremainder;
	  //cout << "TP: " << std::dec  << tot<< " , " << amp<< " , " << intgrl << endl;
	 if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	    std::cout <<"************* First 16 bits loop *********" << std::endl;
	    std::cout << "Check Frame: " <<std::dec << frame4 << std::endl; //hex_to_decimal_32u(frame4) << std::endl;                                       
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

	    if(channel>31 and channel<64){
	      cout << "filling tplist" <<endl;
	      tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                            
	    }
          }
	  
	 tot=0;
	  amp=0;
	 intgrl=0;


	}
	samplecount++;
} 
      
      
       if (last16b>>12 == 0x2){
	tot+=1;
	adcval=(last16b & 0xfff);
	//std::cout <<std::dec << "adc val a10: " << adcval << " \t" << prevadcval << std::endl;
	if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	  diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	  nearestPower = nearestPowerOf2(diff);
	  //std::cout <<std::dec << "adc val a10: " << adcval << " \t" << prevadcval << std::endl;
	  //std::cout << "diff a10: " << std::dec << diff << std::endl;
	  //std::cout << "nearest power a10: " << nearestPower << std::endl;
	  adcval = abs(adcval-nearestPower);
	  prevadcval=adcval;
	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "10 adc: " << std::dec <<adcval << " , baseline 10: " << baseline << std::endl;
	  //std::cout << "10 remainder: " << std::dec << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";

	  if(adcvalremainder>amp){
	    amp=adcvalremainder;
	  }
	  intgrl += adcvalremainder;
	}

        else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
          adcval=prevadcval; //adcval-10;                                                                                                                                                                                                 

          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "remainder: " << adcvalremainder << std::endl;
          outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl +=adcvalremainder;
	}
	else{
	  //adcval=(last16b & 0xfff);
	  prevadcval=adcval;
 	  adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "10 adc: " << std::dec <<adcval << " , baseline 10: " << baseline << std::endl;
	  //std::cout << "10 remainder: " << std::dec << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	  if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl += adcvalremainder;
	}
	samplecount++;
     }

       else if (last16b>>15 == 1) {
       int j=0;
       for (int i = 0; i < 15; ++i) {
	 if(((last16b >> i) & 1) == 1){
	   j=i;
	   if(((last16b >> j+1) & 1) == 0 and ((last16b >> j+2) & 1) == 0 and ((last16b >> j+3) & 1) == 0 and ((last16b >> j+4) & 1) == 0 and ((last16b>> j+5) & 1) ==0 and ((last16b >> j+6) & 1) == 0) {
	     tot+=1;
	     adcval=adcval+3;
	     // std::cout <<std::dec << "adc val a11: " << adcval << " \t" << prevadcval << std::endl;
	     if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	       diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	       nearestPower = nearestPowerOf2(diff);
	       //std::cout <<std::dec << "adc val a11: " << adcval << " \t" << prevadcval << std::endl;
	       //std::cout << "diff a11: " << std::dec << diff << std::endl;
	       //std::cout << "nearest power a11: " << nearestPower << std::endl;
	       adcval = abs(adcval-nearestPower);
	       prevadcval=adcval;
	       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";

	       //	       std::cout << "11 adc: " << std::dec <<adcval << " , baseline 11: " << baseline << std::endl;
	       //std::cout << "11 remainder: " << std::dec << adcvalremainder << std::endl;
	       if(adcvalremainder>amp){
		 amp=adcvalremainder;
	       }
	       intgrl += adcvalremainder;
	     }

	     else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	       adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

	       prevadcval=adcval;
	       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	       //std::cout << "remainder: " << adcvalremainder << std::endl;
	       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	       if(adcvalremainder>amp){
		 amp=adcvalremainder;
	       }
	       intgrl +=adcvalremainder;
	     }

	     else{
	       //   adcval=(last16b & 0xfff);
	       prevadcval=adcval;
	       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	       //std::cout << "11 adc: " << std::dec <<adcval << " , baseline 11: " << baseline << std::endl;
	       //std::cout << "11 remainder: " << std::dec << adcvalremainder << std::endl;
	       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	       if(adcvalremainder>amp){
		 amp=adcvalremainder;
	       }
	       intgrl += adcvalremainder;
	     }
	     samplecount++;
	   }


	   else if(((last16b >> j+1) & 1) == 0 and ((last16b >> j+2) & 1) == 0 and ((last16b >> j+3) & 1) == 0 and ((last16b >> j+4) & 1) == 0 and ((last16b>> j+5) & 1) == 0 ){
	     tot+=1;
	     adcval=adcval-3;
	     // std::cout <<std::dec << "adc val a12: " << adcval << " \t" << prevadcval << std::endl;
	   if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	     diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	     nearestPower = nearestPowerOf2(diff);
	     // std::cout <<std::dec << "adc val a12: " << adcval << " \t" << prevadcval << std::endl;
	     //std::cout << "diff a12: " << std::dec << diff << std::endl;
	     //std::cout << "nearest power a12: " << nearestPower << std::endl;

	     adcval = abs(adcval-nearestPower);
	     prevadcval=adcval;
	     adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	     outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
	     //std::cout << "12 adc: " << std::dec <<adcval << " , baseline 12: " << baseline << std::endl;
	     //std::cout << "12 remainder: " << std::dec << adcvalremainder << std::endl;

	     if(adcvalremainder>amp){
	       amp=adcvalremainder;
	     }
	     intgrl += adcvalremainder;
	   }

	   else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	     adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

	     prevadcval=adcval;
	     adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	     // std::cout << "remainder: " << adcvalremainder << std::endl;
	     outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	     if(adcvalremainder>amp){
	       amp=adcvalremainder;
	     }
	     intgrl +=adcvalremainder;
	   }


	   else{
	     //adcval=(last16b & 0xfff);
	     prevadcval=adcval;
	     adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	     //std::cout << "12 adc: " << std::dec <<adcval << " , baseline 12: " << baseline << std::endl;
	     //std::cout << "12 remainder: " << std::dec << adcvalremainder << std::endl;
	     outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";

	     if(adcvalremainder>amp){
	       amp=adcvalremainder;
	     }
	     intgrl += adcvalremainder;
	   }
	   samplecount++;
	   }

	   else if(((last16b >> j+1) & 1) == 0 and ((last16b >> j+2) & 1) == 0 and ((last16b >> j+3) & 1) == 0 and ((last16b >> j+4) & 1) == 0 ){
	 tot+=1;
	 adcval=adcval+2;
	 //std::cout <<std::dec << "adc val a13: " << adcval << " \t" << prevadcval << std::endl;
	 if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	   diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	   nearestPower = nearestPowerOf2(diff);
	   //std::cout <<std::dec << "adc val a13: " << adcval << " \t" << prevadcval << std::endl;
	   //std::cout << "diff a13: " << std::dec << diff << std::endl;
	   //std::cout << "nearest power a13: " << nearestPower << std::endl;
	   adcval = abs(adcval-nearestPower);
	   prevadcval=adcval;
	   adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	   //std::cout << "13 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
	   //std::cout << "13 remainder: " << std::dec << adcvalremainder << std::endl;
	   outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";

	   if(adcvalremainder>amp){
	     amp=adcvalremainder;
	   }
	   intgrl += adcvalremainder;
	 }

	 else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	   adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

	   prevadcval=adcval;
	   adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	   //std::cout << "remainder: " << adcvalremainder << std::endl;
	   outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	   if(adcvalremainder>amp){
	     amp=adcvalremainder;
	   }
	   intgrl +=adcvalremainder;
	 }


	 else{
	   //adcval=(last16b & 0xfff);
	   prevadcval=adcval;
	   adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	   //std::cout << "13 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
	   //std::cout << "13 remainder: " << std::dec << adcvalremainder << std::endl;
	   outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	   if(adcvalremainder>amp){
	     amp=adcvalremainder;
	   }
	   intgrl += adcvalremainder;
	 }
	 samplecount++;
       }

	   else if(((last16b >> j+1) & 1) == 0 and ((last16b >> j+2) & 1) == 0 and ((last16b >> j+3) & 1) == 0){
       tot+=1;
       adcval=adcval-2;
       //std::cout <<std::dec << "adc val a14: " << adcval << " \t" << prevadcval << std::endl;
       if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
	 diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
	 nearestPower = nearestPowerOf2(diff);
	 //std::cout <<std::dec << "adc val a14: " << adcval << " \t" << prevadcval << std::endl;
	 // std::cout << "diff a14: " << std::dec << diff << std::endl;
	 //std::cout << "nearest power a14: " << nearestPower << std::endl;
	 adcval = abs(adcval-nearestPower);
	 prevadcval=adcval;
	 adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	 //std::cout << "14 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
	 //std::cout << "14 remainder: " << std::dec << adcvalremainder << std::endl;
	 outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	 if(adcvalremainder>amp){
	   amp=adcvalremainder;
	 }
	 intgrl += adcvalremainder;
       }

       else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
	 adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

	 prevadcval=adcval;
	 adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	 //std::cout << "remainder: " << adcvalremainder << std::endl;
	 outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	 if(adcvalremainder>amp){
	   amp=adcvalremainder;
	 }
	 intgrl +=adcvalremainder;
       }


       else{
	 //adcval=(last16b & 0xfff);
	 prevadcval=adcval;
	 adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	 //std::cout << "14 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
	 //std::cout << "14 remainder: " << std::dec << adcvalremainder << std::endl;
	 outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
	 if(adcvalremainder>amp){
	   amp=adcvalremainder;
	 }
	 intgrl += adcvalremainder;
       }
       samplecount++;
	   }

	   else if(((last16b >> j+1) & 1) == 0 and ((last16b >> j+2) & 1) == 0 ){
     tot+=1;
     adcval=adcval+1;
     //     std::cout <<std::dec << "adc val a15: " << adcval << " \t" << prevadcval << std::endl;

     if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
       diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
       nearestPower = nearestPowerOf2(diff);
       //std::cout <<std::dec << "adc val a15: " << adcval << " \t" << prevadcval << std::endl;
       //std::cout << "diff a15: " << std::dec << diff << std::endl;
       //std::cout << "nearest power a15: " << nearestPower << std::endl;
       adcval = abs(adcval-nearestPower);
       prevadcval=adcval;
       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
       /// std::cout << "15 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
       //std::cout << "15 remainder: " << std::dec << adcvalremainder << std::endl;
       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
       if(adcvalremainder>amp){
	 amp=adcvalremainder;
       }
       intgrl += adcvalremainder;
     }

     else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
       adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

       prevadcval=adcval;
       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
       //std::cout << "remainder: " << adcvalremainder << std::endl;
       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
       if(adcvalremainder>amp){
	 amp=adcvalremainder;
       }
       intgrl +=adcvalremainder;
     }


     else{
       //adcval=(last16b & 0xfff);
       prevadcval=adcval;
       adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
       // std::cout << "15 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
       //std::cout << "15 remainder: " << std::dec << adcvalremainder << std::endl;
       outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
       if(adcvalremainder>amp){
	 amp=adcvalremainder;
       }
       intgrl += adcvalremainder;
     }
     samplecount++;
    }

	   else if(((last16b >> j+1) & 1) == 0){
    tot+=1;
    adcval=adcval-1;
    //std::cout <<std::dec << "adc val a16: " << adcval << " \t" << prevadcval << std::endl;

    if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
      diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
      nearestPower = nearestPowerOf2(diff);
      //std::cout <<std::dec << "adc val a16: " << adcval << " \t" << prevadcval << std::endl;
      //std::cout << "diff a16: " << std::dec << diff << std::endl;
      //std::cout << "nearest power a16: " << nearestPower << std::endl;
      adcval = abs(adcval-nearestPower);
      prevadcval=adcval;
      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
      //std::cout << "16 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
      //std::cout << "16 remainder: " << std::dec << adcvalremainder << std::endl;
      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
      if(adcvalremainder>amp){
	amp=adcvalremainder;
      }
      intgrl += adcvalremainder;
    }
    else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
      adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

      prevadcval=adcval;
      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
      //std::cout << "remainder: " << adcvalremainder << std::endl;
      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
      if(adcvalremainder>amp){
	amp=adcvalremainder;
      }
      intgrl +=adcvalremainder;
    }

    else{
      //adcval=(last16b & 0xfff);
      prevadcval=adcval;
      adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
      // std::cout << "16 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
      //std::cout << "16 remainder: " << std::dec << adcvalremainder << std::endl;
      outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
      if(adcvalremainder>amp){
	amp=adcvalremainder;
      }
      intgrl += adcvalremainder;
    }
    samplecount++;
	   }
	 }}}
       else if (last16b>>12 == 0x3){
       cout << " ***** last end of waveform " << endl;
       proceed=false;
       tot+=1;
	adcval=(last16b & 0xfff);
	//std::cout <<std::dec << "adc val a17: " << adcval << " \t" << prevadcval << std::endl;
	if (abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit) {
          diff = abs(static_cast<int>(adcval) - static_cast<int>(prevadcval));
          nearestPower = nearestPowerOf2(diff);
	  //std::cout <<std::dec << "adc val a17: " << adcval << " \t" << prevadcval << std::endl;
	  //std::cout << "diff a17: " << std::dec << diff << std::endl;
	  //std::cout << "nearest power a17: " << nearestPower << std::endl;
          adcval = abs(adcval-nearestPower);
          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  //std::cout << "17 adc: " << std::dec <<adcval << " , baseline: " << baseline << std::endl;
	  //std::cout << "17 remainder: " << std::dec << adcvalremainder << std::endl;
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount  << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl += adcvalremainder;
	   cout << "last TP: " << std::dec  << tot << " , " << amp << " , " << intgrl << endl;
	   if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	    std::cout <<"************* Last 16 bits loop *********" << std::endl;
	    std::cout << "Check Frame: " << std::dec << frame4 << std::endl; //hex_to_decim                                                                                                                     
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

	    if(channel>31 and channel<64){
	      cout << "filling tplist" << endl;
	      tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                                                                  
	    }
          }
	 
	   tot=0;
	   amp=0;
	   intgrl=0;

	}

        else if(abs(static_cast<int>(adcval) - static_cast<int>(prevadcval)) > limit2) {
          adcval=prevadcval; //adcval-10;                                                                                                                                                                                                                     

          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  // std::cout << "remainder: " << adcvalremainder << std::endl;
          outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl +=adcvalremainder;
	  cout << "last TP: " << std::dec  << tot << " , " << amp << " , " << intgrl << endl;
	  if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	    std::cout <<"************* Last 16 bits loop *********" << std::endl;
	    std::cout << "Check Frame: " << std::dec << frame4 << std::endl; //hex_to_decim                                                                                                                                                                   
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

            if(channel>31 and channel<64){
              cout << "filling tplist" << endl;
              tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                                                                                                                
            }
          }

	  tot=0;
	  amp=0;
	  intgrl=0;
        }


	else{
	  //  adcval=(last16b & 0xfff);
          prevadcval=adcval;
          adcvalremainder=std::abs(static_cast<int>(adcval)-static_cast<int>(baseline));
	  outFile << frame4 << " \t" << fem << " \t" << channel << " \t" << timetick << " \t" << samplecount << " \t" << adcvalremainder << " \n";
          if(adcvalremainder>amp){
            amp=adcvalremainder;
          }
          intgrl += adcvalremainder;
	  //cout << "last TP: " << std::dec  << tot << " , " << amp << " , " << intgrl << endl;
	 
	  
	  if(tot!=0 and amp!=0 and intgrl!=0 and frame4>1){
	    std::cout <<"************* Last 16 bits loop *********" << std::endl;
	    std::cout << "Check Frame: " << std::dec << frame4 << std::endl; //hex_to_decim                                                                                                                     
	    std::cout << "FEM number : " << std::dec << fem << std::endl;
	    std::cout <<  " Channel Number: " << std::dec << channel << std::endl;
	    std::cout << "ChannelMap : "<< CRATE << " , " << fem << " , " << channel << std::endl;
	    std::cout << "mapped wire: " << std::dec << mappedchannel << std::endl;
	    std::cout << "Final adc val: " << std::dec  << amp << std::endl;
	    std::cout << "tot: " <<  tot << std::endl;
	    std::cout << "intgrl: " << std::dec << intgrl << std::endl;
	    std::cout << "time: " << std::dec << timetick << std::endl;

	    if(channel>31 and channel<64){
	      cout << "filling tplist" << endl;
	      tp_list.emplace_back(minFrame,frame4,fem,mappedchannel,timetick,tot,amp,intgrl); //,run,subrun);                                                                                                  
	    }
          }
	  
	  tot=0;
          amp=0;
          intgrl=0;

	}
	samplecount++;
    
     }} 
else{
	continue;
      }
      

     //}
    }
    word_count++;
  }//closed while loop on reading a binary file                                                                                                                                                                                                                      

  binFile.close();
  //  }// close loop on binary files 

 }

 return 0;
                                                             
}
