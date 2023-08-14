// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "ProtectedClient.h"
#include "SpeechRecognitionService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "Behavior.h"
#include <iostream>
#include "reactiveLayer.h"
#include <string>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::imi;
using namespace ::BB;
class SpeechRecognitionServiceHandler : virtual public SpeechRecognitionServiceIf

{
 public:
	 reactiveLayer *Decision;
  SpeechRecognitionServiceHandler(reactiveLayer *aDecision) {
    // Your initialization goes here
	  Decision=aDecision;
  }

  bool sentenceRecognized(const std::string& sensorID, const  ::imi::Microseconds timestamp, const std::string& sentence, const int32_t confidence) {
	  perceptualInput aInput;
	  aInput.sensorId="Speech";
	  aInput.sentence=sentence;
	  std::cout << "Starting " << sentence<<"\n";
	  Decision->updateActivationLevels(aInput);
	  std::cout << "Finished " << sentence<<"\n";
	  return true;
  }

  void LanguageRecognized(const std::string& sensorID, const  ::imi::Microseconds timestamp, const std::string& language){
  }
};

