//Code written by Daisy Kalra (June 20, 2022) dkalra@nevis.columbia.edu                                                                                     
// Binary decoder to decode SN data, calculate TPs and write TPs to a text file.                                                                             
// g++ -std=gnu++11 -o fil filter.cpp
//./fil

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
#include <iomanip>
#include <string>
#include <sstream>

#include <chrono>

#include <zmq.hpp>
#include <unistd.h>
#include <thread>

#include <dirent.h>
#include <cstring>
//#include <filesystem>



using namespace std;

std::string padWithZeros(int value) {
  std::ostringstream oss;
  oss << std::setw(5) << std::setfill('0') << value;
  return oss.str();
}

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
  TriggerPrimitive(uint32_t fr4a, uint32_t fma, uint32_t mca, uint16_t tta, int tot1a, uint64_t ama, uint64_t ina, int driftRegion_,int crateid_,int candidateBool_, int ra, int sra)

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
    run = ra;
    subrun = sra;
  }

};


int main() {

  zmq::context_t context(1);

  zmq::socket_t subscriber1(context, ZMQ_SUB);
  subscriber1.connect("tcp://127.0.0.1:7611");
  subscriber1.setsockopt(ZMQ_SUBSCRIBE, "", 0);


  std::ofstream DataFile;
  DataFile.open("Filtered1_seb02.bin", std::ios::binary);

  std::ofstream outputFile;
  outputFile.open("fil1_2time.txt");

  while(true) {

    //    auto start = std::chrono::system_clock::now();

    zmq::message_t message;
    subscriber1.recv(&message);
    std::cout << "message size: " << message.size() << std::endl;                                                                 auto start = std::chrono::system_clock::now();                           

    std::vector<TriggerPrimitive> myvec( static_cast<TriggerPrimitive*>(message.data()),
				      static_cast<TriggerPrimitive*>(message.data()) + message.size() / sizeof(TriggerPrimitive)
				      );

    //    for (const auto& elem : myvec) {

    //std::cout << "frame: " << elem.frame << " , FEM " <<  elem.fem << " , channel " << elem.channel << std::endl;
    //}

    //  std::vector<int> myvec = {5,4,7794,5,478,2350,6,2,1}; 
    //  uint32_t FilterFrame = myvec[0];

    for (const auto& elem : myvec) {

      uint32_t FilterFrame = elem.frame;
      std::cout  << "filter frame " <<  FilterFrame  <<  " , Channel: " << elem.channel << " , Timetick: " << elem.time_start << std::endl;

      //      int seb = myvec[7];
      int seb = elem.crateid;
      int rrun = elem.run;
      int ssubrun = elem.subrun;

      std::string subrunpattern = padWithZeros(ssubrun);

      std::string pattern2 = "seb02";// + std::to_string(seb);
      /*      std::string pattern3 = "seb03";
      std::string pattern4 = "seb04";
      std::string pattern5 = "seb05";
      std::string pattern6 = "seb06";
      std::string pattern7 = "seb07";
      std::string pattern8 = "seb08";
      std::string pattern9 = "seb09";
      */
      std::string runpattern =    std::to_string(rrun);


      //      std::string MyfileName = "RetrievedData_" runpattern + "_" + subrunpattern + "_" + pattern2;
      //std::ofstream DataFile(MyfileName, std::ios::binary);


      //       DataFile.open("Filtered1_seb02.bin", std::ios::binary);      

  //std::string subrunpattern = std::to_string(ssubrun);

      //std::cout << "Pattern: " << pattern2 << " , " << pattern3  <<  " , run " << runpattern << " , subrun pattern, " << subrunpattern << std::endl;

 const char* directory = "/data/kalra_TPCTriggerFiles/Nom_plus_20/"; // "/home/uboonedaq/kalra_seb01/Files/";
  DIR* dir = opendir(directory);
  if (dir == nullptr) {
    std::cerr << "Error opening directory." << std::endl;
    return 1;
    // break;
  }
  std::string fileExtension = ".ubdaq"; 
  // std::cout << "Directory is " << directory << std::endl;

  //  std::ofstream DataFile;
  //DataFile.open("Filtered.bin", std::ios::binary);
  bool breaknow=false;
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string fileName = entry->d_name;
    DataFile.flush();
    if(breaknow==true) break;
      
    std::string subrunmatch;
    size_t lastHyphenIndex = fileName.find_last_of('-');

    // Ensure that the last hyphen exists
    if (lastHyphenIndex != std::string::npos) {
     subrunmatch = fileName.substr(lastHyphenIndex + 1);

      // Remove any non-numeric characters from the beginning of the extracted value
      size_t firstDigitIndex = subrunmatch.find_first_of("0123456789");
      if (firstDigitIndex != std::string::npos) {
	subrunmatch = subrunmatch.substr(firstDigitIndex);

	// Find the position of the last dot ('.') to remove the extension
	size_t lastDotIndex = subrunmatch.find_last_of('.');
	if (lastDotIndex != std::string::npos) {
	  subrunmatch = subrunmatch.substr(0, lastDotIndex);
	}}}
      

    //    std::string  subrunmatch = fileName.substr(48,5);
    cout << "subrun pattern: " << subrunpattern << " , to match with: " << subrunmatch << endl;
    // Check if the filename has the desired file extension
    if (fileName.size() >= fileExtension.size() &&
	fileName.substr(fileName.size() - fileExtension.size()) == fileExtension) {
      // Check if the filename contains the pattern "seb02"
      if (fileName.find(pattern2) != std::string::npos && subrunpattern==subrunmatch){ // or (fileName.find(pattern3) != std::string::npos && subrunpattern==subrunmatch)){
	// Extract the SEB number from the filename
	////std::string sebNumber = fileName.substr(18,2); //3, 2);
	std::cout << "Found file: " << fileName <<  std::endl;



	/*  ****************************************************************************  */

	//Perform operation on binary file here
	int femHdrCount=0;
	int k=-1;
	int i =0;
	uint32_t frame, frame1, frame2, frame3, frame4, framebitsToskip;
	bool writingKeep = false;
	bool writing = false;
	bool writeNext = false;
	bool writePrev = false;
	int run,subrun;
	//bool breaknow=false;
	//	bool firstTime = false;
	std::vector<uint32_t> buffer;
	std::vector<uint32_t> prevbuffer;
	std::vector<uint32_t> nextbuffer;
	std::vector<uint32_t> newbuffer;
	std::vector<uint32_t> prevBufSize;
	//	std::streampos prevStartPos;
	//std::streampos prevEndPos;

	//std::vector<std::streampos> prevStartPosList;
	//std::vector<std::streampos> prevEndPosList;

	//int countPos=0;

	bool doincrement = false;
	//std::cout << "frame is " << FilterFrame << endl;
	//std::cout << std::hex << frame << std::endl;

	std::ifstream binFile;
	//	binFile.open("/home/uboonedaq/kalra_seb01/Files/"+fileName, std::ios::binary);
	//	binFile.open("/data/kalra_TPCTriggerFiles/FilteredData/"+fileName, std::ios::binary);
	binFile.open("/data/kalra_TPCTriggerFiles/Nom_plus_20/"+fileName, std::ios::binary);

	//	std::ofstream DataFile;
	//DataFile.open("Filtered.bin", std::ios::binary);

	if (!DataFile) {
	  std::cerr << "Error opening output file." << std::endl;
	  return 1;
	} 

	if( !binFile.is_open() ){
	  std::cerr << "ERROR: Could not open file " << std::endl;
	  return 0;
	}

	/*std::ofstream DataFile1;
	DataFile1.open("check.bin", std::ios::binary);

        if (!DataFile1) {
	  std::cerr << "Error opening output file." << std::endl;
          return 1;
	}
	*/
	int word_count = 0;

	/*	if(breaknow==true){
	  break;
	  }*/
      
	while( binFile.peek() != EOF ){
	  
	  //auto start = std::chrono::system_clock::now();
	  uint32_t word32b;
	  binFile.read( reinterpret_cast<char*>(&word32b), sizeof(word32b) );

	  std::cout.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield                                                    
	  std::cout.setf ( std::ios::showbase ); // activate showbase    i.e with prefix 0x                                                      
	  uint16_t first16b = word32b & 0xffff; //right 16 bit word                                                                              
	  uint16_t last16b = (word32b>>16) & 0xffff; // left 16 bit word                                                                         
	  //	  std::vector<uint32_t> buffer; // Buffer to hold 32-bit words 


	  if(word32b == 0xffffffff) {
	    // cout << "Start of frame ********************" << endl;  
	    femHdrCount=1;
	    doincrement=true;
	    writing=true;
	    //cout << "writing and od increment" << doincrement << writing << endl;
	    buffer.clear();
	    buffer.push_back(word32b);

	    //  cout << "start PrevBuffer Size " << std::dec << prevbuffer.size() << endl;    
   	    prevBufSize.push_back(prevbuffer.size());

	    prevbuffer.push_back(word32b);


	    //prevStartPos = binFile.tellg();
	    //cout << "Start Position" << prevStartPos << endl;
	    //cout << "word " << word32b << endl;

	    //prevStartPosList.push_back(prevStartPos);
	    //	    i+=1;


	    /*if (prevStartPos == -1) {
	      std::cerr << "Error getting file position." << std::endl;
	      return 1;
            }
            prevEndPos = -1;
	    */

	  }

	  
	  else if (word32b  == 0xe0000000){
	    //cout << "End of frame ********************" << endl;

	    prevbuffer.push_back(word32b);
	    //cout << "PrevBuffer Size " << std::dec << prevbuffer.size() << endl;	    
	    i+=1;
	    //cout << "I counter " << i << endl;

	    //   prevBufSize.push_back(prevbuffer.size());
	    /*  for(int ib=0; ib<prevBufSize.size(); ib++ ){
	      cout << "PrevBuf size" << prevBufSize[ib] << endl;

	      }*/

	    if(writing==true){
	      buffer.push_back(word32b);

	      //cout << "Write to file "  << buffer.size() << endl;	     
	      /* std::string runNumber = fileName.substr(42,5); 
	      run = std::stoi(runNumber);
	      std::string subrunNumber = fileName.substr(48,5);
	      subrun = std::stoi(subrunNumber);
	      std::cout << "Run: " << run <<  " ,Subrun " << subrun << std::endl;
	      */

	      for (const auto& wrd : buffer) {
		//		std::cout << "buffer content " << wrd << std::endl;
                DataFile.write(reinterpret_cast<const char*>(&wrd), sizeof(wrd));
		DataFile.flush();
	      }


	      writing=false;

	   
	     
	    }

	    if(writingKeep==true) {
	      //	      cout << "Write to file from next frame  "  << buffer.size() << endl;
	      buffer.push_back(word32b);
              for (const auto& wrd : buffer) {
                //              std::cout << "buffer content " << wrd << std::endl;                                                                                                                                        
                DataFile.write(reinterpret_cast<const char*>(&wrd), sizeof(wrd));
                DataFile.flush();
              }

              writingKeep=false;
	      writeNext=false;
	      breaknow = true;
	      //    return -1;
	      DataFile.flush();
	      //std::streampos fileSize = DataFile.tellp();
	      //cout << "File sixe :" << fileSize << endl;
 	      break;
	    }


	    /*prevEndPos = binFile.tellg();
      
	    cout << "End Position" << prevEndPos << endl;
	    cout << "word " << word32b << endl;
            prevEndPosList.push_back(prevEndPos);

	    if (prevEndPos == -1) {
	      std::cerr << "Error getting file position." << std::endl;
	      return 1;
	      }*/


	    /*
	    if(writePrev==true) {
	      while (binFile.tellg() != prevEndPos && binFile.read(reinterpret_cast<char*>(&word32b), sizeof(word32b))) {
		DataFile.write(reinterpret_cast<char*>(&word32b), sizeof(word32b));
	      }
	      }*/




	  }

	  else{

	    //	    cout << "befor end" << endl;
	    if(doincrement==true){ // or writeNext==true){   //we need this boolean so as to stop FEM cntr after evaluating frame number.
	      femHdrCount+=1;

	      if (femHdrCount==2 or femHdrCount==3 or femHdrCount==4 or femHdrCount==6 or femHdrCount==7 ){
		buffer.push_back(word32b);
		prevbuffer.push_back(word32b);

		//		cout << "buffer content " << buffer[0] << endl;
	      }

	      if (femHdrCount==5){ // FEM frame number               
		buffer.push_back(word32b);
		prevbuffer.push_back(word32b);
                                                                                                   
		frame = (first16b & 0xfff);
		frame1 = (last16b>>6 & 0x3f);
		framebitsToskip = (last16b & 0x3f);   // last 6 bits of FEM frame number                                                          \
                                                                                                                                             
	      }
	      if (femHdrCount==8){ //ROI frame number                                                                                                                              

		buffer.push_back(word32b);
		prevbuffer.push_back(word32b);

		if(first16b>>12 == 0x1){
		  frame2=(first16b>>6) & 0x3f;
		  if ((framebitsToskip-frame2)>=0){
		    frame3 = (frame<<12)+(frame1<<6)+(frame2);
		    frame4 = (frame3 & 0xffffff);
		    //std::cout << "frame " <<  std::dec << frame4 << std::endl;

		    if(frame4==FilterFrame) {
		      start = std::chrono::system_clock::now();  
		      writing = true;
		      writeNext = true;
		      writePrev=true;

		      //     cout << "i counter " << std::dec<< i << endl;
		      //cout << "Check sizes of prev bufgfer :  " << std::dec << prevBufSize[i-1] << " , " << prevBufSize[i] << endl;
	
		      for (int ipb=prevBufSize[i-1]; ipb<prevBufSize[i]; ipb++ ){
	      		newbuffer.push_back(prevbuffer[ipb]);
      
		      }
		      //cout << "new buffer size " << newbuffer.size() << endl;


		      for (const auto& wrd : newbuffer) {
			//              std::cout << "buffer content " << wrd << std::endl;                                                                                              
			DataFile.write(reinterpret_cast<const char*>(&wrd), sizeof(wrd));
			DataFile.flush();
		      }
}

		    else{
		      if (writeNext==true){
			writingKeep = true;
			writing = false;
		      }
		      else{
		      writing=false;
		      buffer.clear();
		      }

		      //prevStartPos = -1;
		      //prevEndPos = -1;

		    }
		  }

		  else if ((framebitsToskip-frame2)<0){
		    frame3 = (frame<<12)+(frame1<<6)+(frame2);
		  }

		  k+=1;
		  if (k!=(frame3 & 0xffffff)){
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
		    //		    std::cout << "frame " <<  std::dec << frame4 << std::endl;

		    if(frame4==FilterFrame) {
		      start = std::chrono::system_clock::now();  
		      writing = true;
                      writeNext = true;
		      writePrev=true;
		      //cout << "wrietPreb" << writePrev << endl;
		   
		      for (int ipb=prevBufSize[i-1]; ipb<prevBufSize[i]; ipb++ ){
                        newbuffer.push_back(prevbuffer[ipb]);

                      }
                      //cout << "new buffer size " << newbuffer.size() << endl;


                      for (const auto& wrd : newbuffer) {
                        DataFile.write(reinterpret_cast<const char*>(&wrd), sizeof(wrd));
                        DataFile.flush();
                      }

		    }
		 
		    
		    else{

		      if (writeNext==true){
			writingKeep = true;
			writing = false;
	      } 
                      else{
			writing=false;
			buffer.clear();
                      }

		      //prevStartPos = -1;
                      //prevEndPos = -1;

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

            else if(doincrement==false and writing==true) {
              buffer.push_back(word32b);
            }
	    else if(doincrement==false and writingKeep==true) {
              buffer.push_back(word32b);
            }

	    else if (doincrement==false) {
	      prevbuffer.push_back(word32b);
	    }

	  }}

        /*  ****************************************************************************  */


	}// patter
    }
  }


    
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //    auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start1);
    std::cout << "tot time: " << std::dec << elapsed.count() << " microseconds" << std::endl;
    // std::cout << "time after receiving msgs: " << std::dec << elapsed1.count() << " microseconds" << std::endl;

     outputFile.flush();
     outputFile << elapsed.count() <<  endl;
    }
}
  DataFile.flush();
  DataFile.close();



}//end of main function
