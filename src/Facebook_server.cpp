// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Facebook.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
#include <iostream>
using namespace  ::imi;
#include <string>
using namespace ::std;

class FacebookHandler : virtual public FacebookIf {
 public:
	 string toSend1;
  FacebookHandler() {
	 
	  toSend1 = "NO";
    // Your initialization goes here
  }

  void postMessage(const std::string& message) {
    // Your implementation goes here

    printf("postMessage\n");
  }

  void postMessageAndPicture(const std::string& message, const std::string& picture) {
    // Your implementation goes here

    printf("postMessageAndPicture\n");
  }

  void takePicture(std::vector<std::string> & _return) {
    // Your implementation goes here
	std::string aSentence1 = toSend1;
//	std::cout << "Facebook is sending " << toSend1 << "\n";
	toSend1 = "NO";
	_return.push_back(aSentence1);
  }

void takepic(string aString)
 {
	 toSend1 = aString;
 }

};
