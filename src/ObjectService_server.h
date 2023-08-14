#pragma once
#include "ObjectDetectionService.h"
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

class ObjectServiceHandler : virtual public ObjectDetectionServiceIf {
 public:
	ProtectedClient<imi::AgentControlClient> *Client;
	ObjectServiceHandler(reactiveLayer *aDecision);
	void objectRecognized(const std::vector< ::imi::ObjectInfo> & objects);
	reactiveLayer *Decision;
};


