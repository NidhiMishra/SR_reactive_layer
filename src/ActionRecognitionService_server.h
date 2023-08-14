#pragma once
#include "ActionRecognitionService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "I2P_types.h"
#include <string>
#include "reactiveLayer.h"
#include "Inputs_types.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
using namespace  ::imi;
using namespace ::BB;

class ActionRecognitionServiceHandler : virtual public ActionRecognitionServiceIf {
 public:
	ProtectedClient<imi::AgentControlClient> *Client;
	ActionRecognitionServiceHandler(reactiveLayer *aDecision);
	void queryActionRecogition(const std::vector<UserAction> & userActions);
	reactiveLayer *Decision;
  
};