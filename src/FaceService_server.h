#pragma once
#include "FaceService.h"
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

class FaceServiceHandler : virtual public FaceServiceIf {
 public:
	ProtectedClient<imi::AgentControlClient> *Client;
	FaceServiceHandler(reactiveLayer *aDecision);
	
	FaceStatus::type faceRecognized(const std::string& sensorID, const  ::imi::Microseconds timestamp, const std::string& name, const  ::imi::Vec2& position,const ::imi::Facial_Expression::type exp);
	void emotionRecognized(const std::string& sensorID, const  ::imi::Microseconds timestamp, const  ::imi::Emotion::type emotion, const std::string& name);

	FaceStatus::type updateFaceRecognitionStatus(const std::string& sensorID, const  ::imi::Microseconds timestamp, const FaceStatus::type status);
	reactiveLayer *Decision;
};


