#ifndef NEWSPEECHRECOG
#define NEWSPEECHRECOG
#include <iostream>
#include "NewSpeechRecognitionService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "reactiveLayer.h"
#include <string>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::imi;
using namespace ::std;

class NewSpeechRecognitionServiceHandler : virtual public NewSpeechRecognitionServiceIf {
 public:

	 string toSend;
  NewSpeechRecognitionServiceHandler() {
    // Your initialization goes here
	  toSend = "";
  }




 void NewsentenceRecognized(std::vector<std::string> & _return) {
    // Your implementation goes here
	std::string aSentence = toSend;
	std::cout << "NewSpeech is sending " << toSend << "\n";
	_return.push_back(aSentence);
  }

 void sendNewString(string aString)
 {
	 toSend = aString;
 }

};

#endif